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

#include "System.h"
#include "PeriodicTimer.h"

WEAK void periodic_timer_event(PeriodicTimer* timer) {
    (void) timer;
}

#ifdef __cplusplus
extern "C" {
#endif

PeriodicTimer* periodic_timer_create(const uint8_t timer_id) {
    auto* timer     = new PeriodicTimer();
    timer->timer_id = timer_id;
    timer->callback = periodic_timer_event;
    if (periodic_timer_init_peripherals_BSP(timer)) {
        system_register_periodic_timer(timer);
    } else {
        timer->timer_id = PERIODIC_TIMER_INIT_INCOMPLETE;
    }
    return timer;
}

void periodic_timer_delete(PeriodicTimer* timer) {
    periodic_timer_deinit_peripherals_BSP(timer);
    delete timer;
}

#ifdef __cplusplus
}
#endif
