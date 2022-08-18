/*
 * this example demonstrates how to use a board as a MIDI USB Device.
 *
 * @note make sure to select `Tools > USB support > USB Device MIDI`
 * 
 * @todo( there seems to be a problem when sending MIDI messages to the host. this can be temporarily fixed by copying 
 * the USB Device library sources to the core arduino folder ( however, this needs to be fixed properly ): 
 * `% cp $KLANGSTROM/stm32/libraries/USBDevice/src/* $KLANGSTROM/stm32/cores/arduino/` )
 */

#include "Klangstrom.h"
#include "USBDevice.h"

using namespace klangstrom;

uint8_t mNote = 48;

void setup() {
    Serial.begin(115200);
    usb_device_init();
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            led(LED_00, true);
            transmit_midi_note_on(0, mNote, 85);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            led(LED_00, false);
            transmit_midi_note_off(0, mNote);
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_event(data).delta > 0) {
                mNote++;
            } else {
                mNote--;
            }
            break;
    }
}

void loop() {}

void receive_midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    led(LED_02, true);
}

void receive_midi_note_off(uint8_t channel, uint8_t note) {
    led(LED_02, false);
}
