/*
* Klangstrom
*
* This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

#include "KlangstromEnvironment.h"

#if defined(KLST_ARCH_IS_STM32)
#define KLST_CONSOLE_LINE_ENDING "\r\n" // used in `Console.h`
#elif defined(KLST_ARCH_IS_EMU)
#define KLST_CONSOLE_LINE_ENDING "\n\r" // used in `Console.h` // TODO why is this reversed in CLI terminal
#endif

#define KLST_SDCARD_SLOW_INITIALIZATION // used in `SCCard_BSP_STM32.cpp`
//#define KLST_DISABLE_SDCARD_PRINT_CLIENT // used in `SCCard_BSP_STM32.cpp`
//#define KLST_DISABLE_PRINT_CLIENT        // used in `Console.h`
//#define KLST_PRINT_ASCII_IMAGE_AT_STARTUP // used in `Console.cpp`
#define KLST_USE_ARM_REGISTERS_FOR_CYCLE_COUNTER // used in `System_STM32.cpp`
//#define KLST_TOUCH_CONFIGURE_TOUCH_AS_NORMAL_GPIO // used in `Touch_BSP_STM32.cpp`
//#define KLST_DISABLE_REGISTER_DEFAULT_CALLBACKS // used in `MIDI.cpp`
#define KLST_DELAY_SERIAL_SEND_IF_BUSY_USEC 400 // used in `SerialDevice_STM32.cpp`
#define KLST_DEFAULT_NUMBER_OF_KEYS 2           // used in `Key.h`
#define KLST_PANDA_STM32_NUM_LEDS 2
// #define KLST_ENABLE_FIRE_EVENT_FOR_ALL_KEYS
#ifndef KLST_ADC_BUFFER_SIZE
#define KLST_ADC_BUFFER_SIZE 128
#endif
#ifndef KLST_AUDIO_CODEC_AUDIO_BUFFER_SIZE
#define KLST_AUDIO_CODEC_AUDIO_BUFFER_SIZE 128
#endif

#if __has_include("KlangstromConfigurationCustom.h")
#include "KlangstromConfigurationCustom.h"
#endif

// TODO add the other defines that are used in codebase
