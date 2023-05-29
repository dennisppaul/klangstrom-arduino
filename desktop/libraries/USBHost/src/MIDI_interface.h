#ifndef MIDI_INTERFACE_H_
#define MIDI_INTERFACE_H_

void transmit_midi_message(const uint8_t cable, const uint8_t data1, const uint8_t data2, const uint8_t data3);

void transmit_midi_note_off(const uint8_t channel, const uint8_t note);
void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
void transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
void transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value);
void transmit_midi_program_change(const uint8_t channel, const uint8_t program);
void transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
void transmit_midi_pitch_bend(const uint8_t channel, const int16_t value);
/* note, that sys ex messages are sent via this collector function */
void transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2); 

#endif /* MIDI_INTERFACE_H_ */
