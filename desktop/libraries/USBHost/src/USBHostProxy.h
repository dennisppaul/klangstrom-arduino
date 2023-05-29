/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#ifndef _USBHOST_PROXY_H_
#define _USBHOST_PROXY_H_

#ifndef USE_USBHOST
#ifdef __cplusplus

#include "stdint.h"

void usb_host_transmit_midi_message(const uint8_t data1, const uint8_t data2, const uint8_t data3);
void usb_host_transmit_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void usb_host_transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value);
void usb_host_transmit_midi_program_change(const uint8_t channel, const uint8_t program);
void usb_host_transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void usb_host_transmit_midi_pitch_bend(const uint8_t channel, const int16_t value);
void usb_host_transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);

#endif  // __cplusplus
#endif  // USE_USBHOST
#endif  // _USBHOST_PROXY_H_