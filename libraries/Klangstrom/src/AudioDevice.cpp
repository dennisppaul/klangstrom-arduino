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
#include "AudioDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

WEAK void audioblock(AudioBlock* audio_block) {
    for (int i = 0; i < audio_block->block_size; ++i) {
        for (int j = 0; j < audio_block->output_channels; ++j) {
            audio_block->output[j][i] = 0.0;
        }
    }
}

AudioDevice* audiodevice_create_from_audioinfo(AudioInfo* audioinfo) {
    const auto audiodevice                  = new AudioDevice();
    audiodevice->audioinfo                  = new AudioInfo();
    audiodevice->audioinfo->sample_rate     = audioinfo->sample_rate;
    audiodevice->audioinfo->output_channels = audioinfo->output_channels;
    audiodevice->audioinfo->input_channels  = audioinfo->input_channels;
    audiodevice->audioinfo->block_size      = audioinfo->block_size;
    audiodevice->audioinfo->bit_depth       = audioinfo->bit_depth;
    audiodevice->audioinfo->device_type     = audioinfo->device_type;
    audiodevice->audioinfo->device_id       = AUDIO_DEVICE_ID_UNDEFINED;

    audiodevice->audioblock                  = new AudioBlock();
    audiodevice->audioblock->sample_rate     = audiodevice->audioinfo->sample_rate;
    audiodevice->audioblock->output_channels = audiodevice->audioinfo->output_channels;
    audiodevice->audioblock->input_channels  = audiodevice->audioinfo->input_channels;
    audiodevice->audioblock->block_size      = audiodevice->audioinfo->block_size;
    audiodevice->audioblock->output          = new float*[audiodevice->audioblock->output_channels];
    audiodevice->audioblock->input           = new float*[audiodevice->audioblock->input_channels];
    for (int i = 0; i < audiodevice->audioblock->output_channels; ++i) {
        audiodevice->audioblock->output[i] = new float[audiodevice->audioblock->block_size];
    }
    for (int i = 0; i < audiodevice->audioblock->input_channels; ++i) {
        audiodevice->audioblock->input[i] = new float[audiodevice->audioblock->block_size];
    }

    return audiodevice;
}

static void audiodevice_init_custom_device(AudioDevice* audiodevice) {
    (void) audiodevice;
    // TODO this is a placeholder for custom device initialization
    // 1. assume that the device is already initialized ( see `audiodevice_init_audiocodec()` )
    //     - audioinfo is set
    //     - audioblock is set ( including input and output buffers )
    //     - audioperipherals is set ( including callbacks )
    // 2. set the device ID
    // 3. set application callback ( e.g `audioblock` )
}

static bool fCleanUpMemory = false;

AudioDevice* audiodevice_init_audiocodec(AudioInfo* audioinfo) {
    fCleanUpMemory = true;

    AudioDevice* audiodevice = audiodevice_create_from_audioinfo(audioinfo);
    audiodevice_init_peripherals_BSP(audiodevice);

    /* initialize audio device */
    audiodevice_init_device_BSP(audiodevice);
    audiodevice_setup_device(audiodevice);

    return audiodevice;
}

void audiodevice_setup_device(AudioDevice* audiodevice) {
    audiodevice->audioinfo->device_id = system_get_unique_device_ID();
    audiodevice->callback_audioblock  = audioblock;
    system_register_audiodevice(audiodevice);
}

void audiodevice_deinit(AudioDevice* audiodevice) {
    if (fCleanUpMemory) {
        audiodevice_deinit_BSP(audiodevice);
        audiodevice_deinit_peripherals_BSP(audiodevice);
        for (int i = 0; i < audiodevice->audioblock->output_channels; ++i) {
            delete[] audiodevice->audioblock->output[i];
        }
        delete[] audiodevice->audioblock->output;
        for (int i = 0; i < audiodevice->audioblock->input_channels; ++i) {
            delete[] audiodevice->audioblock->input[i];
        }
        delete[] audiodevice->audioblock->input;
        delete audiodevice->audioblock;
        delete audiodevice->audioinfo;
        delete audiodevice;
    }
}

void process_audioblock_data_16_2_2(AudioDevice* audiodevice,
                                    uint16_t*    input,
                                    uint16_t*    output) {
    if (audiodevice->audioinfo->bit_depth != 16 ||
        audiodevice->audioinfo->output_channels != 2 ||
        audiodevice->audioinfo->input_channels != 2) {
        return;
    }
    AudioBlock&    audioblock = *audiodevice->audioblock;
    const uint16_t block_size = audiodevice->audioinfo->block_size;

    /* unpack receive buffer to (float) samples */
    constexpr float M_INT_SCALE = 1 << 15;
    for (uint16_t i = 0; i < block_size; i++) {
        const int16_t mLeftInt  = static_cast<int16_t>(input[2 * i]);
        const int16_t mRightInt = static_cast<int16_t>(input[2 * i + 1]);
        audioblock.input[0][i]  = static_cast<float>(mLeftInt) / M_INT_SCALE;
        audioblock.input[1][i]  = static_cast<float>(mRightInt) / M_INT_SCALE;
    }

    if (audiodevice->callback_audioblock != nullptr) {
        audiodevice->callback_audioblock(audiodevice->audioblock);
    }

    /* pack samples for transmit buffer */
    for (uint16_t i = 0; i < block_size; i++) {
        int16_t mLeftInt  = static_cast<int16_t>(audioblock.output[0][i] * M_INT_SCALE);
        int16_t mRightInt = static_cast<int16_t>(audioblock.output[1][i] * M_INT_SCALE);
        output[2 * i]     = static_cast<uint16_t>(mLeftInt);
        output[2 * i + 1] = static_cast<uint16_t>(mRightInt);
    }
}

//static void process_audioblock_data_16_2_2(AudioDevice* audiodevice,
//                                           uint32_t*    input,
//                                           uint32_t*    output) {
//    if (audiodevice->audioinfo->bit_depth != 16 ||
//        audiodevice->audioinfo->output_channels != 2 ||
//        audiodevice->audioinfo->input_channels != 2) {
//        return;
//    }
//
//    AudioBlock&    audioblock  = *audiodevice->audioblock;
//    const uint32_t sample_rate = audiodevice->audioinfo->sample_rate;
//    const uint16_t block_size  = audiodevice->audioinfo->block_size;
//
//    /* unpack receive buffer to (float) samples */
//    constexpr float    M_INT_SCALE  = 1 << 15;
//    constexpr uint32_t M_MASK_LEFT  = 0x0000FFFF;
//    constexpr uint32_t M_MASK_RIGHT = 0xFFFF0000;
//    for (uint16_t i = 0; i < block_size; i++) {
//        const uint32_t p         = input[i];
//        const int16_t  mLeftInt  = static_cast<int16_t>(p & M_MASK_LEFT);
//        const int16_t  mRightInt = static_cast<int16_t>((p & M_MASK_RIGHT) >> 16);
//        audioblock.input[0][i]   = static_cast<float>(mLeftInt) / M_INT_SCALE;
//        audioblock.input[1][i]   = static_cast<float>(mRightInt) / M_INT_SCALE;
//    }
//
//    if (audiodevice->callback_audioblock != nullptr) {
//        audiodevice->callback_audioblock(audiodevice->audioblock);
//    }
//
//    /* pack samples for transmit buffer */
//    for (uint16_t i = 0; i < block_size; i++) {
//        int16_t mLeftInt  = static_cast<int16_t>(audioblock.output[0][i] * M_INT_SCALE);
//        int16_t mRightInt = static_cast<int16_t>(audioblock.output[1][i] * M_INT_SCALE);
//        output[i]         = static_cast<uint32_t>(static_cast<uint16_t>(mLeftInt)) | (static_cast<uint32_t>(static_cast<uint16_t>(mRightInt)) << 16);
//    }
//}

#ifdef __cplusplus
}
#endif
