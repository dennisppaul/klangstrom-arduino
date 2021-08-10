#ifndef MIDI_INTERFACE_H_
#define MIDI_INTERFACE_H_

#define MIDI_NOTE_OFF               0x80
#define MIDI_NOTE_ON                0x90
#define MIDI_AFTERTOUCH             0xA0
#define MIDI_CONTROL_CHANGE         0xB0
#define MIDI_PROGRAM_CHANGE         0xC0
#define MIDI_CHANNEL_PRESSURE       0xD0
#define MIDI_PITCHBEND              0xE0
#define MIDI_SYS_EX                 0xF0

#define MIDI_SONG_POSITION_POINTER  0xF2
#define MIDI_CLOCK_TICK             0xF8
#define MIDI_CLOCK_START            0xFA
#define MIDI_CLOCK_CONTINUE         0xFB
#define MIDI_CLOCK_STOP             0xFC

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

#define WEAK __attribute__ ((weak))
WEAK void receive_midi_note_off(const uint8_t channel, const uint8_t note);
WEAK void receive_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);
WEAK void receive_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure);
WEAK void receive_midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value);
WEAK void receive_midi_program_change(const uint8_t channel, const uint8_t program);
WEAK void receive_midi_channel_pressure(const uint8_t channel, const uint8_t pressure);
WEAK void receive_midi_pitch_bend(const uint8_t channel, const int16_t value);
WEAK void receive_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2);
WEAK void receive_midi_message(const uint8_t cable, const uint8_t data1, const uint8_t data2, const uint8_t data3);

// @todo(implement MIDI SysEx commands)
/*
- 0xF0 - System Exclusive Start, with data1 being the manufacturer's ID and data2 the data itself. This message makes
 up for all that MIDI doesn't support. Data1 is usually a seven-bit Manufacturer's I.D. code. If a device recognizes 
 the I.D. code as its own, it will listen to the rest of the message, otherwise it will be ignored. System Exclusive 
 is used to send data in bulk, such as patch parameters and other non MIDI spec data. (Note: Only Real-Time messages 
 may be interleaved with a System Exclusive.) This message also is used for extensions called Universal Exclusive 
 Messages.
- 0xF1 - MIDI Time Code, Quarter Frame
- 0xF2 - System Common, Song Pointer, with data1 and data2 being LSB and MSB respectively. This is an internal 14 bit
 register that holds the number of MIDI beats (1 beat = six MIDI clock messages) since the start of the song.
- 0xF3 - System Common, Song Select, with data1 being the Song Number, data2 unused
- 0xF4 - Undefined. (Reserved)
- 0xF5 - Undefined. (Reserved)
- 0xF6 - Tune Request. This is a request of all analog synthesizers to tune their oscillators
- 0xF7 - System Exclusive End, data1 and data2 unused
- 0xF8 - System Realtime, Timing Clock, Sent 24 times per quarter note when synchronization is required
- 0xF9 - System Realtime, Measure End
- 0xFA - System Realtime, Start the current sequence playing. (This message will be followed with Timing Clocks.)
- 0xFB - System Realtime, Continue at the point the sequence was Stopped
- 0xFC - System Realtime, Stop the current sequence.
- 0xFD - Undefined. (Reserved)
- 0xFE - System Realtime, Active Sensing. Use of this message is optional. When initially sent, the receiver will 
expect to receive another Active Sensing message each 300ms (max), or it will be assume that the connection has been 
terminated. At termination, the receiver will turn off all voices and return to normal (non-active sensing) operation.
- 0xFF - System Realtime, Reset. Reset all receivers in the system to power-up status. This should be used sparingly,
 preferably under manual control. In particular, it should not be sent on power-up.
*/

#endif /* MIDI_INTERFACE_H_ */
