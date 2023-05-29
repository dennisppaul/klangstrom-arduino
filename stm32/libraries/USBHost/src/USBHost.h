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

#ifndef __USBHOST__H__
#define __USBHOST__H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_USBHOST
#include "Arduino.h"
#include "HID_application.h"
#include "MIDI_application.h"
#include "USBHost_BSP_inc.h"
#include "usb_host.h"
#include "usbh_conf.h"
#else
#include <stdint.h>
#include <stdbool.h>
#include "USBHostProxy.h"
#endif
#include "USBHostCallbacks.h"
#include "KlangstromEvents.h"

void usb_host_init();
void usb_host_process();
bool usb_host_is_mouse_connected();     // see 'USBH_HID_InterfaceInit' + 'USBH_HID_InterfaceDeInit' in 'usbh_hid.c'
bool usb_host_is_keyboard_connected();  // see 'USBH_HID_InterfaceInit' + 'USBH_HID_InterfaceDeInit' in 'usbh_hid.c'
bool usb_host_is_midi_connected();      // see 'USBH_MIDI_InterfaceInit' + 'USBH_MIDI_InterfaceDeInit' in 'usbh_MIDI.c'

typedef void (*CallbackType0)(void);
typedef void (*CallbackType1_UI8)(const uint8_t);
typedef void (*CallbackType1_UI16)(const uint16_t);
typedef void (*CallbackType2_UI8)(const uint8_t, const uint8_t);
typedef void (*CallbackType2_UI8_I16)(const uint8_t, const int16_t);
typedef void (*CallbackType2_UI8_VOIDPTR)(const uint8_t, const void*);
typedef void (*CallbackType3_UI8)(const uint8_t, const uint8_t, const uint8_t);
typedef void (*CallbackType3_UI8_I8_I8)(const uint8_t, const int8_t, const int8_t);
typedef void (*CallbackType3_BOOLPTR_I8_I8)(const bool*, const int8_t, const int8_t);

void usb_host_callback_event_receive(CallbackType2_UI8_VOIDPTR callback);

void usb_host_callback_key_pressed(CallbackType1_UI8 callback);
void usb_host_callback_key_released(CallbackType1_UI8 callback);

void usb_host_callback_mouse_pressed(CallbackType3_UI8_I8_I8 callback);
void usb_host_callback_mouse_released(CallbackType3_UI8_I8_I8 callback);
void usb_host_callback_mouse_moved(CallbackType3_BOOLPTR_I8_I8 callback);

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

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

extern void mouse_pressed(EVENT_DATA button, int8_t x, int8_t y);
extern void mouse_released(EVENT_DATA button, int8_t x, int8_t y);
extern void mouse_moved(const bool* buttons, int8_t x, int8_t y);
extern void key_pressed(uint8_t key);
extern void key_released(uint8_t key);

extern void midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity);
extern void midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);

extern void midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
extern void midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value);
extern void midi_program_change(const uint8_t channel, const uint8_t program);
extern void midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
extern void midi_pitch_bend(const uint8_t channel, const int16_t value);
extern void midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);

extern void midi_clock_tick();
extern void midi_clock_start();
extern void midi_clock_continue();
extern void midi_clock_stop();

extern void midi_rt_sense();
extern void midi_rt_reset();

class _USBHost {
public:
    void init() {
        usb_host_init();
    }

    void process() {
        usb_host_process();
    }

    bool is_mouse_connected() {
        return usb_host_is_mouse_connected();
    }

    bool is_keyboard_connected() {
        return usb_host_is_keyboard_connected();
    }

    bool is_midi_connected() {
        return usb_host_is_midi_connected();
    }

    void register_keyboard_defaults() {
        register_key_pressed(key_pressed);
        register_key_released(key_released);
    }

    void register_mouse_defaults() {
        register_mouse_pressed(mouse_pressed);
        register_mouse_released(mouse_released);
        register_mouse_moved(mouse_moved);
    }

    void register_midi_defaults() {
        register_midi_note_off(midi_note_off);
        register_midi_note_on(midi_note_on);
        register_midi_clock_tick(midi_clock_tick);
        register_midi_clock_start(midi_clock_start);
        register_midi_clock_continue(midi_clock_continue);
        register_midi_clock_stop(midi_clock_stop);
    }

    void register_event_receive(CallbackType2_UI8_VOIDPTR callback) {
        usb_host_callback_event_receive(callback);
    }

    /* --- KEYBOARD --- */

    void register_key_pressed(CallbackType1_UI8 callback) {
        usb_host_callback_key_pressed(callback);
    }

    void register_key_released(CallbackType1_UI8 callback) {
        usb_host_callback_key_released(callback);
    }

    /* --- MOUSE --- */

    void register_mouse_pressed(CallbackType3_UI8_I8_I8 callback) {
        usb_host_callback_mouse_pressed(callback);
    }

    void register_mouse_released(CallbackType3_UI8_I8_I8 callback) {
        usb_host_callback_mouse_released(callback);
    }

    void register_mouse_moved(CallbackType3_BOOLPTR_I8_I8 callback) {
        usb_host_callback_mouse_moved(callback);
    }

    /* --- MIDI IN --- */

    void register_midi_note_off(CallbackType3_UI8 callback) {
        usb_host_callback_midi_note_off(callback);
    }

    void register_midi_note_on(CallbackType3_UI8 callback) {
        usb_host_callback_midi_note_on(callback);
    }

    void register_midi_aftertouch(CallbackType3_UI8 callback) {
        usb_host_callback_midi_aftertouch(callback);
    }

    void register_midi_control_change(CallbackType3_UI8 callback) {
        usb_host_callback_midi_control_change(callback);
    }

    void register_midi_program_change(CallbackType2_UI8 callback) {
        usb_host_callback_midi_program_change(callback);
    }

    void register_midi_channel_pressure(CallbackType2_UI8 callback) {
        usb_host_callback_midi_channel_pressure(callback);
    }

    void register_midi_pitch_bend(CallbackType2_UI8_I16 callback) {
        usb_host_callback_midi_pitch_bend(callback);
    }

    void register_midi_sys_ex(CallbackType0 callback) {
        usb_host_callback_midi_sys_ex(callback);
    }

    void register_midi_mtc_quarter_frame(CallbackType1_UI8 callback) {
        usb_host_callback_midi_mtc_quarter_frame(callback);
    }

    void register_midi_song_position_pointer(CallbackType1_UI16 callback) {
        usb_host_callback_midi_song_position_pointer(callback);
    }

    void register_midi_song_select(CallbackType1_UI8 callback) {
        usb_host_callback_midi_song_select(callback);
    }

    void register_midi_tune_req(CallbackType0 callback) {
        usb_host_callback_midi_tune_req(callback);
    }

    void register_midi_end_ex(CallbackType0 callback) {
        usb_host_callback_midi_end_ex(callback);
    }

    void register_midi_clock_tick(CallbackType0 callback) {
        usb_host_callback_midi_clock_tick(callback);
    }

    void register_midi_clock_start(CallbackType0 callback) {
        usb_host_callback_midi_clock_start(callback);
    }

    void register_midi_clock_continue(CallbackType0 callback) {
        usb_host_callback_midi_clock_continue(callback);
    }

    void register_midi_clock_stop(CallbackType0 callback) {
        usb_host_callback_midi_clock_stop(callback);
    }

    void register_midi_rt_sense(CallbackType0 callback) {
        usb_host_callback_midi_rt_sense(callback);
    }

    void register_midi_rt_reset(CallbackType0 callback) {
        usb_host_callback_midi_rt_reset(callback);
    }

    /* --- MIDI OUT --- */

    void transmit_midi_message(const uint8_t data1, const uint8_t data2, const uint8_t data3) {
        usb_host_transmit_midi_message(data1, data2, data3);
    }

    void transmit_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
        usb_host_transmit_midi_note_off(channel, note, velocity);
    }

    void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
        usb_host_transmit_midi_note_on(channel, note, velocity);
    }

    void transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure) {
        usb_host_transmit_midi_aftertouch(channel, note, pressure);
    }

    void transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value) {
        usb_host_transmit_midi_control_change(channel, command, value);
    }

    void transmit_midi_program_change(const uint8_t channel, const uint8_t program) {
        usb_host_transmit_midi_program_change(channel, program);
    }

    void transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure) {
        usb_host_transmit_midi_channel_pressure(channel, pressure);
    }

    void transmit_midi_pitch_bend(const uint8_t channel, const int16_t value) {
        usb_host_transmit_midi_pitch_bend(channel, value);
    }

    void transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2) {
        usb_host_transmit_midi_sys_ex(type, data1, data2);
    }

    void transmit_midi_clock_tick() {
        transmit_midi_sys_ex(MIDI_CLOCK_TICK, 0, 0);
    }

    void transmit_midi_clock_start() {
        transmit_midi_sys_ex(MIDI_CLOCK_START, 0, 0);
    }

    void transmit_midi_clock_continue() {
        transmit_midi_sys_ex(MIDI_CLOCK_CONTINUE, 0, 0);
    }

    void transmit_midi_clock_stop() {
        transmit_midi_sys_ex(MIDI_CLOCK_STOP, 0, 0);
    }
};

_USBHost USBHost;

#endif

#endif /* __USBHOST__H__ */
