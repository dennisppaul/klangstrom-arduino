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

#include "Klangstrom.h"

#ifdef KLST_ARCH_IS_EMU

#include "System.h"
#include "Timer.h"
#include "Timer_EMU.h"
#include "Console.h"
#include "stm32.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void HAL_TIM_PeriodElapsedCallback(Timer* htim);

bool timer_init_peripherals_BSP(Timer* timer) {
    timer->peripherals = new TimerPeripherals();
    if (timer->timer_id == 7) {
        TimerPeripherals& peripherals = *timer->peripherals;
        peripherals.timer_number      = 7;
        peripherals.timer_handle      = new HardwareTimer(TIM7);
        timer->peripherals->timer_handle->attachInterrupt(
            [timer]() {
                if (timer) {
                    HAL_TIM_PeriodElapsedCallback(timer);
                }
            });
        timer_update_BSP(timer);
        return true;
    }
    if (timer->timer_id == 13) {
        TimerPeripherals& peripherals = *timer->peripherals;
        peripherals.timer_number      = 13;
        peripherals.timer_handle      = new HardwareTimer(TIM13);
        timer->peripherals->timer_handle->attachInterrupt(
            [timer]() {
                if (timer) {
                    HAL_TIM_PeriodElapsedCallback(timer);
                }
            });
        timer_update_BSP(timer);
        return true;
    }
    if (timer->timer_id == 14) {
        TimerPeripherals& peripherals = *timer->peripherals;
        peripherals.timer_number      = 14;
        timer->peripherals->timer_handle->attachInterrupt(
            [timer]() {
                if (timer) {
                    HAL_TIM_PeriodElapsedCallback(timer);
                }
            });
        timer_update_BSP(timer);
        return true;
    }
    return false;
}

void timer_deinit_peripherals_BSP(Timer* timer) {
    delete timer->peripherals;
}

void timer_resume(Timer* timer) {
    if (timer == nullptr) {
        return;
    }
    if (timer->peripherals == nullptr) {
        return;
    }
    if (timer->peripherals->timer_handle == nullptr) {
        console_println("timer_resume: timer_handle is null");
        return;
    }
    timer->peripherals->timer_handle->resume();
}

void timer_pause(Timer* timer) {
    timer->peripherals->timer_handle->pause();
}

void timer_set_prescale_period(Timer* timer, const uint32_t prescaler, const uint32_t period) {
    // ( PRESCALER * 1000000 ) / APB_CLK = microseconds per tick
    const uint64_t sysclock_half = system_clock_frequency() / 2;
    const uint64_t tick_duration = prescaler * 1000000 / sysclock_half;
    timer->peripherals->timer_handle->setOverflow(tick_duration * period, MICROSEC_FORMAT);
    // TODO check if this ^^^^ works
}

void timer_set_overflow(Timer* timer, const uint32_t duration_us) {
    timer->peripherals->timer_handle->setOverflow(duration_us, MICROSEC_FORMAT);
}

void timer_update_BSP(Timer* timer) {
    // NOTE this is not necessary for the emulated timer
    (void) timer;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU