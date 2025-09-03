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
#include "AudioBlock.h"
#include "AudioInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

constexpr uint8_t AUDIO_DEVICE_TYPE_UNDEFINED               = 0;
constexpr uint8_t AUDIO_DEVICE_KLST_PANDA_AUDIO_CODEC       = 1;
constexpr uint8_t AUDIO_DEVICE_KLST_PANDA_STEREO_MIC        = 2;
constexpr uint8_t AUDIO_DEVICE_KLST_CATERPILLAR_AUDIO_CODEC = 3;
constexpr uint8_t AUDIO_DEVICE_KLST_CATERPILLAR_STEREO_MIC  = 4;
constexpr uint8_t AUDIO_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES   = 5;
constexpr uint8_t AUDIO_DEVICE_CUSTOM                       = 127;

constexpr uint8_t AUDIO_DEVICE_INIT_ERROR   = 254;
constexpr uint8_t AUDIO_DEVICE_ID_UNDEFINED = 255;

static constexpr uint8_t CALLBACK_TX_ERROR      = 0;
static constexpr uint8_t CALLBACK_RX_ERROR      = 1;
static constexpr uint8_t CALLBACK_FULL_COMPLETE = 2;
static constexpr uint8_t CALLBACK_HALF_COMPLETE = 3;

struct AudioDevicePeripherals; /* BSP or ASP implementation */

typedef void (*Callback_1_AUDIOBLOCKPTR)(AudioBlock*);

typedef struct AudioDevice {
    AudioInfo*               audioinfo           = nullptr;
    AudioBlock*              audioblock          = nullptr;
    AudioDevicePeripherals*  peripherals         = nullptr;
    Callback_1_AUDIOBLOCKPTR callback_audioblock = nullptr;
} AudioDevice;

typedef void (*Callback_2_AUDIODEVICE_UI8)(AudioDevice*, uint8_t);

/**
 * callback to be implemented by client application
 * @param audio_block
 */
WEAK void audioblock(AudioBlock* audio_block);

/**
 * initializes audio device with info as specified in `audioinfo`.
 * note, that `audioinfo` is copied.
 * after initialization `audioinfo.device_id` will contain the ID of the device.
 * if intialization fails, `audioinfo.device_id` will be set to `AUDIO_DEVICE_INIT_ERROR`.
 * @param audioinfo
 * @return
 */
AudioDevice* audiodevice_init_audiocodec(AudioInfo* audioinfo);
void         audiodevice_deinit(AudioDevice* audiodevice);
void         audiodevice_resume(AudioDevice* audiodevice);
void         audiodevice_pause(AudioDevice* audiodevice);
void         audiodevice_setup_device(AudioDevice* audiodevice);
AudioDevice* audiodevice_create_from_audioinfo(AudioInfo* audioinfo);

void audiodevice_init_peripherals_BSP(AudioDevice* audiodevice);
void audiodevice_deinit_peripherals_BSP(AudioDevice* audiodevice);
void audiodevice_init_device_BSP(AudioDevice* audiodevice);
void audiodevice_deinit_BSP(AudioDevice* audiodevice);

void process_audioblock_data_16_2_2(AudioDevice* audiodevice, uint16_t* input, uint16_t* output);

#ifdef __cplusplus
}
#endif
