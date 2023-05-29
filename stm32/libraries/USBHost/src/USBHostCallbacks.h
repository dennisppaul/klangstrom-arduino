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

#ifndef __USBHOST_CALLBACKS__H__
#define __USBHOST_CALLBACKS__H__

#ifdef __cplusplus
extern "C" {
#endif

void usb_host_call_event_receive(const uint8_t event, const void *data);

void usb_host_call_key_pressed(const uint8_t key);
void usb_host_call_key_released(const uint8_t key);

void usb_host_call_mouse_pressed(const uint8_t button, const int8_t x, const int8_t y);
void usb_host_call_mouse_released(const uint8_t button, const int8_t x, const int8_t y);
void usb_host_call_mouse_moved(const bool *buttons, const int8_t x, const int8_t y);
void usb_host_call_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_call_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_call_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void usb_host_call_midi_control_change(const uint8_t channel, const uint8_t control, const uint8_t value);
void usb_host_call_midi_program_change(const uint8_t channel, const uint8_t program);
void usb_host_call_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void usb_host_call_midi_pitch_bend(const uint8_t channel, const uint16_t pitch);
void usb_host_call_midi_sys_ex();
void usb_host_call_midi_mtc_quarter_frame(uint8_t frame_number);
void usb_host_call_midi_song_position_pointer(uint16_t song_position);
void usb_host_call_midi_song_select(uint8_t song_number);
void usb_host_call_midi_tune_req();
void usb_host_call_midi_end_ex();
void usb_host_call_midi_clock_tick();
void usb_host_call_midi_clock_start();
void usb_host_call_midi_clock_continue();
void usb_host_call_midi_clock_stop();
void usb_host_call_midi_rt_sense();
void usb_host_call_midi_rt_reset();

#ifdef __cplusplus
}
#endif

#endif  // __USBHOST_CALLBACKS__H__