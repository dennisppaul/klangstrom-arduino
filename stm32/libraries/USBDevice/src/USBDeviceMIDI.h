#ifndef _USB_DEVICE_MIDI_H_
#define _USB_DEVICE_MIDI_H_

#ifndef TINYUSB_MIDI_DEVICE
#error "@USB Device :: select `Tools > USB support > USB Device MIDI` from menu"
#endif

#include "MIDI_interface.h"

void usb_device_midi_init();
void usb_device_midi_process();
uint8_t usb_device_midi_state();


// /* --- callback functions --- */

// #define WEAK __attribute__ ((weak))
// WEAK void receive_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
// WEAK void receive_midi_note_off(const uint8_t channel, const uint8_t note);
// WEAK void receive_midi_pitch_bend(const uint8_t channel, const int16_t value);
// WEAK void receive_midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value);
// WEAK void receive_midi_message(const uint8_t command, const uint8_t channel, const uint8_t value_a, const uint8_t value_b);

#endif // _USB_DEVICE_MIDI_H_