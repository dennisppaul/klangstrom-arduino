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

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

static constexpr uint8_t SERIAL_DEVICE_TYPE_IDC_00                = 1;
static constexpr uint8_t SERIAL_DEVICE_TYPE_IDC_01                = 2;
static constexpr uint8_t SERIAL_DEVICE_TYPE_MIDI                  = 3;
static constexpr uint8_t SERIAL_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES = 4;
static constexpr uint8_t SERIAL_DEVICE_TYPE_CUSTOM                = 127;
static constexpr uint8_t SERIAL_DEVICE_TYPE_UNDEFINED             = 255;
static constexpr uint8_t SERIAL_DEVICE_INIT_INCOMPLETE            = 254;
static constexpr uint8_t SERIAL_DEVICE_ID_UNDEFINED               = 255;
static constexpr int32_t SERIAL_DEVICE_DEFAULT_BAUD               = -1;

struct SerialPeripherals; /* BSP or ASP implementation */
struct SerialDevice;

typedef void (*Callback_1_SERIALDEVICEPTR)(SerialDevice*);

typedef struct SerialDevice {
    SerialPeripherals*         peripherals           = nullptr;
    Callback_1_SERIALDEVICEPTR callback_serial       = nullptr;
    uint8_t                    device_type           = SERIAL_DEVICE_TYPE_UNDEFINED;
    uint8_t                    device_id             = SERIAL_DEVICE_ID_UNDEFINED;
    uint32_t                   baud_rate             = SERIAL_DEVICE_DEFAULT_BAUD; // NOTE will be set at init time
    uint16_t                   data_buffer_size      = 0;
    uint8_t*                   data                  = nullptr;
    uint16_t                   length                = 0;
    Callback_1_SERIALDEVICEPTR callback_serialdevice = nullptr;
} SerialDevice;

/**
 * callback to be implemented by client application
 * @param serial_device
 */
WEAK void serial_event(SerialDevice* serial_device);

SerialDevice* serialdevice_create(uint8_t device_type, uint32_t buffer_size = 32, uint32_t baud_rate = 115200);
void          serialdevice_delete(SerialDevice* serialdevice);
void          serialdevice_send(const SerialDevice* serialdevice, const uint8_t* data, uint16_t length); // implemented as A/BSP
/**
 * initializes device on board-specific level. needs to decide if device type is supported.
 * furthermore, this function initializes the data buffer.
 * @param serialdevice
 * @return returns true if initialization was successful, if false client needs to initialize manually
 */
bool serialdevice_init_BSP(SerialDevice* serialdevice);
void serialdevice_deinit_BSP(SerialDevice* serialdevice);
/**
 * initializes peripherals on board-specific level. needs to decide if device type is supported.
 * @param serialdevice
 * @return returns true if initialization was successful, if false client needs to initialize manually
 */
bool serialdevice_init_peripherals_BSP(SerialDevice* serialdevice);
void serialdevice_deinit_peripherals_BSP(SerialDevice* serialdevice);

#ifdef __cplusplus
}
#endif