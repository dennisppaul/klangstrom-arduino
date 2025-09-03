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

#include <cstdio>
#include <cstdarg>

#include "System.h"
#include "Console.h"
#include "KlangstromEnvironment.h"

#ifdef __cplusplus
extern "C" {
#endif

static bool fSerialDebugInitialized = false;
static bool fIsMuted                = false;

//#ifndef CONSOLE_PRINT_BUFFER_SIZE
//#define CONSOLE_PRINT_BUFFER_SIZE 128
//#endif // CONSOLE_PRINT_BUFFER_SIZE

void console_mute(bool mute) {
    fIsMuted = mute;
}

void console_init() {
    if (!fSerialDebugInitialized) {
        fSerialDebugInitialized = true;
        console_init_BSP();
    }
}

void console_set_color_red() {
    console_printf("\033[31m");
}

void console_set_color_green() {
    console_printf("\033[32m");
}

void console_reset_color() {
    console_printf("\033[0m");
}

void console_clear() {
    console_printf("\033[2J\033[H");
}

#ifndef KLST_DISABLE_PRINT_CLIENT
void _console_printf_(const char* format, ...) {
    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    //    char    buffer[CONSOLE_PRINT_BUFFER_SIZE];
    //    va_list args;
    //    va_start(args, format);
    //    vsnprintf(buffer, CONSOLE_PRINT_BUFFER_SIZE, format, args);
    //    va_end(args);
    //    KLST_BSP_serialdebug_printf("%s", buffer);
}

void _console_println_(const char* format, ...) {
    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(KLST_CONSOLE_LINE_ENDING);
    fflush(stdout);

    //    char    buffer[CONSOLE_PRINT_BUFFER_SIZE];
    //    va_list args;
    //    va_start(args, format);
    //    vsnprintf(buffer, CONSOLE_PRINT_BUFFER_SIZE, format, args);
    //    va_end(args);
    //    KLST_BSP_serialdebug_printf("%s\n\r", buffer);
}

void _console_print_(const char* format, ...) {
    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);

    //    char    buffer[CONSOLE_PRINT_BUFFER_SIZE];
    //    va_list args;
    //    va_start(args, format);
    //    vsnprintf(buffer, CONSOLE_PRINT_BUFFER_SIZE, format, args);
    //    va_end(args);
    //    KLST_BSP_serialdebug_printf("%s\n\r", buffer);
}
#endif // KLST_DISABLE_PRINT_CLIENT

#ifndef KLST_DISABLE_PRINT_DEBUG
void _console_status_(const char* format, ...) {
    console_set_color_green();
    console_timestamp(false);

    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(KLST_CONSOLE_LINE_ENDING);
    fflush(stdout);

    console_reset_color();
}

void _console_error_(const char* format, ...) {
    console_set_color_red();
    console_timestamp(false);

    if (fIsMuted) {
        return;
    }
    console_init();
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(KLST_CONSOLE_LINE_ENDING);
    fflush(stdout);

    console_reset_color();
}
#endif // KLST_DISABLE_PRINT_DEBUG

void console_system_info() {
    console_reset_color();

    console_status("----------------------------------------------------------------------------------------------------");
#if defined(KLST_PANDA_STM32) || defined(KLST_PANDA_EMU)
    const char* mBoardName = "KLST_PANDA";
#elif defined(KLST_CATERPILLAR_STM32) || defined(KLST_CATERPILLAR_EMU)
    const char* mBoardName = "KLST_CATERPILLAR";
#endif // KLST_PANDA_STM32 || KLST_CATERPILLAR_STM32
    console_status("%s(STM32H723ZGT) @ %liMHz (%s)", mBoardName, system_clock_frequency() / 1000000, __TIME__);
    console_status("----------------------------------------------------------------------------------------------------");
#ifdef KLST_PRINT_ASCII_IMAGE_AT_STARTUP
#if defined(KLST_PANDA_STM32) || defined(KLST_PANDA_EMU)
    console_status("                                                                                                    ");
    console_status("                        ▓▓████▓▓▒▒                                                                  ");
    console_status("                      ░░██████████                                                                  ");
    console_status("                      ██████████▓▓░░                                                                ");
    console_status("                      ████████████░░                                                                ");
    console_status("                      ████████████▒▒                                ▓▓▓▓██                          ");
    console_status("                      ██████████▓▓░░                              ▓▓██▓▓████                        ");
    console_status("                      ████████▓▓░░                            ░░████████████▒▒                      ");
    console_status("                      ▓▓████▒▒░░░░                              ▓▓████████████                      ");
    console_status("                      ▒▒██▒▒                  ░░░░  ░░            ████████████                      ");
    console_status("                        ░░░░              ░░░░    ░░░░░░░░░░░░░░  ░░██████████                      ");
    console_status("                        ░░        ░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓████████                      ");
    console_status("                                ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████▓▓                      ");
    console_status("                                ░░░░████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████                        ");
    console_status("                              ░░░░██████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░                            ");
    console_status("                        ░░░░░░░░▒▒████▓▓████▒▒░░░░░░░░▒▒████░░░░░░░░░░░░                            ");
    console_status("                      ░░░░░░░░░░▓▓████▓▓██▓▓░░  ░░░░░░██████▓▓░░░░░░░░░░                            ");
    console_status("                      ░░░░░░░░░░████████░░          ░░████████░░░░░░░░░░                            ");
    console_status("                      ░░░░░░░░░░████▓▓                ██▓▓████░░░░░░░░░░░░                          ");
    console_status("                      ░░░░░░░░░░████                    ████▓▓░░░░░░░░░░                            ");
    console_status("                      ░░░░░░░░░░░░░░                    ▒▒██▒▒░░░░░░░░░░                            ");
    console_status("                      ░░░░░░░░░░░░        ░░██████▒▒      ██░░░░░░░░░░░░                            ");
    console_status("                      ░░░░░░░░░░░░░░  ░░░░▒▒▓▓████████░░░░▓▓░░░░░░░░░░                              ");
    console_status("                    ░░██░░░░░░░░░░░░░░░░░░░░██████████░░░░░░░░░░░░░░░░                              ");
    console_status("                  ░░████▓▓░░░░░░▒▒░░░░░░░░░░▒▒██████▒▒░░░░░░░░░░░░░░                                ");
    console_status("                  ▓▓██████▓▓▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░                              ");
    console_status("              ▒▒▓▓██████████████▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒████░░                            ");
    console_status("            ▓▓██████████████████████▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░▒▒▓▓▓▓██████████▓▓▒▒                        ");
    console_status("          ░░▓▓████████████████████████████████████▓▓▒▒▒▒████████████████████                        ");
    console_status("          ██▓▓████████████████████████████████████████████████████████████████                      ");
    console_status("        ████▓▓████████████████████████████████████████████████████████████████▒▒                    ");
    console_status("        ▓▓▓▓████████████████████████████████████████████████████████████████████                    ");
    console_status("      ▓▓████████████████▓▓░░░░░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██████████████████████▓▓                  ");
    console_status("      ██████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒██████████████████                  ");
    console_status("      ▓▓████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒██████████████████░░                ");
    console_status("    ▒▒▓▓████████████████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒██████████████████▓▓                ");
    console_status("    ████████████████████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒████████████████████                ");
    console_status("    ▓▓▓▓██████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒██████████████████▓▓                ");
    console_status("    ▓▓████████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▓▓██████████████████▓▓                ");
    console_status("    ████████████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒██████████████████████                ");
    console_status("    ████████████████████████░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒██████████████████████                ");
    console_status("      ████████████████████▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒██████████████████████                ");
    console_status("      ████████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░██████████████████████████████▒▒        ");
    console_status("      ██████████████████▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓████████████████████████████████▒▒    ");
    console_status("    ▒▒████████████████████▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓████████████████████████████████▓▓▓▓  ");
    console_status("    ████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒████▓▓██████████████████████████████░░");
    console_status("  ▒▒████████████████████████▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓██████████████████████████████████████");
    console_status("  ████████████████████████████▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒████████████████████████████████████████");
    console_status("  ████████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒████████████████████████████████▓▓██████");
    console_status("  ████████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒██████████████████████████████████▓▓████");
    console_status("  ████████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒████████████████████████████▓▓▓▓▓▓▓▓██▓▓");
    console_status("░░▓▓██████████████████████████▒▒▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒██████████████████████████▓▓▓▓▓▓▓▓▓▓██  ");
    console_status("  ████████████████▓▓▓▓████████▒▒▒▒▒▒▒▒░░░░░░░░░░░░▒▒▒▒░░      ▒▒██████████████████████▓▓████████▓▓  ");
    console_status("  ████████████████▓▓██████████  ░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░            ████████████████████▓▓████████▒▒    ");
    console_status("  ██████████████████████████▓▓                                    ██████████████████▓▓▓▓▓▓██▓▓      ");
    console_status("    ████████████████▓▓██████                                        ██████████████████████          ");
    console_status("    ██████████████████████▒▒                                                                        %s", KLST_CONSOLE_LINE_ENDING);
#elif defined(KLST_CATERPILLAR_STM32) || defined(KLST_CATERPILLAR_EMU)
    console_status("                                                                                                                                                        ");
    console_status("                                                                                                                                                        ");
    console_status("                                                                                                    ░▓░▒░░░░██░░▒▒░░                                    ");
    console_status("                                                                                                ░░░░▓█▓░░▒▓▓██▒░░░░▓█░                                  ");
    console_status("                                                                                             ░██▒░░░▒██░░░░░▓██░░░░▓█▒░░▒░                              ");
    console_status("                                                                                           ░░░▓██▒░▒▒██▒░░░░▓██░░░░██▓░░░▓░░                            ");
    console_status("                                                                                         █░░▒▓▒▒██▓░░███░░░░███░░░░██▒ ░ ░░▓▓                           ");
    console_status("                                                                                       ▒░██░░  ░███░░░██▒░▒▓▓██░░░░██▒░ ░░▓█▒░░░                        ");
    console_status("                                                                                    ░░██░░██▓░░░░██▒░░███░░░░██░░░▓██░ ░▒▒██░░██▒░                      ");
    console_status("                                                                                 ░▓░░▓▓██░░███░░▓▓███░░▓██░░░░█░░░██▓░ ░░▒█▓ ▒██░░░░                    ");
    console_status("                                                                            ▒░░░█▓▒█░░░▒██░░███░░░▓███▓░██░░░▒█░░▒██▒░░ ░██░░██▒ ░  ▒░                  ");
    console_status("                                                                  ░▓░░░░██▓█░░▓▓██░█▓░░░▓██░░███▒░ ░███▒░█▓░░░█▒░▓██▓░░▒██▓ ▓▓░░  ░▒▓▒                  ");
    console_status("                                                         ░░░░███░░██░▓▓▓██▓▓█░░ ███░█▓░░▒▓██▓░▓██▓░ ░███░██▓░██▓░████████▓ ░▒  ░ ▓▓▓▒░                  ");
    console_status("                                                      ▒█░░▓▓▓███░░██░░░░███░█▒░░ ███░██░▓▓████░▓███░▒░▒█░████▓▓▓▒███▓░▒▓▓▒▒░░░████▓░▒                   ");
    console_status("                                             ░░░░███░░██▓░░ ░███▒░▓██░▒░▓██░██░░ ░██▓░██░░░▒███▒█▓██▓▓░░▓▓██░ ░▓▓█▒░ ░░▓▓▓▓▓▓▓▓▓░░░▓░                   ");
    console_status("                                         ░▓██░▒▓▒████░▒██░░░ ░███░░██░░░░██▓░██░░▓▓███▒██▒░░▒░██▓█▓█░▒▓▓█▓██░ ▓█▓▓▒░░▓▓███░░▒▓▓█▓▓▓░               ░▒▒▒▓");
    console_status("                                   ░░███░░▓██░   ░███░░██▒░  ░███░░███░░▓███░░███▓▓████▓███░░░ ▓█▓█▓█░░▓▓▓▓▓▒▓▓▓▓▓▓█▓███▓▓██████▓░          ░░▒▒▓▓▓▓▓▓▓▓");
    console_status("                               ░█ ░░░▒███░░██▓░  ░███░░███░░░▓███▓░░███▓▓█████▓██░░░▓███▒▓▓▓▓▓▓▓█▓▓███░░██▓████▓▓███▓▓▓▒▒░░░░▒▓░      ░░▒▒▒▓▓▓▓▓▓▓▓███▓▓");
    console_status("                             ░░██▒░░ ░███░░███░ ░▓▓██▒░░████▓▓▓████░▓███░█████████░░░░░██▓█▓▓░░░▓▓▓██████▓▓██▒▓▓█▒░▓▓█▒░▓▓▓▓▒░ ░▒░░▒▓▓▒▓▓▓▓▒▓▓█▓▓▓▓▓▓▓▓▒");
    console_status("                          ▓█▒░▒███░░░░▓███░░███▓▓▓████░░▓███░░░█████░███░ ░████▒███░▒▓░░▒█▓▓█▓▓▓▒███▓▓░░▓▓░░▒▒▓▓▓█░░▒▓░▒▒▒▒▒▒▓▒▓▓▓▓▓▒▓▓█▓▓▓▓▓▓▓▒▒░░░    ");
    console_status("                        ░▓███░░▒███░▓▓▓███▒░████░░██████░███▒░░░████▒▒██░░░░░▒▓▓▓█▓▓▓▓▓▓▓▓▓▓▓███████▒ ░░░▓▓█▓░░▒▓██▓▒▒▒▒▓▓▓▓▓▓▓▓█▓▓▒▓▓▓▓▓▒▒░░           ");
    console_status("                      ░▓░ ░███░░▒███░░░█████░███░░░▒████░░███░ ░▒░▒█▓▓█▓██▓▓░░░▓▓▒▓▓▓░░░▓▓▓█▓▒▓▓░░░▓░ ▒░░▒▓██▓▒▒▒▒▓░░▒▓▓▓▓▓▓▒▒▓▓▓▒▒▒░░                  ");
    console_status("                      ░█▓░░▓████░░███░░░████░▒███░ ░░░███▓██▓▓█▓▓░░░▓▓░▓▓░░▒▓▓▓▓▓▓▓▓██████▓▓░░░▒▒▒▒▒▒▒▒▒▓▒▒▓▓▒▒▓▓▓▓▒▒▒▒▓▓▓▒▒▒░░                         ");
    console_status("                     ██▒█▓░▓██████░███░░▒░░██▓█████▓▒░░░▓▓░█▓▓░░▒▒▓▓██▓▓██████████▒░░██████▒▒▒▒▒▒▒▓▒▒▓▓▓▓▓▓▒░░▓▓▓▒▒▒░░                                  ");
    console_status("                    ░░█████ ░░▓█████████▓▓░▒█▓░███░░▒▓▓▓█▓▓▓████████████░▒██████▓▒░▓▓▒░░░▒▓▒▓▓▓▓▓▓▒▓▒▓▓▒▒▒▒▒░░░                                         ");
    console_status("                     ░░░████▒░░░░░█████░▒▒▓▓██▓▓██▓▓███▓███▓░░██████▓▒░▓███▓░░▓▓▒▒▒▒▓█▓▓▓▓▒▒▓▒▒▒▒▒░░ ░                                                  ");
    console_status("                    ░░▓█▓▒███▓▓▓▓▓▓██▓██▓▓▓█████░▓█████▓▓▒░▒██▓░░░▓▓▒▒▒░▒█░░░░▓▒▒▒▓▒▒▒▒▒▒▒░                                                             ");
    console_status("                    ░█▓▓▒░░░░▓█▒░░░███▓█████▓░░█████▒▓█▓▒▒▒▒▒█▒░░░▓▓▒▒▒▒▒▒▓▒▒▓░                                                                         ");
    console_status("                    ░▒██░░▓▓████████▒▒░░░░▒▒░░▒▒▓░░░▒██▒▒▒▒▓▒▒▒▒▒▓▒▒░░    ░▒░░                                                                          ");
    console_status("                    ░▒░██▓███████▓░░▒░▒▒▓▓▓▓▓▓▒▒▓▓▓▓█▒▒▒▒▒░▒░ ░░░░░░                                                                                    ");
    console_status("                    ░█▓▒▓▓█▒▒░░░▒▒▒▒▓▓▓▒▓▓▓▒▓▓▓▒▓▒▒▒░▒░░                                                                                                ");
    console_status("            ▒░▓▓▓█▓█▓█▓█▓░░░░░▓▓▒▒▒▒▒▒▒▒▒▓░░░░                                                                                                          ");
    console_status("  ░░▒▓▓▓█▓▓█▓████▓█▓██▓▓▒▒▒▓▒▒▒░▒▒░▒░                                                                                                                   ");
    console_status("▓▓██▓▓███▓▓▓▓█▓▓▓▓▒▓▓▒▒▒▒▒▒░░                                                                                                                           ");
    console_status("█▓██▓▓▓▓▓▒▓▒▓▒▒▒▒░░░░                                                                                                                                   ");
    console_status("▓▒▓▒▒▒▒▒░░ ░                                                                                                                                            ");
    console_status("░                                                                                                                                                       ");
    console_status("                                                                                                                                                        ");
    console_status("                                                                                                                                                        %s", KLST_CONSOLE_LINE_ENDING);
#endif
#endif
    console_status("%s", KLST_CONSOLE_LINE_ENDING);

    //    for (int i = 0; i < 7; ++i) {
    //        rainbow("■□■ □■□□ □□□ ■ □□■■□■ □■■□ □■ ■□ ■□□ □■");
    //    }
}

#ifdef __cplusplus
}
#endif
