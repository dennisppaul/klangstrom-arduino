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

#include "Klangstrom.h"
#if defined(KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG) || \
    defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) ||   \
    defined(KLST_PERIPHERAL_ENABLE_MIDI)
#ifdef KLST_ARCH_IS_STM32

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SerialPeripherals {
    UART_HandleTypeDef* uart_handle     = nullptr;
    DMA_HandleTypeDef*  dma_handle_rx   = nullptr;
    DMA_HandleTypeDef*  dma_handle_tx   = nullptr;
    uint8_t*            buffer_rx       = nullptr;
    uint8_t*            buffer_tx       = nullptr;
    uint16_t            buffer_size     = 32; // TODO maybe make this configurable
    volatile bool       is_transmitting = false;
} SerialPeripherals;

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG || KLST_PERIPHERAL_ENABLE_IDC_SERIAL || KLST_PERIPHERAL_ENABLE_MIDI