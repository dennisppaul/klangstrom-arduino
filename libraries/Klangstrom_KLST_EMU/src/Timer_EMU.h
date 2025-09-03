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

#include "Klangstrom.h"
#ifdef KLST_ARCH_IS_EMU

#include <stdint.h>
#include "HardwareTimer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Callback_0_VOID)();
typedef void (*Callback_1_TIMERPTR)(Timer*);

typedef struct TimerPeripherals {
    uint16_t       timer_number;
    HardwareTimer* timer_handle;
    // Callback_1_TIMERPTR callback;
} TimerPeripherals;

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU