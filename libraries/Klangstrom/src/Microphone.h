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

#include "AudioBlock.h"
#include "AudioInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

typedef void (*Callback_1_AUDIOBLOCKPTR)(AudioBlock*);

typedef struct Microphone {
    AudioInfo*               audioinfo           = nullptr;
    AudioBlock*              audioblock          = nullptr;
    Callback_1_AUDIOBLOCKPTR callback_audioblock = nullptr;
} Microphone;

/**
 * callback to be implemented by client application
 * @param audio_block
 */
WEAK void audioblock(AudioBlock* audio_block);

Microphone* microphone_init(AudioInfo* audio_info = nullptr);
void        microphone_deinit(Microphone* microphone);
void        microphone_start(Microphone* microphone); // implementation in BSP
void        microphone_stop(Microphone* microphone);  // implementation in BSP

void microphone_init_BSP(Microphone* microphone);

#ifdef __cplusplus
}
#endif
