/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2025 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// NOTE this class is a bit of experiment in a sense that it tries to not only replicate the behaviour of
//      the underlying hardware but also the implementation in STM32duino. this could be extended to other
//      class or functions like e.g Serial, SPI, digitalread/write
// TODO and how does this work in standalone mode?

#include <thread>
#include <functional>
#include <stdint.h>

#include "Console.h"
#include "stm32.h"

#include <atomic>
#include <mutex>
#include <condition_variable>

// Platform-specific includes for priority/affinity
#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#if defined(__linux__)
#include <sys/resource.h>
#include <sys/syscall.h>
#endif
#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>
#include <mach/thread_policy.h>
#include <pthread.h>
#endif
#endif

typedef enum {
    TICK_FORMAT, // default
    MICROSEC_FORMAT,
    HERTZ_FORMAT,
} TimerFormat_t;

using callback_function_t = std::function<void()>;

class HardwareTimer {
public:
    explicit HardwareTimer(TIM_TypeDef* instance) : fTimerInstance(instance),
                                                    fCallback(nullptr),
                                                    fDuration_us(0),
                                                    fRunning(false),
                                                    fPaused(false),
                                                    fSpinMicros(0),
                                                    fAffinityCore(-1),
                                                    fRequestHighPriority(true) {
        (void) fTimerInstance;
    }

    void attachInterrupt(const callback_function_t& callback) {
        fCallback = callback;
        // fCallback = std::move(callback);
        // TOOD ^^^ check if `move` might be better, safer, ...
    }

    void setOverflow(const uint32_t duration, const TimerFormat_t format) {
        if (format == TICK_FORMAT) {
            // NOTE this assumes that tick is set to 1ms which is the default but of course not entirely correct
            fDuration_us = duration * 1000;
        } else if (format == HERTZ_FORMAT) {
            fDuration_us = 1000000 / duration;
        } else {
            fDuration_us = duration;
        }
    }

    void resume() {
        if (!fRunning) {
            start();
        }
        {
            std::lock_guard<std::mutex> lock(fMutex);
            fPaused = false;
        }
        fCV.notify_all();
    }

    void pause() {
        std::lock_guard<std::mutex> lock(fMutex);
        fPaused = true;
    }

    bool isRunning() const {
        return fRunning && !fPaused;
    }

    ~HardwareTimer() {
        stop();
    }

    // --- Tuning API ---
    // Busy-wait window before each deadline in microseconds (0 disables busy-wait)
    void setSpinWindowMicros(const int spin_us) {
        fSpinMicros = spin_us < 0 ? 0 : spin_us;
    }

    // Pin timer thread to a specific core (-1 disables pinning)
    void setAffinityCore(const int coreIndex) {
        fAffinityCore = coreIndex;
    }

    // Request elevated scheduling priority (may require privileges on POSIX)
    void setRequestHighPriority(const bool enable) {
        fRequestHighPriority = enable;
    }

private:
    void run() {
        using clock       = std::chrono::steady_clock; // monotonic
        auto period       = std::chrono::microseconds(fDuration_us);
        auto nextCallTime = clock::now();
        while (fRunning) {
            {
                std::unique_lock<std::mutex> lock(fMutex);
                if (fPaused) {
                    fCV.wait(lock, [this]() { return !fPaused || !fRunning; });
                    nextCallTime = clock::now();
                }
            }

            if (!fRunning) {
                break;
            }

            nextCallTime += std::chrono::microseconds(fDuration_us);
            if (fCallback) {
                fCallback();
            }

            // Sleep-then-optional-spin for higher precision
            if (fSpinMicros > 0) {
                auto spinWindow   = std::chrono::microseconds(fSpinMicros);
                auto sleepUntilTs = nextCallTime - spinWindow;
                auto now          = clock::now();
                if (sleepUntilTs > now) {
                    std::this_thread::sleep_until(sleepUntilTs);
                }
                // Busy-wait until deadline
                while (clock::now() < nextCallTime) {
#if defined(_MSC_VER)
                    _mm_pause();
#elif defined(__x86_64__) || defined(__i386__)
                    asm volatile("pause");
#else
                    asm volatile("" ::: "memory");
#endif
                }
            } else {
                std::this_thread::sleep_until(nextCallTime);
            }
        }
    }

    void start() {
        fRunning     = true;
        fTimerThread = std::thread([this]() {
        // Apply affinity/priority best-effort at thread start
#if defined(_WIN32)
            if (fRequestHighPriority) {
                // Elevate thread priority
                SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
                // Optional: improve system timer resolution (ignored if unavailable)
                timeBeginPeriod(1);
            }
            if (fAffinityCore >= 0) {
                DWORD_PTR mask = (DWORD_PTR) 1 << (DWORD) fAffinityCore;
                SetThreadAffinityMask(GetCurrentThread(), mask);
            }
#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
            pthread_t thr = pthread_self();
            if (fRequestHighPriority) {
#if defined(__linux__)
                struct sched_param sp{};
                sp.sched_priority = 80;                      // typical RT range 1..99
                pthread_setschedparam(thr, SCHED_FIFO, &sp); // may require privileges
#else
                struct sched_param sp{};
                sp.sched_priority = 31;
                pthread_setschedparam(thr, SCHED_RR, &sp);
#endif
            }
            if (fAffinityCore >= 0) {
#if defined(__linux__)
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET((size_t) fAffinityCore, &cpuset);
                pthread_setaffinity_np(thr, sizeof(cpu_set_t), &cpuset);
#elif defined(__APPLE__) && defined(__MACH__)
                // Advisory affinity on macOS
                thread_affinity_policy_data_t policy      = {static_cast<integer_t>(fAffinityCore + 1)};
                thread_port_t                 mach_thread = pthread_mach_thread_np(thr);
                thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t) &policy, 1);
#endif
            }
#endif
            run();
#if defined(_WIN32)
            if (fRequestHighPriority) {
                timeEndPeriod(1);
            }
#endif
        });
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(fMutex);
            fRunning = false;
            fPaused  = false;
        }
        fCV.notify_all();
        if (fTimerThread.joinable()) {
            fTimerThread.join();
        }
    }

    TIM_TypeDef*            fTimerInstance;
    callback_function_t     fCallback;
    int                     fDuration_us;
    std::atomic<bool>       fRunning;
    bool                    fPaused;
    std::thread             fTimerThread;
    std::mutex              fMutex;
    std::condition_variable fCV;

    // Tuning
    int  fSpinMicros;   // busy-wait window before deadline (us), 0 to disable
    int  fAffinityCore; // -1 = no pinning
    bool fRequestHighPriority;

    // // Platform thread handles
    // #if defined(_WIN32)
    //     void* fNativeHandle = nullptr;
    // #elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    //     pthread_t fPthreadHandle{};
    // #endif
};
