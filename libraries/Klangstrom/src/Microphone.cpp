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

#include "Microphone.h"
#include "System.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

static Microphone* microphone        = nullptr;
static bool        fCleanUpAudioInfo = false;

WEAK void audioblock(AudioBlock* audio_block) {
    (void) audio_block;
}

Microphone* microphone_init(AudioInfo* audio_info) {
    microphone = new Microphone();
    /* audio info */
    if (audio_info == nullptr) {
        console_status("Microphone: creating audio info");
        audio_info                  = new AudioInfo();
        audio_info->sample_rate     = 48000;
        audio_info->output_channels = 0;
        audio_info->input_channels  = 2;
        audio_info->block_size      = 128;
        audio_info->bit_depth       = 16; // TODO this is interesting … investigate
        fCleanUpAudioInfo           = true;
    }
    if (audio_info->sample_rate != 48000) {
        console_error("Microphone: currently only 48kHz sample rate is supported");
        audio_info->sample_rate = 48000;
    }
    if (audio_info->output_channels > 0) {
        console_error("Microphone: output channels are not supported");
        audio_info->output_channels = 0;
    }
    if (audio_info->input_channels != 2) {
        console_error("Microphone: currently only 2 input channels are supported");
        audio_info->input_channels = 2;
    }
    microphone->audioinfo            = audio_info;
    microphone->audioinfo->device_id = system_get_unique_device_ID();
    /* audio block */
    microphone->audioblock                  = new AudioBlock();
    microphone->audioblock->sample_rate     = audio_info->sample_rate;
    microphone->audioblock->output_channels = audio_info->output_channels;
    microphone->audioblock->input_channels  = audio_info->input_channels;
    microphone->audioblock->block_size      = audio_info->block_size;
    microphone->audioblock->output          = nullptr;
    microphone->audioblock->input           = new float*[microphone->audioinfo->input_channels];
    for (int i = 0; i < microphone->audioinfo->input_channels; ++i) {
        microphone->audioblock->input[i] = new float[microphone->audioinfo->block_size];
    }
    microphone->audioblock->device_id = microphone->audioinfo->device_id;
    /* default callback */
    microphone->callback_audioblock   = audioblock;
    /* init BSP */
    microphone_init_BSP(microphone);
    return microphone;
}

void microphone_deinit(Microphone* microphone) {
    if (fCleanUpAudioInfo) {
        delete microphone->audioinfo;
    }
    delete microphone->audioblock;
    for (int i = 0; i < microphone->audioinfo->input_channels; ++i) {
        delete[] microphone->audioblock->input[i];
    }
    delete[] microphone->audioblock->input;
    delete microphone;
}


#ifdef __cplusplus
}
#endif
