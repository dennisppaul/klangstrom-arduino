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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_STM32
#include "PeripheralConfiguration_STM32.h"
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE

#include "WM8904.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WM8904_TIMEOUT HAL_MAX_DELAY
//#define WM8904_TIMEOUT     1000

static I2C_HandleTypeDef* hi2c = nullptr;

uint8_t WM8904_write_register(uint8_t register_address, uint16_t data) {
    if (!hi2c) {
        console_error("WM8904 not initialized! (I2C not setup)");
        return HAL_ERROR;
    }
    uint8_t transmit_buffer[3];
    transmit_buffer[0] = register_address;
    transmit_buffer[1] = (data & 0xff00) >> 8;
    transmit_buffer[2] = data & 0xff;

    HAL_StatusTypeDef status    = HAL_ERROR;
    uint8_t           mAttempts = 8;
    while (status == HAL_ERROR && mAttempts > 0) {
        mAttempts--;
        status = HAL_I2C_Master_Transmit(hi2c, WM8904_I2C_ADDRESS, transmit_buffer, 3, WM8904_TIMEOUT);
        if (status != HAL_OK) {
            console_status("WM8904: attempt: %i", mAttempts);
        }
    }
    if (status != HAL_OK) {
        console_error("WM8904: transmit I2C ERROR(W)");
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint16_t WM8904_read_register(uint8_t register_address) {
    if (!hi2c) {
        console_error("WM8904 not initialized!");
        return HAL_ERROR;
    }
    uint8_t transmit_buffer[1];
    transmit_buffer[0]    = register_address;
    HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(hi2c, WM8904_I2C_ADDRESS, transmit_buffer, 1, WM8904_TIMEOUT);
    if (ret != HAL_OK) {
        console_error("WM8904: transmit I2C ERROR(R0)");
        return HAL_ERROR;
    }

    uint8_t receive_buffer[2];
    ret = HAL_I2C_Master_Receive(hi2c, WM8904_I2C_ADDRESS, receive_buffer, 2, WM8904_TIMEOUT);
    if (ret != HAL_OK) {
        console_error("WM8904: receive I2C ERROR(R1)");
        return HAL_ERROR;
    }
    return (((uint16_t) receive_buffer[0] << 8) & 0xff00) | receive_buffer[1];
}

uint32_t WM8904_init(I2C_HandleTypeDef* hi2c_handle) {
    hi2c = hi2c_handle;

    if (!hi2c) {
        console_error("WM8904 not initialized!");
        return HAL_ERROR;
    }

    WM8904_write_register(WM8904_SW_RESET_AND_ID, 0xFFFF);
    uint16_t data = WM8904_read_register(WM8904_SW_RESET_AND_ID);
    HAL_Delay(10);
    if (data != 0x8904) {
        console_error("WM8904 not found! (ID respone to OK)");
        return 1;
    } else {
        console_status("audiocodec: found WM8904");
    }
    return HAL_OK;
}

uint8_t WM8904_toggle_flag(uint8_t register_address, uint16_t flag_bit) {
    uint16_t register_value = WM8904_read_register(register_address);
    register_value ^= (1 << flag_bit);
    return WM8904_write_register(register_address, register_value);
}

uint8_t WM8904_set_flag(uint8_t register_address, uint16_t flag_bit, uint8_t flag_state) {
    uint16_t register_value = WM8904_read_register(register_address);
    if (flag_state) {
        register_value |= (1 << flag_bit); // set to 1
    } else {
        register_value &= ~(1 << flag_bit); // set to 0
    }
    return WM8904_write_register(register_address, register_value);
}

#ifdef __cplusplus
}
#endif
#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE
#endif // KLST_ARCH_IS_STM32
