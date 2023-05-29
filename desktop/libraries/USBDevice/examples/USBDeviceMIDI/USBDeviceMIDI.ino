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
    Serial.println("---------------");
    Serial.println("USB Device MIDI");
    Serial.println("---------------");
    Serial.println(__DATE__);
    Serial.println(__TIME__);
    USBDevice.init();
    USBDevice.register_midi_note_on(midi_note_on);
    USBDevice.register_midi_note_off(midi_note_off);
    USBDevice.register_midi_clock_tick(midi_clock_tick);
    USBDevice.register_event_receive(event_receive);

    beats_per_minute(120 * 24);
}

void beat(uint32_t beat_count) {
    USBDevice.transmit_midi_message(0xF8, 0, 0);
    LED(LED_04, LED_TOGGLE);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            LED(LED_00, true);
            USBDevice.transmit_midi_note_on(0, mNote, 85);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            LED(LED_00, false);
            USBDevice.transmit_midi_note_off(0, mNote, 0);
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_event(data).delta > 0) {
                mNote++;
            } else {
                mNote--;
            }
            Serial.println(mNote);
            break;
        case EVENT_MIDI:
            // LED(LED_04, LED_TOGGLE);
            // TODO crashes if `Serial` is used here
            // Serial.print("midi    ..... : ");
            // Serial.print("channel(");
            // Serial.print(midi_event(data).channel, HEX);
            // Serial.print(") command(");
            // Serial.print(midi_event(data).command, HEX);
            // Serial.print(")");
            // Serial.println();
            break;
    }
}

void loop() {}

void midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    Serial.print("note_on ..... : ");
    Serial.print(note);
    Serial.print(", ");
    Serial.print(velocity);
    Serial.println();
    LED(LED_02, true);
}

void midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity) {
    Serial.print("note_off .... : ");
    Serial.print(note);
    Serial.print(", ");
    Serial.print(velocity);
    Serial.println();
    LED(LED_02, false);
}

void midi_clock_tick() {
    Serial.println("clock tick");
    LED(LED_03, false);
}
