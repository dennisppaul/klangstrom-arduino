#ifdef TINYUSB_MIDI_DEVICE

#include "USBDeviceTinyUSB.h"
#include "USBDeviceMIDI.h"

enum  {
    MIDI_NOT_MOUNTED = 0,
    MIDI_MOUNTED = 1,
    MIDI_SUSPENDED = 2,
};
uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
static uint32_t mState = MIDI_NOT_MOUNTED;

void usb_device_midi_init() {
  tusb_init();
}

#define MIDI_NOTE_ON        0x90
#define MIDI_NOTE_OFF       0x80
#define MIDI_AFTERTOUCH     0xA0
#define MIDI_PITCHBEND      0xE0
#define MIDI_CONTROL_CHANGE 0xB0

static void parse_midi_packet(uint8_t packet[4]) {
    // const uint8_t mCable = packet[0];
    const uint8_t channel = packet[1] & 0x0F;
    const uint8_t command = packet[1] & 0xF0;
    const uint8_t a = packet[2];
    const uint8_t b = packet[3];
    switch(command) {
        case MIDI_NOTE_OFF     		: receive_midi_note_off(channel, a); break;
        case MIDI_NOTE_ON           : receive_midi_note_on(channel, a, b); break;
        case MIDI_AFTERTOUCH        : receive_midi_aftertouch(channel, a, b); break;
        case MIDI_CONTROL_CHANGE    : receive_midi_control_change(channel, a, b); break;
        case MIDI_PROGRAM_CHANGE    : receive_midi_program_change(channel, a); break;
        case MIDI_CHANNEL_PRESSURE  : receive_midi_channel_pressure(channel, a); break;
        case MIDI_PITCHBEND         : receive_midi_pitch_bend(channel, (int16_t)(((a << 7) + b) - 0x2000)); break;
        case MIDI_SYS_EX			: receive_midi_sys_ex(channel, a, b); break;
        default                     : receive_midi_message(packet[0], packet[1], packet[2], packet[3]);
    }
}

static void midi_task(void) {
    uint8_t packet[4];
    while ( tud_midi_available() ) {
        tud_midi_packet_read(packet);
        parse_midi_packet(packet);
        receive_midi_message(packet[0], packet[1], packet[2], packet[3]);
    }
}

void usb_device_midi_process() {
    tud_task();
    midi_task();
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void) {
    mState = MIDI_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
    mState = MIDI_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    mState = MIDI_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void) {
    mState = MIDI_MOUNTED;
}

uint8_t usb_device_midi_state() {
    return (uint8_t)mState;
}

/* --- MIDI transmit --- */

static void transmit_midi_command(const uint8_t command, const uint8_t channel, const uint8_t value_a, const uint8_t value_b) {
    const uint8_t mCMD = command | channel;
    transmit_midi_message(cable_num, mCMD, value_a, value_b);
}

void transmit_midi_note_off(const uint8_t channel, const uint8_t note) {
    transmit_midi_command(MIDI_NOTE_OFF, channel, note, 0);
}

void transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    transmit_midi_command(MIDI_NOTE_ON, channel, note, velocity);
}

void transmit_midi_aftertouch(const uint8_t channel, const uint8_t note, const uint8_t pressure) {
    transmit_midi_command(MIDI_AFTERTOUCH, channel, note, pressure);    
}

void transmit_midi_control_change(const uint8_t channel, const uint8_t command, const uint8_t value) {
    transmit_midi_command(MIDI_CONTROL_CHANGE, channel, command, value);    
}

void transmit_midi_program_change(const uint8_t channel, const uint8_t program) {
    transmit_midi_command(MIDI_PROGRAM_CHANGE, channel, program, 0);    
}

void transmit_midi_channel_pressure(const uint8_t channel, const uint8_t pressure) {
    transmit_midi_command(MIDI_CHANNEL_PRESSURE, channel, pressure, 0);    
}

void transmit_midi_pitch_bend(const uint8_t channel, const int16_t value) {
   	// @todo(test this!)
	const uint16_t mValue = (uint16_t)value;
	const uint16_t LSB_MASK = 0b00000001111111;
	const uint16_t MSB_MASK = 0b11111110000000;
	const uint8_t msb = (mValue & MSB_MASK) / 128;
	const uint8_t lsb = mValue & LSB_MASK;
    transmit_midi_command(MIDI_PITCHBEND, channel, lsb, msb);    
}

void transmit_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2) {
   	transmit_midi_message(cable_num, type, data1, data2);
}

void transmit_midi_message(const uint8_t cable, const uint8_t data1, const uint8_t data2, const uint8_t data3) {
    const uint8_t mData[3] = { data1, data2, data3};
    tud_midi_stream_write(cable_num, mData, 3);
}

#endif // TINYUSB_MIDI_DEVICE
