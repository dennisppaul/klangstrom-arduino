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
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* function type defintions */

typedef void (*CallbackType0)(void);
typedef void (*CallbackType1_UI8)(const uint8_t);
typedef void (*CallbackType1_UI16)(const uint16_t);
typedef void (*CallbackType2_UI8)(const uint8_t, const uint8_t);
typedef void (*CallbackType2_UI8_I16)(const uint8_t, const int16_t);
typedef void (*CallbackType2_UI8_VOIDPTR)(const uint8_t, const void*);
typedef void (*CallbackType3_UI8)(const uint8_t, const uint8_t, const uint8_t);
typedef void (*CallbackType3_UI8_I8_I8)(const uint8_t, const int8_t, const int8_t);
typedef void (*CallbackType3_BOOLPTR_I8_I8)(const bool*, const int8_t, const int8_t);

/* callback function setters */

void usb_host_callback_event_receive(CallbackType2_UI8_VOIDPTR callback);

void usb_host_callback_key_pressed(CallbackType1_UI8 callback);
void usb_host_callback_key_released(CallbackType1_UI8 callback);

void usb_host_callback_mouse_pressed(CallbackType3_UI8_I8_I8 callback);
void usb_host_callback_mouse_released(CallbackType3_UI8_I8_I8 callback);
void usb_host_callback_mouse_moved(CallbackType3_BOOLPTR_I8_I8 callback);

// maybe rename `usb_host_callback_*` to `usb_host_register_*`
// TODO maybe add `midi_message` for unimplemented messages
// void usb_host_callback_midi_message(CallbackType3_UI8 callback);
void usb_host_callback_midi_note_off(CallbackType3_UI8 callback);
void usb_host_callback_midi_note_on(CallbackType3_UI8 callback);
void usb_host_callback_midi_aftertouch(CallbackType3_UI8 callback);
void usb_host_callback_midi_control_change(CallbackType3_UI8 callback);
void usb_host_callback_midi_program_change(CallbackType2_UI8 callback);
void usb_host_callback_midi_channel_pressure(CallbackType2_UI8 callback);
void usb_host_callback_midi_pitch_bend(CallbackType2_UI8_I16 callback);
void usb_host_callback_midi_sys_ex(CallbackType0 callback);
void usb_host_callback_midi_mtc_quarter_frame(CallbackType1_UI8 callback);
void usb_host_callback_midi_song_position_pointer(CallbackType1_UI16 callback);
void usb_host_callback_midi_song_select(CallbackType1_UI8 callback);
void usb_host_callback_midi_tune_req(CallbackType0 callback);
void usb_host_callback_midi_end_ex(CallbackType0 callback);
void usb_host_callback_midi_clock_tick(CallbackType0 callback);
void usb_host_callback_midi_clock_start(CallbackType0 callback);
void usb_host_callback_midi_clock_continue(CallbackType0 callback);
void usb_host_callback_midi_clock_stop(CallbackType0 callback);
void usb_host_callback_midi_rt_sense(CallbackType0 callback);
void usb_host_callback_midi_rt_reset(CallbackType0 callback);

/* call callback functions */

void usb_host_call_event_receive(const uint8_t event, const void* data);

void usb_host_call_key_pressed(const uint8_t key);
void usb_host_call_key_released(const uint8_t key);

void usb_host_call_mouse_pressed(const uint8_t button, const int8_t x, const int8_t y);
void usb_host_call_mouse_released(const uint8_t button, const int8_t x, const int8_t y);
void usb_host_call_mouse_moved(const bool* buttons, const int8_t x, const int8_t y);
void usb_host_call_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_call_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_host_call_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void usb_host_call_midi_control_change(const uint8_t channel, const uint8_t control, const uint8_t value);
void usb_host_call_midi_program_change(const uint8_t channel, const uint8_t program);
void usb_host_call_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void usb_host_call_midi_pitch_bend(const uint8_t channel, const uint16_t pitch);
void usb_host_call_midi_sys_ex(); // TODO add message
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
