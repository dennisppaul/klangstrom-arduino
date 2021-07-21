#include "TinyUSB.h"

using namespace klangstrom;

uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
uint8_t const channel   = 0; // 0 for channel 1
uint8_t mNote = 48;

void setup() {
    tusb_init();
}

void loop() {
    tud_task();
    midi_task();
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            send_note_on(mNote);
            led(LED_00, true);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            send_note_off(mNote);
            led(LED_00, false);
            break;
        case EVENT_ENCODER_ROTATE:
            int mDelta = data[TICK] - data[PREVIOUS_TICK];
            mNote += mDelta;
            break;
    }
}

void midi_task(void) {
    uint8_t packet[4];
    while ( tud_midi_available() ) {
        tud_midi_packet_read(packet);
    }
}

void send_note_on(uint8_t pNote) {
    uint8_t note_on[3] = { 0x90 | channel, pNote, 127 };
    tud_midi_stream_write(cable_num, note_on, 3);
}

void send_note_off(uint8_t pNote) {
    uint8_t note_off[3] = { 0x80 | channel, pNote, 0};
    tud_midi_stream_write(cable_num, note_off, 3);
}
