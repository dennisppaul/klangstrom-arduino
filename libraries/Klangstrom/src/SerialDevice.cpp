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

#include "System.h"
#include "SerialDevice.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

WEAK void serial_event(SerialDevice* serial_device) {
    (void) serial_device;
}

SerialDevice* serialdevice_create(uint8_t device_type, uint32_t buffer_size, uint32_t baud_rate) {
    auto* serialdevice             = new SerialDevice();
    serialdevice->data_buffer_size = buffer_size;
    serialdevice->device_type      = device_type;
    serialdevice->device_id        = system_get_unique_device_ID();
    if (baud_rate == SERIAL_DEVICE_DEFAULT_BAUD) {
        // TODO this is a bit tricky because the baud rate is hardwired into in the MX_UARTX_Init() function
        console_error("SerialDevice: default baud rates are currently not supported");
        // TODO implement something like this ``change_baud_rate(serialdevice, 9600);``:
        //        HAL_UART_Abort_IT(serialdevice->peripherals->uart_handle);
        //        HAL_UART_DeInit(serialdevice->peripherals->uart_handle);
        //        huart1.Init.BaudRate = 9600;
        //        if (HAL_UART_Init(serialdevice->peripherals->uart_handle) != HAL_OK) {
        //            Error_Handler();
        //        }
        //        start_receive(serialdevice);
    }
    serialdevice->baud_rate       = baud_rate;
    serialdevice->callback_serial = serial_event;
    serialdevice->data            = new uint8_t[serialdevice->data_buffer_size];
    serialdevice_init_peripherals_BSP(serialdevice);
    serialdevice_init_BSP(serialdevice);
    system_register_serialdevice(serialdevice);
    return serialdevice;
}

void serialdevice_delete(SerialDevice* serialdevice) {
    serialdevice_deinit_peripherals_BSP(serialdevice);
    serialdevice_deinit_BSP(serialdevice);
    delete serialdevice;
}

#ifdef __cplusplus
}
#endif
