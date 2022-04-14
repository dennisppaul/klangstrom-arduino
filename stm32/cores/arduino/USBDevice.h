#ifndef _USBDEVICE_H_
#define _USBDEVICE_H_

#ifndef USE_USBDEVICE
#error "+++ ERROR no USB Device type selected! select from `Tools > USB support (if available) > USB Device …` */"
#else

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

// @todo(unify this with USB Host)
// @todo(check if it is a bug or a feature that host and device use the same callback functions)

#define MIDI_NOTE_OFF              0x80
#define MIDI_NOTE_ON               0x90
#define MIDI_AFTERTOUCH            0xA0
#define MIDI_CONTROL_CHANGE        0xB0
#define MIDI_PROGRAM_CHANGE        0xC0
#define MIDI_CHANNEL_PRESSURE      0xD0
#define MIDI_PITCHBEND             0xE0
#define MIDI_SYS_EX                0xF0

#define MIDI_SONG_POSITION_POINTER 0xF2
#define MIDI_CLOCK_TICK            0xF8
#define MIDI_CLOCK_START           0xFA
#define MIDI_CLOCK_CONTINUE        0xFB
#define MIDI_CLOCK_STOP            0xFC

void transmit_midi_note_off(const uint8_t channel, const uint8_t note);
void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value);
void transmit_midi_program_change(const uint8_t channel, const uint8_t program);
void transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void transmit_midi_pitch_bend(const uint8_t channel, const int16_t value);
void transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);
void transmit_midi_message(const uint8_t cable, const uint8_t data1, const uint8_t data2, const uint8_t data3);

/* --- callback functions --- */

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif
WEAK void receive_midi_note_off(const uint8_t channel, const uint8_t note);
WEAK void receive_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
WEAK void receive_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
WEAK void receive_midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value);
WEAK void receive_midi_program_change(const uint8_t channel, const uint8_t program);
WEAK void receive_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
WEAK void receive_midi_pitch_bend(const uint8_t channel, const int16_t value);
WEAK void receive_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);
WEAK void receive_midi_message(const uint8_t cable, const uint8_t data1, const uint8_t data2, const uint8_t data3);

#ifdef __cplusplus
}
#endif

#endif  // USE_USBDEVICE
#endif  /* _USBDEVICE_H_ */
