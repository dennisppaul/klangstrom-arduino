/*
* Klangstrom
*
* This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

#include "SerialDevice.h"
#include "KlangstromMIDIParser.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Callback_1_SERIALDEVICEPTR)(SerialDevice*);

WEAK void midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity);
WEAK void midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity);

WEAK void             midi_event(SerialDevice* serial_device);
KlangstromMIDIParser* midi_parser();

bool midi_init();
void midi_deinit();
void midi_send(const uint8_t* data, uint16_t length);

#ifdef __cplusplus
}
#endif