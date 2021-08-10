//
// ExampleUSBDeviceMIDI
//

#include "USBDevice.h"

using namespace klangstrom;

uint8_t mNote = 48;

void setup() {
    usb_device_midi_init();
}

void loop() {
    usb_device_midi_process();
    led_blinking_task();
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            transmit_midi_note_on(0, mNote, 100);
            // same as `transmit_cmd(0x90, 0, mNote, 100)`
            led(LED_00, true);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            transmit_midi_note_off(0, mNote);
            led(LED_00, false);
            break;
        case EVENT_ENCODER_ROTATE:
            int mDelta = data[TICK] - data[PREVIOUS_TICK];
            mNote += mDelta;
            break;
    }
}

void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool mLEDSate = false;

    uint32_t blink_interval_ms = usb_device_midi_state() * 250 + 250;

    // Blink every interval ms
    if ( millis() - start_ms < blink_interval_ms) { return; }
    start_ms += blink_interval_ms;

    led(LED_01, mLEDSate);
    mLEDSate = 1 - mLEDSate;
}

void receive_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    led(LED_02, true);
}

void receive_midi_note_off(uint8_t channel, uint8_t note) {
    led(LED_02, false);
}
