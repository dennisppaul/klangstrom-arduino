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

#ifndef KlangstromDefinesArduino_h
#define KlangstromDefinesArduino_h

#include <stdint.h>

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

/* compatibility */

#define uint32 uint32_t  // this is required to compile e.g the library `TFT_22_IL9225`

/* buttons */

#define KLST_BUTTON_ENCODER_00 0x00 // same as KlangstromEvents::ENCODER_00
#define KLST_BUTTON_ENCODER_01 0x01 // same as KlangstromEvents::ENCODER_01
#define KLST_BUTTON_ENCODER_02 0x02 // same as KlangstromEvents::ENCODER_02
#define KLST_BUTTON_PROGRAMMER 0x03

/* options */

// enum KLST_OPTION { AUDIO_INPUT, ENCODERS, SERIAL_PORTS, BEAT}; // @todo(consider replacing defines with enum)

#define KLST_OPTION_AUDIO_INPUT             0x03
#define KLST_MIC                            0x00
#define KLST_LINE_IN                        0x01
#define KLST_OPTION_ENCODERS                0x04
#define KLST_DISABLE                        0x00
#define KLST_ENABLE                         0x01
#define KLST_OPTION_SERIAL_PORTS            0x05
#define KLST_OPTION_BEAT                    0x06
#define KLST_OPTION_PROGRAMMER_BUTTON       0x07
#define KLST_OPTION_ENABLE_AUDIO_INPUT      0x08
#define KLST_OPTION_HEADPHONE_OUTPUT_VOLUME 0x09
#define KLST_OPTION_SERIAL_00_BAUD_RATE     0x0A
#define KLST_OPTION_SERIAL_01_BAUD_RATE     0x0B
#define KLST_OPTION_SERIAL_02_BAUD_RATE     0x0C

/* UIDs */

#define KLST_NUM_OF_U_ID (1 + 10 + 5 + 10)
static const int16_t  KLST_NO_ID                     = -1;
static const uint32_t KLST_U_ID[KLST_NUM_OF_U_ID][3] = {
    {0x00000000, 0x00000000, 0x00000000},  // NO BOARD
    {0x0040001D, 0x0200C000, 0x445243EC},  // KLST_TINY_01
    {0x003D0010, 0x0200C000, 0x445243EC},  // KLST_TINY_02
    {0x0040000E, 0x0200C000, 0x445243EC},  // KLST_TINY_03
    {0x00000000, 0x00000000, 0x00000000},  // KLST_TINY_04*
    {0x003F000D, 0x0200C000, 0x445243EC},  // KLST_TINY_05
    {0x00000000, 0x00000000, 0x00000000},  // KLST_TINY_06*
    {0x007F6999, 0x02762964, 0x44C7AD50},  // KLST_TINY_07
    {0x0025003E, 0x0200C000, 0x445243EC},  // KLST_TINY_08
    {0x0040001C, 0x0200C000, 0x445243EC},  // KLST_TINY_09
    {0x003E000A, 0x0200C000, 0x445243EC},  // KLST_TINY_10
    {0x00190036, 0xFFFFFFFF, 0x0000306A},  // KLST_CORE_11
    {0x001C0030, 0xFFFFFFFF, 0x00003051},  // KLST_CORE_12
    {0x00190031, 0xFFFFFFFF, 0x00002FC2},  // KLST_CORE_13
    {0x0019002C, 0xFFFFFFFF, 0x00002F9A},  // KLST_CORE_14
    {0x001A0025, 0xFFFFFFFF, 0x0000302E},  // KLST_CORE_15
    {0x001B003E, 0xFFFFFFFF, 0x00003037},  // KLST_SHEEP_16
    {0x00470024, 0xFFFFFFFF, 0x00003029},  // KLST_SHEEP_17
    {0x001A003F, 0xFFFFFFFF, 0x00002F81},  // KLST_SHEEP_18
    {0x00300047, 0xFFFFFFFF, 0x0000302A},  // KLST_SHEEP_19
    {0x0046001D, 0xFFFFFFFF, 0x00003093},  // KLST_SHEEP_20
    {0x00300045, 0xFFFFFFFF, 0x00003013},  // KLST_SHEEP_21 (90°+NO_USBMICROB)
    {0x002E0028, 0xFFFFFFFF, 0x00003057},  // KLST_SHEEP_22 (90°)
    {0x002E0022, 0xFFFFFFFF, 0x00002FE8},  // KLST_SHEEP_23
    {0x001A003D, 0xFFFFFFFF, 0x00003031},  // KLST_SHEEP_24
    {0x00300040, 0xFFFFFFFF, 0x00003073},  // KLST_SHEEP_25 (BUTTONS)
};

/* Klanstrong Defines ( redefined for C context ) */

#ifndef LED_OFF
#define LED_OFF 0
#endif
#ifndef LED_ON
#define LED_ON 1
#endif
#ifndef LED_TOGGLE
#define LED_TOGGLE 2
#endif

/* DEBUGGING */

#define DEBUG_DEFINE(x) STR(x)
#define STR(x)          #x

#ifndef serial_debug
#define serial_debug Serial
#endif

/* ERROR CODES */

static const uint8_t KLST_ERROR_CODE_NO_ERROR   = 0x00;
static const uint8_t KLST_ERROR_CODE_I2S_DMA_TX = 0x01;
static const uint8_t KLST_ERROR_CODE_I2S_DMA_RX = 0x02;
static const uint8_t KLST_ERROR_CODE_I2C_WRITE  = 0x03;

#endif /* KlangstromDefinesArduino_h */
