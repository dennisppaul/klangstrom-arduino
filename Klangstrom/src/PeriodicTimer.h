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

#include <cstdint>
#include <functional>

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

constexpr int8_t PERIODIC_TIMER_INIT_INCOMPLETE = -1;

struct PeriodicTimerPeripherals; /* BSP or ASP implementation */
struct PeriodicTimer;

typedef void (*Callback_1_PERIODICTIMERPTR)(PeriodicTimer*);

typedef struct PeriodicTimer {
    PeriodicTimerPeripherals*           peripherals = nullptr;
    int8_t                              timer_id    = PERIODIC_TIMER_INIT_INCOMPLETE;
    std::function<void(PeriodicTimer*)> callback    = nullptr;
} PeriodicTimer;

WEAK void periodic_timer_event(PeriodicTimer* timer);

#ifdef __cplusplus
extern "C" {
#endif

PeriodicTimer* periodic_timer_create(uint8_t timer_id);
void           periodic_timer_delete(PeriodicTimer* timer);
void           periodic_timer_resume(PeriodicTimer* timer);                                                   // implemented as BSP
void           periodic_timer_pause(PeriodicTimer* timer);                                                    // implemented as BSP
void           periodic_timer_set_overflow(PeriodicTimer* timer, uint32_t duration_us);                       // implemented as BSP
void           periodic_timer_set_prescale_period(PeriodicTimer* timer, uint32_t prescaler, uint32_t period); // implemented as BSP

bool periodic_timer_init_peripherals_BSP(PeriodicTimer* timer);
void periodic_timer_deinit_peripherals_BSP(PeriodicTimer* timer);
void periodic_timer_update_BSP(PeriodicTimer* timer);

#ifdef __cplusplus
}
#endif
