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

#ifdef __cplusplus
extern "C" {
#endif

void transmit_midi_message(const uint8_t cable, const uint8_t data1, const uint8_t data2, const uint8_t data3);

void transmit_midi_note_off(const uint8_t channel, const uint8_t note);
void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value);
void transmit_midi_program_change(const uint8_t channel, const uint8_t program);
void transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void transmit_midi_pitch_bend(const uint8_t channel, const int16_t value);
/* note, that sys ex messages are sent via this collector function */
void transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);

#ifdef __cplusplus
}
#endif