/* @note make sure to select `Tools > USB support (if available) > USB Host Mouse+Keyboard+MIDI` */

#include "Arduino.h"
#include "Klangstrom.h"
#include "USBHost.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("-----------");
    Serial.println("USBHostMIDI");
    Serial.println("-----------");

    USBHost.init();
    USBHost.register_midi_note_off(midi_note_off);
    USBHost.register_midi_note_on(midi_note_on);
    USBHost.register_midi_clock_tick(midi_clock_tick);
    USBHost.register_midi_clock_start(midi_clock_start);
    USBHost.register_midi_clock_continue(midi_clock_continue);
    USBHost.register_midi_clock_stop(midi_clock_stop);
    // USBHost.register_midi_defaults();
    USBHost.register_event_receive(event_receive);
}

void loop() {
    USBHost.process();
    LED(LED_04, USBHost.is_midi_connected());
}

void beat(uint32_t beat) {
    // @todo(check if transmit works)
    // transmit_midi_note_on(0, 47, 100);
    LED(LED_00, LED_TOGGLE);
}

void midi_note_off(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    Serial.print("note_off ... : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(note);
    Serial.print(", ");
    Serial.print(velocity);
    Serial.println();
    LED(LED_01, LED_TOGGLE);
}

void midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    Serial.print("note_on .... : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(note);
    Serial.print(", ");
    Serial.print(velocity);
    Serial.println();
    LED(LED_01, LED_TOGGLE);
}

void midi_clock_tick() {
    Serial.println("clock tick");
}
void midi_clock_start() {
    Serial.println("clock start");
}
void midi_clock_continue() {
    Serial.println("clock continue");
}
void midi_clock_stop() {
    Serial.println("clock stop");
}

void midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value) {
    Serial.print("CC ......... : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(controller);
    Serial.print(", ");
    Serial.print(value);
    Serial.println();
    LED(LED_01, LED_TOGGLE);
}

void midi_pitch_bend(const uint8_t channel, const int16_t value) {
    Serial.print("pitch bend . : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(value);
    Serial.println();
}

void midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2) {
    Serial.print("sys ex ..... : ");
    Serial.print(type);
    Serial.print(", ");
    Serial.print(data1);
    Serial.print(", ");
    Serial.print(data2);
    Serial.println();
}

void event_receive(EVENT_TYPE event, const void* data) {
    if (event == EVENT_MIDI) {
        if (midi_event(data).command != MIDI_NOTE_ON && midi_event(data).command != MIDI_NOTE_OFF && midi_event(data).command != MIDI_CLOCK_TICK) {
            Serial.print("EVENT_MIDI  :");
            Serial.print("channel(");
            // Serial.print(midi_event(data).channel);
            Serial.printf("0x%02X", midi_event(data).channel);
            Serial.print(") ");
            Serial.print("command(");
            // Serial.print(midi_event(data).command);
            Serial.printf("0x%02X", midi_event(data).command);
            Serial.print(") ");
            Serial.print("a(");
            // Serial.print(midi_event(data).a);
            Serial.printf("0x%02X", midi_event(data).a);
            Serial.print(") ");
            Serial.print("b(");
            // Serial.print(midi_event(data).b);
            Serial.printf("0x%02X", midi_event(data).b);
            Serial.print(") ");
            Serial.println();
        }
        LED(LED_02, LED_TOGGLE);
    }
    if (event == EVENT_ENCODER_BUTTON_PRESSED) {
        // USBHost.transmit_midi_sys_ex(MIDI_CLOCK_START, 0, 0);
        USBHost.transmit_midi_clock_start();
        // USBHost.transmit_midi_sys_ex(MIDI_CLOCK_CONTINUE, 0, 0);
        Serial.println("MIDI_CLOCK_START");
    }
    if (event == EVENT_ENCODER_BUTTON_RELEASED) {
        USBHost.transmit_midi_sys_ex(MIDI_CLOCK_STOP, 0, 0);
        Serial.println("MIDI_CLOCK_STOP");
    }
}
