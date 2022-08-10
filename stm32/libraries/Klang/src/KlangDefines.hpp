/*
 * Klang – a node+text-based synthesizer library
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

#ifndef KlangDefines_hpp
#define KlangDefines_hpp

/* --- DEBUG --- */

#ifndef KLANG_DEBUG_LEVEL
#define KLANG_DEBUG_LEVEL 2
#endif

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
    printf(KLANG_NL);
#define KLANG_LOG(...)
#define KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL == 2)
#define KLANG_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(KLANG_NL);
#define KLANG_LOG(...)   \
    printf(__VA_ARGS__); \
    printf(KLANG_NL);
#define KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL >= 3)
#define KLANG_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(KLANG_NL);
#define KLANG_LOG(...)   \
    printf(__VA_ARGS__); \
    printf(KLANG_NL);
#define KLANG_LOG_SIM(...) \
    printf(__VA_ARGS__);   \
    printf(KLANG_NL);
#endif

/* --- CONSTANTS --- */

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define OSC_DEFAULT_FREQUENCY 220.0
#define OSC_DEFAULT_AMPLITUDE 0.75
#define SIGNAL_TYPE_FLOAT     1
#define SIGNAL_TYPE_INT16     2

#ifndef KLST_ARCH_MCU
#define KLST_ARCH_MCU 1
#endif
#ifndef KLST_ARCH_CPU
#define KLST_ARCH_CPU 2
#endif

/* --- TOOLS --- */

#define PPQN              24
#define FULL_NOTE(x)      ((x) % (PPQN * 4) == 0)
#define HALF_NOTE(x)      ((x) % (PPQN * 2) == 0)
#define QUARTER_NOTE(x)   ((x) % (PPQN) == 0)
#define EIGTH_NOTE(x)     ((x) % (PPQN / 2) == 0)
#define SIXTEENTH_NOTE(x) ((x) % (PPQN / 4) == 0)
#define THIRTYTWO_NOTE(x) ((x) % (PPQN / 8) == 0)

/* --- TYPEDEFS --- */

#include "KlangTypes.hpp"

namespace klang {
    enum KLANG_EVENT : uint8_t {
        RESERVED_00,
        RESERVED_01,
        TRIGGER,
    };
}

#define KLANG_DEFINES

#endif /* KlangDefines_hpp */
