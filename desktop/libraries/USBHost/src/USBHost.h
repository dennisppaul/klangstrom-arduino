#ifndef __USBHOST__H__
#define __USBHOST__H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Arduino.h"
#include "HID_application.h"
#include "MIDI_application.h"
#include "USBHost_BSP_inc.h"
#include "usb_host.h"
#include "usbh_conf.h"

void usb_host_init();
void usb_host_process();
bool usb_host_is_mouse_connected(); // see 'USBH_HID_InterfaceInit' + 'USBH_HID_InterfaceDeInit' in 'usbh_hid.c'
bool usb_host_is_keyboard_connected(); // see 'USBH_HID_InterfaceInit' + 'USBH_HID_InterfaceDeInit' in 'usbh_hid.c'
bool usb_host_is_midi_connected(); // see 'USBH_MIDI_InterfaceInit' + 'USBH_MIDI_InterfaceDeInit' in 'usbh_MIDI.c'

void usb_host_callback_defaults();  // TODO separate into mouse, keyboard, midi

typedef void (*CallbackKeyPressed)(const uint8_t key);
void usb_host_callback_key_pressed(CallbackKeyPressed callback);
typedef void (*CallbackKeyReleased)(const uint8_t key);
void usb_host_callback_key_released(CallbackKeyReleased callback);

typedef void (*CallbackMousePressed)(const EVENT_DATA button, const int8_t x, const int8_t y);
void usb_host_callback_mouse_pressed(CallbackMousePressed callback);
typedef void (*CallbackMouseReleased)(const EVENT_DATA button, const int8_t x, const int8_t y);
void usb_host_callback_mouse_released(CallbackMouseReleased callback);
typedef void (*CallbackMouseMoved)(const bool* buttons, const int8_t x, const int8_t y);
void usb_host_callback_mouse_moved(CallbackMouseMoved callback);

typedef void (*CallbackEventReceive)(const EVENT_TYPE event, const void* data);
void usb_host_callback_event_receive(CallbackEventReceive callback);

// @TODO(add MIDI callbacks)
// typedef void (*Callback_midi_note_on)(const uint8_t channel, const uint8_t note, const uint8_t velocity);
// void usb_host_callback_receive_midi_note_on(Callback_midi_note_on callback);
// typedef void (*Callbacke_midi_note_off)(const uint8_t channel, const uint8_t note);
// void usb_host_callback_receive_midi_note_off(Callbacke_midi_note_off callback);
// typedef void (*Callback_midi_control_change)(const uint8_t channel, const uint8_t controller, const uint8_t value);
// void usb_host_callback_receive_midi_control_change(Callback_midi_control_change callback);
// typedef void (*Callback_midi_pitch_bend)(const uint8_t channel, const int16_t value);
// void usb_host_callback_receive_midi_pitch_bend(Callback_midi_pitch_bend callback);
// typedef void (*Callback_midi_sys_ex)(const uint8_t type, const uint8_t data1, const uint8_t data2);
// void usb_host_callback_receive_midi_sys_ex(Callback_midi_sys_ex callback);
// Error
// NoteOn
// NoteOff
// AfterTouchPoly
// ControlChange
// ProgramChange
// AfterTouchChannel
// PitchBend
// SystemExclusive
// TimeCodeQuarterFrame
// SongPosition
// SongSelect
// TuneRequest
// Clock
// Start
// Tick
// Continue
// Stop
// ActiveSensing
// SystemReset

#ifdef __cplusplus
}
#endif

#endif /* __USBHOST__H__ */
