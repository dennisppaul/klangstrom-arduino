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
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

#include <cstring>

#include "stm32h7xx_hal.h"
#include "sai.h"
#include "i2c.h"
#include "AudioDevice.h"
#include "AudioDevice_STM32.h"
#include "Console.h"
#include "WM8904.h"
#include "WM8904Configure.h"
#include "DMAMemoryAllocator.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AUDIOCODEC_RX
#define AUDIOCODEC_RX hsai_BlockA1
#endif // AUDIOCODEC_RX
#ifndef AUDIOCODEC_TX
#define AUDIOCODEC_TX hsai_BlockB1
#endif // AUDIOCODEC_TX
#ifndef AUDIOCODEC_CONFIG
#define AUDIOCODEC_CONFIG hi2c4
#endif // AUDIOCODEC_CONFIG

extern SAI_HandleTypeDef AUDIOCODEC_RX;
extern SAI_HandleTypeDef AUDIOCODEC_TX;
extern I2C_HandleTypeDef AUDIOCODEC_CONFIG;

uint16_t* dma_TX_buffer;
uint16_t* dma_RX_buffer;
uint16_t* mCurrentInputRXBuffer;
uint16_t* mCurrentOutputTXBuffer;

#define M_BIT_DEPTH 16
#define M_SAMPLE_RATE 48000
#define M_TX_OUTPUT_CHANNELS 2
#define M_RX_INPUT_CHANNELS 2
#define M_SAMPLES_PER_AUDIO_BLOCK KLST_AUDIO_CODEC_AUDIO_BUFFER_SIZE
#define M_DOUBLE_BUFFER_SIZE (M_SAMPLES_PER_AUDIO_BLOCK * 2)

static void check_audiodevice_config(AudioDevice* audiodevice) {
    if (audiodevice->audioinfo->sample_rate != M_SAMPLE_RATE) {
        console_error("@check_audiodevice_config / sample rate not supported\n");
        return;
    }
    if (audiodevice->audioinfo->output_channels != M_TX_OUTPUT_CHANNELS) {
        console_error("@check_audiodevice_config / output channels not supported\n");
        return;
    }
    if (audiodevice->audioinfo->input_channels != M_RX_INPUT_CHANNELS) {
        console_error("@check_audiodevice_config / input channels not supported\n");
        return;
    }
    if (audiodevice->audioinfo->block_size != M_SAMPLES_PER_AUDIO_BLOCK) {
        console_error("@check_audiodevice_config / block size not supported\n");
        return;
    }
    if (audiodevice->audioinfo->bit_depth != M_BIT_DEPTH) {
        console_error("@check_audiodevice_config / bit depth not supported\n");
        return;
    }
}

static void rx_input_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    if (callback_type == CALLBACK_FULL_COMPLETE) {
        mCurrentInputRXBuffer = &(dma_RX_buffer[audiodevice->audioinfo->block_size * audiodevice->audioinfo->input_channels]);
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
        mCurrentInputRXBuffer = &(dma_RX_buffer[0]);
    }
}

static void tx_output_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    if (callback_type == CALLBACK_FULL_COMPLETE) {
        mCurrentOutputTXBuffer = &(dma_TX_buffer[audiodevice->audioinfo->block_size * audiodevice->audioinfo->output_channels]);
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
        mCurrentOutputTXBuffer = &(dma_TX_buffer[0]);
    }
    if (audiodevice != nullptr) {
        process_audioblock_data_16_2_2(audiodevice,
                                       mCurrentInputRXBuffer,
                                       mCurrentOutputTXBuffer);
    }
}

static void error_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    (void) audiodevice;
    (void) callback_type;
    console_error("error in audio device");
}

static void delay_ms(uint32_t duration) {
    HAL_Delay(duration);
}

static void setup_SAI(AudioDevice* audiodevice) {
    console_status("setting up SAI");

    const uint32_t fDoubleBufferLength        = audiodevice->audioinfo->block_size * 2;
    const uint32_t fDoubleBufferLengthInBytes = fDoubleBufferLength * sizeof(uint16_t);
    memset(dma_TX_buffer, 0, fDoubleBufferLengthInBytes * audiodevice->audioinfo->output_channels);
    memset(dma_RX_buffer, 0, fDoubleBufferLengthInBytes * audiodevice->audioinfo->input_channels);
    mCurrentInputRXBuffer  = &(dma_RX_buffer[0]);
    mCurrentOutputTXBuffer = &(dma_TX_buffer[0]);

    HAL_StatusTypeDef status;
    status = HAL_SAI_Transmit_DMA(audiodevice->peripherals->audiodevice_sai_tx,
                                  (uint8_t*) dma_TX_buffer,
                                  // TODO why is length not in bytes?
                                  // fDoubleBufferLengthInBytes * audiodevice->audioinfo->output_channels);
                                  fDoubleBufferLength * audiodevice->audioinfo->output_channels);
    if (HAL_OK != status) {
        console_error("initializing SAI TX: %i", status);
    }

    status = HAL_SAI_Receive_DMA(audiodevice->peripherals->audiodevice_sai_rx,
                                 (uint8_t*) dma_RX_buffer,
                                 // TODO why is length not in bytes?
                                 // fDoubleBufferLengthInBytes * audiodevice->audioinfo->input_channels);
                                 fDoubleBufferLength * audiodevice->audioinfo->input_channels);
    if (HAL_OK != status) {
        console_error("initializing SAI RX: %i", status);
    }
}

void audiodevice_init_device_BSP(AudioDevice* audiodevice) {
    audiodevice->peripherals->callback_rx    = rx_input_callback;
    audiodevice->peripherals->callback_tx    = tx_output_callback;
    audiodevice->peripherals->callback_error = error_callback;

    check_audiodevice_config(audiodevice);

    const uint32_t fDoubleBufferLengthInBytes = audiodevice->audioinfo->block_size * 2 * sizeof(uint16_t);

    dma_TX_buffer = (uint16_t*) dma_malloc(fDoubleBufferLengthInBytes * audiodevice->audioinfo->output_channels);
    dma_RX_buffer = (uint16_t*) dma_malloc(fDoubleBufferLengthInBytes * audiodevice->audioinfo->input_channels);
    if (dma_TX_buffer == nullptr || dma_RX_buffer == nullptr) {
        console_error("@audiodevice_init_device / error allocating DMA memory\n");
    }

    MX_I2C4_Init();
    MX_SAI1_Init();
    if (WM8904_init(&AUDIOCODEC_CONFIG) != HAL_OK) {
        console_error("@audiodevice_init_device_BSP / error initializing audio codec\n");
    }
    delay_ms(100);
    WM8904_configure(true, false);
    setup_SAI(audiodevice); // TODO see why `setup_default_start_sequence` needs to be called again
    audiodevice_pause(audiodevice);
}

void audiodevice_init_peripherals_BSP(AudioDevice* audiodevice) {
    audiodevice->peripherals                     = new AudioDevicePeripherals();
    audiodevice->peripherals->audiodevice_sai_rx = &AUDIOCODEC_RX;
    audiodevice->peripherals->audiodevice_sai_tx = &AUDIOCODEC_TX;
    audiodevice->peripherals->audiodevice_config = &AUDIOCODEC_CONFIG;
}

void audiodevice_deinit_peripherals_BSP(AudioDevice* audiodevice) {
    delete audiodevice->peripherals;
}

void audiodevice_deinit_BSP(AudioDevice* audiodevice) {
    (void) audiodevice; // TODO implement
    dma_free_all();
}


#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE
