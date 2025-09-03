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
#ifdef KLST_ARCH_IS_STM32

#include "main.h"
#include "SerialDevice.h"
#include "SerialDevice_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

static void delay_us(const uint32_t us) {
    // Enable the DWT counter if not already enabled
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;                     // Reset the cycle counter
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Enable the cycle counter
    }

    const uint32_t startTick  = DWT->CYCCNT;
    const uint32_t delayTicks = us * (HAL_RCC_GetHCLKFreq() / 1000000);

    while ((DWT->CYCCNT - startTick) < delayTicks) {
        // Wait until the required delay is achieved
    }
}

void serialdevice_send(const SerialDevice* serialdevice, const uint8_t* data, const uint16_t length) {
    serialdevice->peripherals->is_transmitting = true;

    for (uint16_t i = 0; i < length; ++i) {
        serialdevice->peripherals->buffer_tx[i] = data[i];
    }

// TODO this is a dirty hack, but otherwise data tx and rx is corrupted. check why it is necessary to wait here
#ifdef KLST_DELAY_SERIAL_SEND_IF_BUSY_USEC
    uint8_t attempts = 2;
    while (serialdevice->peripherals->uart_handle->gState != HAL_UART_STATE_READY && attempts != 0) {
        delay_us(KLST_DELAY_SERIAL_SEND_IF_BUSY_USEC);
        --attempts;
    }
#endif // KLST_DELAY_SERIAL_SEND_IF_BUSY_USEC

    //    while (serialdevice->peripherals->uart_handle->hdmatx->State != HAL_DMA_STATE_READY && attempts != 0) {
    //    while (serialdevice->peripherals->uart_handle->gState != HAL_UART_STATE_READY && attempts != 0) {
    //        delay_us(500);
    //    }

    //    while (serialdevice->peripherals->dma_handle_tx->State != HAL_DMA_STATE_READY) {
    //        // some other code and work here...
    //        //if timeout is reached (exhausted), poll for transfer would still return dma ready!
    //        HAL_DMA_PollForTransfer(serialdevice->peripherals->dma_handle_tx, HAL_DMA_FULL_TRANSFER, 100);
    //        //cnDMAPollForTransferTimeoutMs);// 10);//timeout in ms
    //    }
    //    if (serialdevice->peripherals->dma_handle_tx->State == HAL_DMA_STATE_READY) {
    //        serialdevice->peripherals->uart_handle->gState = HAL_UART_STATE_READY;
    //    }

    HAL_UART_Transmit_DMA(serialdevice->peripherals->uart_handle,
                          serialdevice->peripherals->buffer_tx,
                          length);
}

void serialdevice_deinit_BSP(SerialDevice* serialdevice) {
    delete[] serialdevice->data;
    if (serialdevice->peripherals != nullptr) {
        delete serialdevice->peripherals;
        serialdevice->peripherals = nullptr;
    }
}

void serialdevice_deinit_peripherals_BSP(SerialDevice* serialdevice) {
    delete serialdevice->peripherals;
    serialdevice->peripherals = nullptr;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG || KLST_PERIPHERAL_ENABLE_IDC_SERIAL || KLST_PERIPHERAL_ENABLE_MIDI