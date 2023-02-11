#ifndef MIDI_APPLICATION_H_
#define MIDI_APPLICATION_H_

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "usbh_core.h"
#include "usbh_MIDI.h"
#include "MIDI_interface.h"

void MIDI_Application(void);

/* --- callback functions --- */

#define WEAK __attribute__ ((weak))
WEAK void receive_midi_note_off(const uint8_t channel, const uint8_t note);
WEAK void receive_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
// @TODO( add 'aftertouch' )
WEAK void receive_midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value);
// @TODO( add 'program change' )
// @TODO( add 'c? touch' )
WEAK void receive_midi_pitch_bend(const uint8_t channel, const int16_t value);
WEAK void receive_midi_message(const uint8_t command, const uint8_t channel, const uint8_t value_a, const uint8_t value_b);

#endif /* MIDI_APPLICATION_H_ */
