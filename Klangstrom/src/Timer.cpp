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

#include "System.h"
#include "Timer.h"

WEAK void timer_event(Timer* timer) {
    (void) timer;
}

#ifdef __cplusplus
extern "C" {
#endif

Timer* timer_create(const uint8_t timer_id) {
    auto* timer     = new Timer();
    timer->timer_id = timer_id;
    timer->callback = timer_event;
    if (timer_init_peripherals_BSP(timer)) {
        system_register_timer(timer);
    } else {
        timer->timer_id = TIMER_INIT_INCOMPLETE;
    }
    return timer;
}

void timer_delete(Timer* timer) {
    timer_deinit_peripherals_BSP(timer);
    delete timer;
}

#ifdef __cplusplus
}
#endif
