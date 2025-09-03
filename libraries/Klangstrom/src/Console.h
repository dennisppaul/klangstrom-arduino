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

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "KlangstromConfiguration.h"

#ifndef KLST_CONSOLE_LINE_ENDING
#warning "KLST_CONSOLE_LINE_ENDING not defined, using default"
#define KLST_CONSOLE_LINE_ENDING "\r\n"
#endif // KLST_CONSOLE_LINE_ENDING

void console_mute(bool mute);
void console_init();
void console_set_color_red();
void console_set_color_green();
void console_reset_color();
void console_clear();
void console_timestamp(bool newline);   // NOTE implemented this ASP for now
// void console_timestamp(bool newline=false);   // NOTE implemented this ASP for now
void console_system_info();                   // NOTE implemented this BSP for now

/*
 * use `#define KLST_DISABLE_PRINT_CLIENT` to disable all printing.
 * this may safes a lot of *program storage space*.
 */
#ifndef KLST_DISABLE_PRINT_CLIENT
#define console_printf(format, ...) _console_printf_(format, ##__VA_ARGS__)
#define console_print(format, ...) _console_print_(format, ##__VA_ARGS__)
#define console_println(format, ...) _console_println_(format, ##__VA_ARGS__)
#else
#define console_printf(format, ...)
#define console_print(format, ...)
#define console_println(format, ...)
#endif
void _console_printf_(const char* format, ...);
void _console_print_(const char* format, ...);
void _console_println_(const char* format, ...);

/*
 * use `#define KLST_DISABLE_PRINT_DEBUG` to disable only error and status messages.
 * this reduces *clutter* in the serial monitor and saves *program storage space*.
 */
#ifndef KLST_DISABLE_PRINT_DEBUG
#define console_status(format, ...) _console_status_(format, ##__VA_ARGS__)
#define console_error(format, ...) _console_error_(format, ##__VA_ARGS__)
#else
#define console_status(format, ...)
#define console_error(format, ...)
#endif
void _console_status_(const char* format, ...);
void _console_error_(const char* format, ...);

void console_init_BSP();

#ifdef __cplusplus
}
#endif
