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

#include <cstdint>
#include <functional>

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

constexpr int8_t TIMER_INIT_INCOMPLETE = -1;

struct TimerPeripherals; /* BSP or ASP implementation */
struct Timer;

typedef void (*Callback_1_TIMERPTR)(Timer*);

typedef struct Timer {
    TimerPeripherals*           peripherals = nullptr;
    int8_t                      timer_id    = TIMER_INIT_INCOMPLETE;
    std::function<void(Timer*)> callback    = nullptr;
} Timer;

WEAK void timer_event(Timer* timer);

#ifdef __cplusplus
extern "C" {
#endif

Timer* timer_create(uint8_t timer_id);
void   timer_delete(Timer* timer);

void timer_resume(Timer* timer);                             // implemented as BSP
void timer_pause(Timer* timer);                              // implemented as BSP
void timer_set_overflow(Timer* timer, uint32_t duration_us); // implemented as BSP

bool timer_init_peripherals_BSP(Timer* timer);
void timer_deinit_peripherals_BSP(Timer* timer);
void timer_update_BSP(Timer* timer);

#ifdef __cplusplus
}
#endif
