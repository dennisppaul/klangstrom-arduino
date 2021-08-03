#ifdef TINYUSB_MIDI_DEVICE

#include "USBDevice.h"
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
        case MIDI_NOTE_ON           : receive_midi_note_on(channel, a, b); break;
        case MIDI_NOTE_OFF          : receive_midi_note_off(channel, a, b); break;
        case MIDI_AFTERTOUCH        : break;
        case MIDI_PITCHBEND         : receive_midi_pitch_bend(channel, (int16_t)(((a << 7) + b) - 0x2000)); break;
        case MIDI_CONTROL_CHANGE    : receive_midi_control_change(channel, a, b); break;
    }
}

static void midi_task(void) {
    uint8_t packet[4];
    while ( tud_midi_available() ) {
        tud_midi_packet_read(packet);
        parse_midi_packet(packet);
        receive_midi_message(packet);
    }
}

void usb_device_midi_process() {
    tud_task();
    midi_task();
}

void transmit_midi_note_on(const uint8_t channel,
                           const uint8_t note,
                           const uint8_t velocity) {
    transmit_midi_cmd(MIDI_NOTE_ON, channel, note, velocity);
}

void transmit_midi_note_off(const uint8_t channel,
                            const uint8_t note) {
    transmit_midi_cmd(MIDI_NOTE_OFF, channel, note, 0);
}

void transmit_midi_control_change(const uint8_t channel,
                                  const uint8_t command,
                                  const uint8_t value) {
    transmit_midi_cmd(MIDI_CONTROL_CHANGE, channel, command, value);
}

void transmit_midi_cmd(const uint8_t pCommand,
                       const uint8_t pChannel,
                       const uint8_t pValueA,
                       const uint8_t pValueB) {
    const uint8_t mCMD = pCommand | pChannel;
    const uint8_t mData[3] = { mCMD, pValueA, pValueB};
    tud_midi_stream_write(cable_num, mData, 3);
}

uint8_t usb_device_midi_state() {
    return (uint8_t)mState;
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

#endif // TINYUSB_MIDI_DEVICE
