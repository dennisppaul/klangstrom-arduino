//
// ExampleUSBHostMouseKeyboardMIDI
//

/* @note make sure to select `Tools > USB support (if available) > USB Host Mouse+Keyboard+MIDI` */

#include "Arduino.h"
#include "Klangstrom.h"
#include "USBHost.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("-------------");
    Serial.println("USB Host MIDI");
    Serial.println("-------------");
    Serial.println(__DATE__);
    Serial.println(__TIME__);
    usb_host_init();
    usb_host_callback_event_receive(event_receive);
}

void loop() {
    usb_host_process();
}

void beat(uint32_t beat) {
    // @todo(check if transmit works)
    // transmit_midi_note_on(0, 47, 100);
    led_toggle(LED_00);
}

void receive_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    Serial.print("note_on .... : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(note);
    Serial.print(", ");
    Serial.print(velocity);
    Serial.println();
    led_toggle(LED_01);
}

void receive_midi_note_off(const uint8_t channel, const uint8_t note) {
    Serial.print("note_off ... : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(note);
    Serial.println();
    led_toggle(LED_01);
}

void receive_midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value) {
    Serial.print("CC ......... : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(controller);
    Serial.print(", ");
    Serial.print(value);
    Serial.println();
    led_toggle(LED_01);
}

void receive_midi_pitch_bend(const uint8_t channel, const int16_t value) {
    Serial.print("pitch bend . : ");
    Serial.print(channel);
    Serial.print(", ");
    Serial.print(value);
    Serial.println();
}

void receive_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2) {
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
        Serial.print("EVENT_KEYBOARD  :");
        Serial.print(" CODE(");
        Serial.print(keyboard_event(data).keycodes[0]);
        Serial.print(")");
        Serial.print(" SHIFT_LEFT: ");
        Serial.print(keyboard_event(data).SHIFT_LEFT);
        Serial.print(" GUI_LEFT: ");
        Serial.print(keyboard_event(data).GUI_LEFT);
        Serial.print(" KEY: ");
        Serial.print((char)(keyboard_event(data).keys[0]));
        Serial.println();
        led_toggle(LED_02);
    }
}
