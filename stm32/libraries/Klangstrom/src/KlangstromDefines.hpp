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

#ifndef KlangstromDefines_hpp
#define KlangstromDefines_hpp

#include <stdint.h>

/* platform / architecture ( as defined in `KLST_ARCH` ) */

#define KLST_ARCH_MCU     1
#define KLST_ARCH_CPU     2
#define KLST_ARCH_DESKTOP 2
#define KLST_ARCH_VCV     3
#define KLST_ARCH_PLUGIN  3

//@todo(@maybe add application specific debugging flags)

/* --- USER DEFINES --- */

#define SIGNAL_TYPE_FLOAT 1
#define SIGNAL_TYPE_INT16 2

#if __has_include("KlangConfiguration.h")
#include "KlangConfiguration.h"
#endif

#ifndef KLANG_AUDIO_RATE
#define KLANG_AUDIO_RATE 48000
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_RATE to default value: 48000
#endif
#endif

#ifndef KLANG_AUDIO_BLOCKS
#define KLANG_AUDIO_BLOCKS 32
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_BLOCKS to default value: 32
#endif
#endif

#ifndef KLANG_SAMPLES_PER_AUDIO_BLOCK
#define KLANG_SAMPLES_PER_AUDIO_BLOCK 512
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_SAMPLES_PER_AUDIO_BLOCK to default value: 512
#endif
#endif

#ifndef KLANG_SIGNAL_TYPE
#define KLANG_SIGNAL_TYPE SIGNAL_TYPE_FLOAT
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_SIGNAL_TYPE to default value: SIGNAL_TYPE_FLOAT
#endif
#endif

/* --- SIGNAL --- */

#if (KLANG_SIGNAL_TYPE == SIGNAL_TYPE_INT16)
typedef int16_t SIGNAL_TYPE;
#elif (KLANG_SIGNAL_TYPE == SIGNAL_TYPE_FLOAT)
typedef float SIGNAL_TYPE;
#else
#error KLANG_SIGNAL_TYPE not defined
#endif

/* --- TYPEDEFS --- */

typedef uint8_t DATA_PERIPHERAL_TYPE;

/**
 * this block defines constants for `data_transmit` + `data_receive` operations
 * including peripherals like UART, SPI, I2C
 */
// namespace klangstrom {
static const DATA_PERIPHERAL_TYPE PERIPHERAL_RESERVED_01 = 0x00;
static const DATA_PERIPHERAL_TYPE ALL_PERIPHERALS        = 0x01;
static const DATA_PERIPHERAL_TYPE SERIAL_00              = 0x02;
static const DATA_PERIPHERAL_TYPE SERIAL_01              = 0x03;
static const DATA_PERIPHERAL_TYPE SERIAL_02              = 0x04;
// }  // namespace klangstrom

/* --- OSC_ADDRESS_PATTERNS --- */

/**
 * this block defines constants for OSC communications
 */
// namespace klangstrom {
static const char KLANG_OSC_CMD[]      = "/klangstrom/command";
static const char KLANG_OSC_DATA[]     = "/klangstrom/data";
static const char KLANG_OSC_MIDI_IN[]  = "/klangstrom/midi_in";
static const char KLANG_OSC_MIDI_OUT[] = "/klangstrom/midi_out";
static const char KLANG_OSC_SIM[]      = "/klangstrom/sim";
static const char KLANG_OSC_SERIAL[]   = "/klangstrom/serial";
// }  // namespace klangstrom

#ifndef KLANG_OSC_TRANSMIT_ADDRESS
#define KLANG_OSC_TRANSMIT_ADDRESS "224.0.0.1"
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_OSC_TRANSMIT_ADDRESS to default value: "224.0.0.1"
#endif
#endif

#ifndef KLANG_OSC_TRANSMIT_PORT
#define KLANG_OSC_TRANSMIT_PORT 7000
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_OSC_TRANSMIT_PORT to default value: 7000
#endif
#endif

#ifndef KLANG_OSC_RECEIVE_PORT
#define KLANG_OSC_RECEIVE_PORT 7001
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_OSC_RECEIVE_PORT to default value: 7001
#endif
#endif

#endif /* KlangstromDefines_hpp */
