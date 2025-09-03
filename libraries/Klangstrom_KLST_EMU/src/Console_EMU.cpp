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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#include <inttypes.h>

#include "Console.h"
#include "stm32_hal.h"

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


void console_init_BSP() {}

static void rainbow(const char* text) {
    static constexpr uint8_t num_colors = 7;
    const char*              colors[]   = {
        "\033[31m", // Red
        "\033[32m", // Green
        "\033[33m", // Yellow
        "\033[34m", // Blue (as Indigo)
        "\033[35m", // Violet
        "\033[36m", // Cyan (as another shade of Violet)
        "\033[37m"  // White (as another shade of Violet)
    };

    const char* reset_color = "\033[0m";
    static int  color_index = 0;

    console_status("%s%s%s", colors[color_index], text, reset_color);
    color_index++;
    color_index %= num_colors;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
