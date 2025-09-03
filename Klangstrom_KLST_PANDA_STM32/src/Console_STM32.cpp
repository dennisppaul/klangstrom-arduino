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
#ifdef KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG
#ifdef KLST_ARCH_IS_STM32

#include <inttypes.h>

#include "usart.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

void console_timestamp(bool newline) {
    if (newline) {
        console_println("[%010" PRIu32 "] ", (uint32_t) HAL_GetTick());
    } else {
        console_printf("[%010" PRIu32 "] ", (uint32_t) HAL_GetTick());
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#else

#include "Console.h"

void console_timestamp(bool newline) { (void) newline; }
void console_system_info() {}
void console_init_BSP() {}

#endif // KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG
