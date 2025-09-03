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

#include "Klangstrom.h"
#if defined(KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG) || \
    defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) ||   \
    defined(KLST_PERIPHERAL_ENABLE_MIDI)
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

#include "main.h"
#include "usart.h"
#include "Console.h"
#include "SerialDevice.h"
#include "DMAMemoryAllocator.h"
#include "SerialDevice_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;
extern UART_HandleTypeDef huart4;
extern DMA_HandleTypeDef  hdma_uart8_rx;
extern DMA_HandleTypeDef  hdma_uart8_tx;
extern DMA_HandleTypeDef  hdma_uart9_rx;
extern DMA_HandleTypeDef  hdma_uart9_tx;
extern DMA_HandleTypeDef  hdma_uart4_rx;
extern DMA_HandleTypeDef  hdma_uart4_tx;

static void start_receive(SerialDevice* serialdevice) {
    HAL_UARTEx_ReceiveToIdle_DMA(serialdevice->peripherals->uart_handle,
                                 serialdevice->peripherals->buffer_rx,
                                 serialdevice->data_buffer_size);
    //    __HAL_DMA_DISABLE_IT(serialdevice->peripherals->dma_handle_rx, DMA_IT_HT);
}

bool serialdevice_init_BSP(SerialDevice* serialdevice) {
    if (serialdevice->peripherals == nullptr) {
        console_error("ERROR: peripherals not initialized");
        return false;
    }
    if (serialdevice->device_type < SERIAL_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES) {
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_00) {
            console_status("device type: IDC_00");
            if (serialdevice->baud_rate != 115200) {
                console_error("SerialDevice: currently baud rate is fixed at 115200");
            }
            MX_UART8_Init();
            start_receive(serialdevice);
            return true;
        }
#if defined(KLST_PANDA_STM32)
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_01) {
            console_status("device type: IDC_01");
            if (serialdevice->baud_rate != 115200) {
                console_error("SerialDevice: currently baud rate is fixed at 115200");
            }
            MX_UART9_Init();
            start_receive(serialdevice);
            return true;
        }
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_MIDI) {
            console_status("device type: is MIDI");
            if (serialdevice->baud_rate != 31250) {
                console_error("SerialDevice: baud rate is set to 31250 to comply with MIDI standard");
            }
            MX_UART4_Init();
            start_receive(serialdevice);
            return true;
        }
#endif // KLST_PANDA_STM32
    }
    console_status("device type: custom(%i)", serialdevice->device_type);
    // NOTE client needs to initialize UART ( e.g 'MX_UARTx_Init()' )
    // and call 'start_receive(serialdevice)' manually
    return false;
}

bool serialdevice_init_peripherals_BSP(SerialDevice* serialdevice) {
    serialdevice->peripherals      = new SerialPeripherals;
    SerialPeripherals& peripherals = *serialdevice->peripherals;
    if (serialdevice->device_type < SERIAL_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES) {
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_00) {
            peripherals.uart_handle   = &huart8;
            peripherals.dma_handle_rx = &hdma_uart8_rx;
            peripherals.dma_handle_tx = &hdma_uart8_tx;
            peripherals.buffer_rx     = static_cast<uint8_t*>(dma_malloc(peripherals.buffer_size));
            peripherals.buffer_tx     = static_cast<uint8_t*>(dma_malloc(peripherals.buffer_size));
            return true;
        }
#if defined(KLST_PANDA_STM32)
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_01) {
            peripherals.uart_handle   = &huart9;
            peripherals.dma_handle_rx = &hdma_uart9_rx;
            peripherals.dma_handle_tx = &hdma_uart9_tx;
            peripherals.buffer_rx     = static_cast<uint8_t*>(dma_malloc(peripherals.buffer_size));
            peripherals.buffer_tx     = static_cast<uint8_t*>(dma_malloc(peripherals.buffer_size));
            return true;
        }
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_MIDI) {
            peripherals.uart_handle   = &huart4;
            peripherals.dma_handle_rx = &hdma_uart4_rx;
            peripherals.dma_handle_tx = &hdma_uart4_tx;
            peripherals.buffer_rx     = static_cast<uint8_t*>(dma_malloc(peripherals.buffer_size));
            peripherals.buffer_tx     = static_cast<uint8_t*>(dma_malloc(peripherals.buffer_size));
            return true;
        }
#endif // KLST_PANDA_STM32
    }
    serialdevice->peripherals = new SerialPeripherals();
    return false;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32 || KLST_CATERPILLAR_STM32
#endif // KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG || KLST_PERIPHERAL_ENABLE_IDC_SERIAL || KLST_PERIPHERAL_ENABLE_MIDI