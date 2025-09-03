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
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

// TODO maybe implement this with `SerialDevice`

#include <inttypes.h>

#include "usart.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KLST_DISABLE_PRINT_CLIENT

#ifndef CONSOLE_UART_INSTANCE
#define CONSOLE_UART_INSTANCE huart3
#endif // CONSOLE_UART_INSTANCE

#ifndef CONSOLE_UART_INSTANCE_INIT
#define CONSOLE_UART_INSTANCE_INIT MX_USART3_UART_Init
#endif // CONSOLE_UART_INSTANCE_INIT

extern UART_HandleTypeDef CONSOLE_UART_INSTANCE; // TODO remove all references to `extern` variables

//typedef struct ConsolePeripherals {
//    UART_HandleTypeDef* uart_handle;
//    void (*init)(void);
//} ConsolePeripherals;
//
//// for KLST_PANDA_STM32
//ConsolePeripherals console_peripherals = {
//    .uart_handle = &huart3,
//    .init        = MX_USART3_UART_Init
//};
//
//extern "C" int _write(int file, char* data, int len) {
//    // @note needs to be implemented with `extern "C"` for printf to work
//    (void) file;
//    HAL_StatusTypeDef status = HAL_UART_Transmit(console_peripherals.uart_handle, (uint8_t*) data, len, HAL_MAX_DELAY);
//    return (status == HAL_OK ? len : 0);
//}
//
//void _console_init_BSP(ConsolePeripherals peripherals) {
//    console_peripherals = peripherals;
//    console_peripherals.init();
//    // HAL_StatusTypeDef status = HAL_UART_Transmit(console_peripherals.uart_handle, (uint8_t*) data, len, HAL_MAX_DELAY);
//}

extern "C" int _write(int file, char* data, int len) {
    // @note needs to be implemented with `extern "C"` for printf to work
    (void) file;
    HAL_StatusTypeDef status = HAL_UART_Transmit(&CONSOLE_UART_INSTANCE, (uint8_t*) data, len, HAL_MAX_DELAY);
    return (status == HAL_OK ? len : 0);
}

void console_init_BSP() {
    // TODO init with parameters i.e callback to e.g `MX_USART3_UART_Init` and pointer to `huart3`
    CONSOLE_UART_INSTANCE_INIT();
}
#else
void console_init_BSP() {}
#endif // KLST_DISABLE_PRINT_CLIENT

static void rainbow(const char* text) {
    static const uint8_t num_colors = 7;
    const char*          colors[]   = {
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

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG