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

#define KLST_PERIPHERAL_ENABLE_GPIO
#define KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG
#define KLST_PERIPHERAL_ENABLE_AUDIODEVICE
#define KLST_PERIPHERAL_ENABLE_LEDS
#define KLST_PERIPHERAL_ENABLE_SD_CARD
#define KLST_PERIPHERAL_ENABLE_IDC_SERIAL
#define KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC
#define KLST_PERIPHERAL_ENABLE_TIMERS

/* KLST_PANDA_STM32 exclusive i.e not built into KLST_CATERPILLAR */

#define KLST_PERIPHERAL_ENABLE_ENCODER
#define KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS
#define KLST_PERIPHERAL_ENABLE_MIDI
#define KLST_PERIPHERAL_ENABLE_ADC_DAC
#define KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY
#define KLST_PERIPHERAL_ENABLE_DISPLAY
