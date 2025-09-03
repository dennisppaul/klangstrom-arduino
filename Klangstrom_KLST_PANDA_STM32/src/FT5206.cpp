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

#include "FT5206.h"
#include "Console.h"

#include <Display.h>

#ifdef __cplusplus
extern "C" {
#endif

static I2C_HandleTypeDef* hi2c = nullptr;

// TODO what about calibration?!?

#define FT5206_TIMEOUT 1000
//                          in ms?
//                          (default)HAL_MAX_DELAY

static uint16_t clamp(const uint16_t value, const int16_t max) {
    return std::min(static_cast<uint16_t>(max), value);
}

static uint16_t word(const uint8_t highByte, const uint8_t lowByte) {
    return static_cast<uint16_t>(highByte << 8) | lowByte;
}

void FT5206_init(I2C_HandleTypeDef* hi2c_handle) {
    hi2c = hi2c_handle;

    if (!hi2c) {
        return;
    }

    //    for (int i = 0; i < 0xFF; ++i) {
    //        if (HAL_I2C_IsDeviceReady(hi2c, i, 3, 1000) != HAL_OK) {
    //            console_println("FT5206: DEVICE NOT READY: 0x%02X", i);
    //        } else {
    //            console_println("FT5206: DEVICE READY    : 0x%02X", i);
    //        }
    //    }

    //    /*
    //	 DEVICE READY: 0x34 // WM8904
    //	 DEVICE READY: 0x35
    //	 DEVICE READY: 0x70 // FT5206
    //	 DEVICE READY: 0x71
    //	 */

    uint8_t                 buf[2] = {FT5206_DEVICE_MODE, 0x00};
    const HAL_StatusTypeDef ret    = HAL_I2C_Master_Transmit(hi2c, FT5206_I2C_ADDRESS, buf, 2, FT5206_TIMEOUT);
    if (ret != HAL_OK) {
        console_println("FT5206: transmit I2C ERROR");
    }
}

bool FT5206_read(TouchEvent* touchEvent) {
    // TODO check if this IC always transmits info when events occur or if some kind of polling is required or possible
    if (!hi2c) {
        return false;
    }
    if (touchEvent == nullptr) {
        return false;
    }

    uint8_t buf[FT5206_NUMBER_OF_REGISTERS];

    const HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(hi2c, FT5206_I2C_ADDRESS, buf, FT5206_NUMBER_OF_REGISTERS, FT5206_TIMEOUT);
    if (ret != HAL_OK) {
        console_error("FT5206: receive I2C ERROR: %i", ret);
        return false;
    }

    const uint8_t number_of_touches = std::min(5, buf[FT5206_TD_STATUS] & 0xF);
    const uint8_t gesture_id        = buf[FT5206_GEST_ID];
    touchEvent->number_of_touches   = number_of_touches;
    touchEvent->gesture_id          = gesture_id;
    for (uint8_t i = 0; i < touchEvent->number_of_touches; i++) {
        const uint16_t x = word(buf[FT5206_TOUCH_XH + i * 6] & 0x0f, buf[FT5206_TOUCH_XL + i * 6]);
        const uint16_t y = word(buf[FT5206_TOUCH_YH + i * 6] & 0x0f, buf[FT5206_TOUCH_YL + i * 6]);
        touchEvent->x[i] = x > display_get_width() ? -1 : x;
        touchEvent->y[i] = y > display_get_height() ? -1 : y;
    }
    return true;
}

void FT5206_print_info() {
    if (!hi2c) {
        return;
    }

    uint8_t buf[FT5206_NUMBER_OF_TOTAL_REGISTERS];

    const HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(hi2c, FT5206_I2C_ADDRESS, buf, FT5206_NUMBER_OF_TOTAL_REGISTERS, FT5206_TIMEOUT);
    if (ret != HAL_OK) {
        console_error("FT5206: receive I2C ERROR");
        return;
    }

    console_println("Library version: %i.%i", buf[FS5206_TOUCH_LIB_VERSION_H], buf[FS5206_TOUCH_LIB_VERSION_L]);
}

#ifdef __cplusplus
}
#endif