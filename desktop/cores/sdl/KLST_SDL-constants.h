/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#ifndef _KLST_SDL_CONSTANTS_
#define _KLST_SDL_CONSTANTS_

#include <stdint.h>

/* BOARD SIMULATED */
#define KLANG_BOARD_EMULATOR_KLST_SHEEP 2
#define KLANG_BOARD_EMULATOR_KLST_TINY  1
#define KLANG_BOARD_EMULATOR_KLST_CORE  0

#ifndef KLANG_BOARD_EMULATOR
#warning KLANG_BOARD_EMULATOR not defined
#define KLANG_BOARD_EMULATOR KLANG_BOARD_EMULATOR_KLST_SHEEP
#endif

#if KLANG_BOARD_EMULATOR == KLANG_BOARD_EMULATOR_KLST_TINY
#endif
#if KLANG_BOARD_EMULATOR == KLANG_BOARD_EMULATOR_KLST_CORE
#endif
#if KLANG_BOARD_EMULATOR == KLANG_BOARD_EMULATOR_KLST_SHEEP
#endif

// @todo(clean this up )

/* PIN LABELS */

#define GPIO_00 0x00
#define GPIO_01 0x01
#define GPIO_02 0x02
#define GPIO_03 0x03
#define GPIO_04 0x04
#define GPIO_05 0x05
#define GPIO_06 0x06
#define GPIO_07 0x07
#define GPIO_08 0x08
#define GPIO_09 0x09

#define ADC_00 0x0A
#define ADC_01 0x0B
#define DAC_00 0x0C
#define DAC_01 0x0D

#define LED_00 0x0E
#define LED_01 0x0F
#define LED_02 0x10
#define LED_03 0x11
#define LED_04 0x12
#define LED_05 0x13
#define LED_06 0x14
#define LED_07 0x15
#define LED_08 0x16
#define LED_09 0x17
#define LED_10 0x18
#define LED_11 0x19
#define LED_12 0x1A
#define LED_13 0x1B
#define LED_14 0x1C
#define LED_15 0x1D

#define ENCODER_00_BUTTON 0x11
#define ENCODER_01_BUTTON 0x12
#define ENCODER_02_BUTTON 0x13
#define BUTTON_PROGRAMMER 0x14

#if KLANG_BOARD_EMULATOR == KLANG_BOARD_EMULATOR_KLST_SHEEP
#define NUMBER_OF_GPIO_PINS 0x15
#define NUMBER_OF_LEDS      0x10
#define NUMBER_OF_ENCODERS  0x02
#else
#define NUMBER_OF_GPIO_PINS 0x15
#define NUMBER_OF_LEDS      0x03
#define NUMBER_OF_ENCODERS  0x03
#endif

/* --- */

#define LED_BUILTIN            LED_00
#define KLST_NUM_ADCS          2
#define KLST_NUM_DACS          2
#define KLST_ANALOG_RESOLUTION 16
#define KLST_UART_BAUD         115200
#define KLST_NUM_LEDS          NUMBER_OF_LEDS

#define KLST_LOG Serial

#define INPUT        0x18
#define OUTPUT       0x19
#define INPUT_PULLUP 0x1A

#endif /* _KLST_SDL_CONSTANTS_ */
