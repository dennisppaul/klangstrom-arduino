#ifndef _USBDEVICE_H_
#define _USBDEVICE_H_

#ifndef USE_USBDEVICE
#error "+++ ERROR no USB Device type selected! select from `Tools > USB support (if available) > USB Device …` */"
#else

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_device.h"
#include "usbd_conf.h"
#include "usbd_core.h"
#include "usbd_ctlreq.h"
#include "usbd_def.h"
#include "usbd_desc.h"
#include "usbd_ioreq.h"

void usb_device_init();

uint8_t transmit_keyboard_key_pressed(uint8_t pKey);
uint8_t transmit_keyboard_key_released(uint8_t pKey);
void    transmit_mouse_moved(const int8_t x, const int8_t y);
void    transmit_mouse_button(const uint8_t button);

/* --- MIDI_INTERFACE_H_ --- */

typedef void (*CallbackType0)(void);
typedef void (*CallbackType1_UI8)(const uint8_t);
typedef void (*CallbackType1_UI16)(const uint16_t);
typedef void (*CallbackType2_UI8)(const uint8_t, const uint8_t);
typedef void (*CallbackType2_UI8_I16)(const uint8_t, const int16_t);
typedef void (*CallbackType2_UI8_VOIDPTR)(const uint8_t, const void*);
typedef void (*CallbackType3_UI8)(const uint8_t, const uint8_t, const uint8_t);
typedef void (*CallbackType3_UI8_I8_I8)(const uint8_t, const int8_t, const int8_t);
typedef void (*CallbackType3_BOOLPTR_I8_I8)(const bool*, const int8_t, const int8_t);

void usb_device_callback_event_receive(CallbackType2_UI8_VOIDPTR callback);

void usb_device_callback_midi_message(CallbackType3_UI8 callback);
void usb_device_callback_midi_note_off(CallbackType3_UI8 callback);
void usb_device_callback_midi_note_on(CallbackType3_UI8 callback);
void usb_device_callback_midi_aftertouch(CallbackType3_UI8 callback);
void usb_device_callback_midi_control_change(CallbackType3_UI8 callback);
void usb_device_callback_midi_program_change(CallbackType2_UI8 callback);
void usb_device_callback_midi_channel_pressure(CallbackType2_UI8 callback);
void usb_device_callback_midi_pitch_bend(CallbackType2_UI8_I16 callback);
void usb_device_callback_midi_sys_ex(CallbackType0 callback);
void usb_device_callback_midi_mtc_quarter_frame(CallbackType1_UI8 callback);
void usb_device_callback_midi_song_position_pointer(CallbackType1_UI16 callback);
void usb_device_callback_midi_song_select(CallbackType1_UI8 callback);
void usb_device_callback_midi_tune_req(CallbackType0 callback);
void usb_device_callback_midi_end_ex(CallbackType0 callback);
void usb_device_callback_midi_clock_tick(CallbackType0 callback);
void usb_device_callback_midi_clock_start(CallbackType0 callback);
void usb_device_callback_midi_clock_continue(CallbackType0 callback);
void usb_device_callback_midi_clock_stop(CallbackType0 callback);
void usb_device_callback_midi_rt_sense(CallbackType0 callback);
void usb_device_callback_midi_rt_reset(CallbackType0 callback);

/* --- transmit --- */

// TODO move to class
void usb_device_transmit_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_device_transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void usb_device_transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void usb_device_transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value);
void usb_device_transmit_midi_program_change(const uint8_t channel, const uint8_t program);
void usb_device_transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void usb_device_transmit_midi_pitch_bend(const uint8_t channel, const int16_t value);
void usb_device_transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);
void usb_device_transmit_midi_message(const uint8_t data1, const uint8_t data2, const uint8_t data3);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class _USBDevice {
public:
    void init() {
        usb_device_init();
    }

    void register_event_receive(CallbackType2_UI8_VOIDPTR callback) {
        usb_device_callback_event_receive(callback);
    }

    /* --- MIDI OUT ---  */

    // TODO add other messages

    void transmit_midi_message(const uint8_t data1, const uint8_t data2, const uint8_t data3) {
        usb_device_transmit_midi_message(data1, data2, data3);
    }

    void transmit_midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
        usb_device_transmit_midi_note_off(channel, note, velocity);
    }

    void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
        usb_device_transmit_midi_note_on(channel, note, velocity);
    }

    void transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value) {
        usb_device_transmit_midi_control_change(channel, command, value);
    }

    /* --- MIDI IN ---  */

    void register_midi_note_off(CallbackType3_UI8 callback) {
        usb_device_callback_midi_note_off(callback);
    }

    void register_midi_note_on(CallbackType3_UI8 callback) {
        usb_device_callback_midi_note_on(callback);
    }

    void register_midi_aftertouch(CallbackType3_UI8 callback) {
        usb_device_callback_midi_aftertouch(callback);
    }

    void register_midi_control_change(CallbackType3_UI8 callback) {
        usb_device_callback_midi_control_change(callback);
    }

    void register_midi_program_change(CallbackType2_UI8 callback) {
        usb_device_callback_midi_program_change(callback);
    }

    void register_midi_channel_pressure(CallbackType2_UI8 callback) {
        usb_device_callback_midi_channel_pressure(callback);
    }

    void register_midi_pitch_bend(CallbackType2_UI8_I16 callback) {
        usb_device_callback_midi_pitch_bend(callback);
    }

    void register_midi_sys_ex(CallbackType0 callback) {
        usb_device_callback_midi_sys_ex(callback);
    }

    void register_midi_mtc_quarter_frame(CallbackType1_UI8 callback) {
        usb_device_callback_midi_mtc_quarter_frame(callback);
    }

    void register_midi_song_position_pointer(CallbackType1_UI16 callback) {
        usb_device_callback_midi_song_position_pointer(callback);
    }

    void register_midi_song_select(CallbackType1_UI8 callback) {
        usb_device_callback_midi_song_select(callback);
    }

    void register_midi_tune_req(CallbackType0 callback) {
        usb_device_callback_midi_tune_req(callback);
    }

    void register_midi_end_ex(CallbackType0 callback) {
        usb_device_callback_midi_end_ex(callback);
    }

    void register_midi_clock_tick(CallbackType0 callback) {
        usb_device_callback_midi_clock_tick(callback);
    }

    void register_midi_clock_start(CallbackType0 callback) {
        usb_device_callback_midi_clock_start(callback);
    }

    void register_midi_clock_continue(CallbackType0 callback) {
        usb_device_callback_midi_clock_continue(callback);
    }

    void register_midi_clock_stop(CallbackType0 callback) {
        usb_device_callback_midi_clock_stop(callback);
    }

    void register_midi_rt_sense(CallbackType0 callback) {
        usb_device_callback_midi_rt_sense(callback);
    }

    void register_midi_rt_reset(CallbackType0 callback) {
        usb_device_callback_midi_rt_reset(callback);
    }
};

_USBDevice USBDevice;

#endif

#endif  // USE_USBDEVICE
#endif  /* _USBDEVICE_H_ */
