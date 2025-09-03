/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
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

#include <iostream>

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
                                                    fPaused(false) {
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

private:
    void run() {
        auto nextCallTime = std::chrono::high_resolution_clock::now();
        while (fRunning) {
            {
                std::unique_lock<std::mutex> lock(fMutex);
                if (fPaused) {
                    fCV.wait(lock, [this]() { return !fPaused || !fRunning; });
                    nextCallTime = std::chrono::high_resolution_clock::now();
                }
            }

            if (!fRunning) {
                break;
            }

            nextCallTime += std::chrono::microseconds(fDuration_us);
            if (fCallback) {
                fCallback();
            }
            std::this_thread::sleep_until(nextCallTime);
        }
    }

    void start() {
        fRunning     = true;
        fTimerThread = std::thread([this]() { run(); });
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
};
