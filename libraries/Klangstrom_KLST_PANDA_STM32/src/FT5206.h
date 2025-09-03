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

#include <stdint.h>
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include "TouchEvent.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FT5206_I2C_ADDRESS 0x70       // or 0x71? used to be 0x38, why?
#define FT5206_NUMBER_OF_REGISTERS 31 // there are more registers, but this is enough to get all 5 touch coordinates.
#define FT5206_NUMBER_OF_TOTAL_REGISTERS 0xFE
#define FT5206_DEVICE_MODE 0x00

#define FT5206_GEST_ID 0x01
#define FT5206_GEST_ID_MOVE_UP 0x10
#define FT5206_GEST_ID_MOVE_LEFT 0x14
#define FT5206_GEST_ID_MOVE_DOWN 0x18
#define FT5206_GEST_ID_MOVE_RIGHT 0x1C
#define FT5206_GEST_ID_ZOOM_IN 0x48
#define FT5206_GEST_ID_ZOOM_OUT 0x49
#define FT5206_GEST_ID_NO_GESTURE 0x00

#define FT5206_ID_G_AUTO_CLB_MODE 0xA0
#define FT5206_ID_G_AUTO_CLB_MODE_ENABLE 0x00
#define FT5206_ID_G_AUTO_CLB_MODE_DISABLE 0xFF
#define FS5206_TOUCH_LIB_VERSION_H 0xA1
#define FS5206_TOUCH_LIB_VERSION_L 0xA2
#define FT5206_ID_G_STATE 0xA7
#define FT5206_ID_G_STATE_CONFIGURE 0x00
#define FT5206_ID_G_STATE_WORK 0x01
#define FT5206_ID_G_STATE_CALIBRATION 0x02
#define FT5206_ID_G_STATE_FACTORY 0x03
#define FT5206_ID_G_STATE_AUTO_CALIBR 0x04

#define FT5206_TD_STATUS 0x02
#define FT5206_TOUCH_XH 0x03
#define FT5206_TOUCH_XL 0x04
#define FT5206_TOUCH_YH 0x05
#define FT5206_TOUCH_YL 0x06

void FT5206_init(I2C_HandleTypeDef* hi2c_handle);
bool FT5206_read(TouchEvent* touchEvent);
void FT5206_print_info();

// TODO(add calibration and touch threshold configuration)

#ifdef __cplusplus
}
#endif