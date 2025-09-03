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

#ifdef KLST_ARCH_IS_STM32

#include "main.h"
#include "tim.h"
#include "System.h"
#include "Timer.h"
#include "Timer_STM32.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

bool timer_init_peripherals_BSP(Timer* timer) {
    timer->peripherals = new TimerPeripherals();
    if (timer->timer_id == 7) {
        TimerPeripherals& peripherals = *timer->peripherals;
        peripherals.timer_number      = 7;
        peripherals.timer_handle      = &htim7;
        peripherals.prescaler         = 5500;
        peripherals.period            = 50000;
        MX_TIM7_Init();
        timer_update_BSP(timer);
        return true;
    }
    if (timer->timer_id == 13) {
        TimerPeripherals& peripherals = *timer->peripherals;
        peripherals.timer_number      = 13;
        peripherals.timer_handle      = &htim13;
        peripherals.prescaler         = 5500;
        peripherals.period            = 50000;
        MX_TIM13_Init();
        timer_update_BSP(timer);
        return true;
    }
    if (timer->timer_id == 14) {
        TimerPeripherals& peripherals = *timer->peripherals;
        peripherals.timer_number      = 14;
        peripherals.timer_handle      = &htim14;
        peripherals.prescaler         = 5500;
        peripherals.period            = 50000;
        MX_TIM14_Init();
        timer_update_BSP(timer);
        return true;
    }
    return false;
}

void timer_deinit_peripherals_BSP(Timer* timer) {
    // TODO unregister timer from BSP
    delete timer->peripherals;
}

void timer_resume(Timer* timer) {
    const HAL_StatusTypeDef result = HAL_TIM_Base_Start_IT(timer->peripherals->timer_handle);
    if (result != HAL_OK) {
        console_error("timer_resume: ", timer->timer_id);
    }
}

void timer_pause(Timer* timer) {
    const HAL_StatusTypeDef result = HAL_TIM_Base_Stop_IT(timer->peripherals->timer_handle);
    if (result != HAL_OK) {
        console_error("timer_pause: ", timer->timer_id);
    }
}

void timer_set_prescale_period(Timer* timer, const uint32_t prescaler, const uint32_t period) {
    // TODO this is crude and should be improved!
    // ( PRESCALER * 1000000 ) / APB_CLK = microseconds per tick
    timer->peripherals->prescaler = prescaler;
    timer->peripherals->period    = period;
    const uint64_t sysclock_half  = system_clock_frequency() / 2;
    const uint64_t tick_duration  = prescaler * 1000000 / sysclock_half;
    console_println("tick_duration   (µs): %i", static_cast<uint32_t>(tick_duration));
    console_println("period_duration (µs): %i", static_cast<uint32_t>(tick_duration * period));
    timer_update_BSP(timer);
}

void timer_set_overflow(Timer* timer, const uint32_t duration_us) {
    // TODO this is crude and should be improved!
    // ( PRESCALER * 1000000 ) / APB_CLK = microseconds per tick
    const uint64_t prescaler     = timer->peripherals->prescaler;
    const uint64_t sysclock_half = system_clock_frequency() / 2;
    const uint64_t tick_duration = prescaler * 1000000 / sysclock_half;
    timer->peripherals->period   = duration_us / tick_duration;
    timer_update_BSP(timer);
}

void timer_update_BSP(Timer* timer) {
    const TimerPeripherals& peripherals = *timer->peripherals;
    TIM_HandleTypeDef*      htim        = peripherals.timer_handle;
    __HAL_TIM_SET_PRESCALER(htim, peripherals.prescaler - 1);
    __HAL_TIM_SET_AUTORELOAD(htim, peripherals.period - 1);
    __HAL_TIM_SET_COUNTER(htim, 0);
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32