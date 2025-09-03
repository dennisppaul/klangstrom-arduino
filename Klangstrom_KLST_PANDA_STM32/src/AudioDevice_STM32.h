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

#include "PeripheralConfiguration_STM32.h" // TODO change this to KLST_STM32 aka 'Architecture Specific' (ASP)
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE
#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_STM32

#ifdef STM32H723xx
#include "stm32h7xx_hal.h"
#else
#error "STM32 device not supported or defined"
#endif

#include "AudioDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief AudioDevicePeripherals struct for an SAI-or I2S-based AudioDevice with I2C interface
 */
typedef struct AudioDevicePeripherals {
    Callback_2_AUDIODEVICE_UI8 callback_rx;
    Callback_2_AUDIODEVICE_UI8 callback_tx;
    Callback_2_AUDIODEVICE_UI8 callback_error;
#ifdef HAL_SAI_MODULE_ENABLED
    SAI_HandleTypeDef* audiodevice_sai_rx;
    SAI_HandleTypeDef* audiodevice_sai_tx;
#endif // HAL_SAI_MODULE_ENABLED
#ifdef HAL_I2S_MODULE_ENABLED
    I2S_HandleTypeDef* audiocodec_i2s_rx;
    I2S_HandleTypeDef* audiocodec_i2s_tx;
#endif // HAL_I2S_MODULE_ENABLED
#ifdef HAL_I2C_MODULE_ENABLED
    I2C_HandleTypeDef* audiodevice_config;
#endif // HAL_I2C_MODULE_ENABLED
} AudioDevicePeripherals;

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE