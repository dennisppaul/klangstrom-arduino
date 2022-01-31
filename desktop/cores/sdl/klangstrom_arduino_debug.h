/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
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

#ifndef klangstrom_arduino_debug_h
#define klangstrom_arduino_debug_h

#if defined(KLANG_TARGET_MCU)
#define KLANG_NL "\n\r"
#else
#define KLANG_NL "\n"
#endif

#if (KLANG_DEBUG_LEVEL == 0)
#define KLANG_LOG_ERR(...)
#define KLANG_LOG(...)
#define KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL == 1)
#define KLANG_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(KLANG_NL);      \
    std::cout << std::flush;

#define KLANG_LOG(...)
#define KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL == 2)
#define KLANG_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(KLANG_NL);      \
    std::cout << std::flush;

#define KLANG_LOG(...)   \
    printf(__VA_ARGS__); \
    printf(KLANG_NL);    \
    std::cout << std::flush;

#define KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL >= 3)
#define KLANG_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(KLANG_NL);      \
    std::cout << std::flush;

#define KLANG_LOG(...)   \
    printf(__VA_ARGS__); \
    printf(KLANG_NL);    \
    std::cout << std::flush;

#define KLANG_LOG_SIM(...) \
    printf(__VA_ARGS__);   \
    printf(KLANG_NL);      \
    std::cout << std::flush;

#endif

#endif // klangstrom_arduino_debug_h