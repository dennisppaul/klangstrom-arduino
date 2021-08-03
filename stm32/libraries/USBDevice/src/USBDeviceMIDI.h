#ifndef _USB_DEVICE_MIDI_H_
#define _USB_DEVICE_MIDI_H_


#ifndef TINYUSB_MIDI_DEVICE
#error "@USB Device :: select `Tools > USB support > USB Device MIDI` from menu"
#endif

void usb_device_midi_init();
void usb_device_midi_process();
uint8_t usb_device_midi_state();

void transmit_midi_note_on(uint8_t pChannel, uint8_t pNote, uint8_t pVelocity);
void transmit_midi_note_off(uint8_t pChannel, uint8_t pNote);
void transmit_midi_cmd(uint8_t pCommand, uint8_t pChannel, uint8_t pValueA, uint8_t pValueB);

/* --- callback functions --- */

#define WEAK __attribute__ ((weak))
WEAK void receive_midi_message(uint8_t packet[4]);
WEAK void receive_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity);
WEAK void receive_midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity);
WEAK void receive_midi_pitch_bend(uint8_t channel, int16_t pValue);
WEAK void receive_midi_control_change(uint8_t channel, uint8_t controller, uint8_t value);

#endif // _USB_DEVICE_MIDI_H_
