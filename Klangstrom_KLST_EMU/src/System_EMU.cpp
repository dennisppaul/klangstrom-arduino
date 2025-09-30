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

#include "KlangstromEnvironment.h"

#include <KlangstromEmulator.h>
#ifdef KLST_ARCH_IS_EMU

#define KLST_DISPLAY_WIDTH                 480
#define KLST_DISPLAY_HEIGHT                272

#ifndef KLST_EMU_EXTMEM_SIZE
#ifdef KLST_PANDA_EMU
#define KLST_EMU_EXTMEM_SIZE ((16 * 1024 * 1024) - (KLST_DISPLAY_WIDTH * KLST_DISPLAY_HEIGHT * 4 * 2)) // 16 MB
#elif defined(KLST_CATERPILLAR_EMU)
#define KLST_EMU_EXTMEM_SIZE (0)
#elif
#define KLST_EMU_EXTMEM_SIZE (0)
#endif
#endif

#include <chrono>
#include "System.h"
#include "Console.h"
#include "PeriodicTimer_EMU.h"
#include "stm32_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace std::chrono;

static uint64_t mStartTime = 0;

void system_enable_cycle_counter(bool enable) {
    if (enable) {
        system_reset_cycles();
    } else {
        mStartTime = 0;
    }
}

void system_reset_cycles() {
    auto now   = std::chrono::high_resolution_clock::now();
    mStartTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

uint32_t system_get_cycles() {
    auto now          = std::chrono::high_resolution_clock::now();
    auto mCurrentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    auto mDelta       = mCurrentTime - mStartTime;
    mDelta *= system_clock_frequency();
    mDelta /= 1e9;
    return static_cast<uint32_t>(mDelta);
}

uint32_t system_clock_frequency() {
    return HAL_RCC_GetSysClockFreq();
}

AudioDevice* system_init_audiocodec() {
    // TODO move this to BSP?
    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    AudioDevice* audiodevice  = audiodevice_init_audiocodec(&audioinfo);
    if (audioinfo.device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_timestamp(false);
        console_error("error initializing audio device");
    }
    audiodevice_resume(audiodevice);
    return audiodevice;
}

static KlangstromEmulatorClient client;
static uint8_t*                 emu_extmem_block = nullptr;
static uintptr_t                emu_base         = 0;
static size_t                   emu_size         = 0;

void system_init_BSP() {
    console_status("----------------------------------------------------------------------------------------------------");
    console_status("system init: registering client at emulator");
    console_status("----------------------------------------------------------------------------------------------------");
    umfeld::KlangstromEmulator::instance()->register_client(&client);

    /* emulate external memory */
    if (emu_extmem_block) { free(emu_extmem_block); }
    emu_extmem_block = static_cast<uint8_t*>(malloc(KLST_EMU_EXTMEM_SIZE));
    if (!emu_extmem_block) {
        return;
    }
    emu_base = reinterpret_cast<uintptr_t>(emu_extmem_block);
    emu_size = KLST_EMU_EXTMEM_SIZE;
    system_external_memory_init(emu_base, emu_size);
}

uint32_t system_get_ticks_BSP() {
    return HAL_GetTick();
}

void HAL_TIM_PeriodElapsedCallback(PeriodicTimer* htim) {
    ArrayList_PeriodicTimerPtr* mTimers = system_get_registered_periodic_timers();
    if (mTimers == nullptr) {
        return;
    }
    for (size_t i = 0; i < mTimers->size; i++) {
        PeriodicTimer* t = arraylist_PeriodicTimerPtr_get(mTimers, i);
        if (t != nullptr && t->peripherals != nullptr) {
            if (t->peripherals->timer_handle == htim->peripherals->timer_handle &&
                t->timer_id == htim->timer_id) {
                t->callback(t);
            }
        }
    }
}

/* ----------------- GPIO --------------- */

void HAL_GPIO_EXTI_Callback(const uint16_t GPIO_Pin) {
    ArrayList_GPIOListenerPtr* fGPIOListeners = system_get_registered_gpio_listener();
    for (size_t i = 0; i < fGPIOListeners->size; i++) {
        const GPIOListener* gpio = arraylist_GPIOListenerPtr_get(fGPIOListeners, i);
        if (gpio != nullptr) {
            gpio->callback(GPIO_Pin);
        }
    }
}

/* --- UID --- */

unsigned long system_get_UID(const uint8_t offset) {
    if (offset > 3) {
        return 0;
    }
    constexpr unsigned long _fake_uid[4] = {0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0};
    return _fake_uid[offset];
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
