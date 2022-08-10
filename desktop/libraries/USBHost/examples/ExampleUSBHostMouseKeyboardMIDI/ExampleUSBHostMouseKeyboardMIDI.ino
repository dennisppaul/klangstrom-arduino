//
// ExampleUSBHostMouseKeyboardMIDI
//

/* @note make sure to select `Tools > USB support (if available) > USB Host Mouse+Keyboard+MIDI` */

#include "Klangstrom.h"
#include "USBHost.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("-------------");
    Serial.println("USB Host Demo");
    Serial.println("-------------");
    usb_host_init();
}

void loop() {
    usb_host_process();
}

void beat(uint32_t beat) {
    // @todo(check if transmit works)
    // transmit_midi_note_on(0, 47, 100);
    led_toggle(LED_02);
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

void receive_mouse(uint8_t x, uint8_t y, uint8_t* buttons) {
    Serial.print("mouse ...... : ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(buttons[0]);
    Serial.println();
    led_toggle(LED_01);
}

void receive_keyboard(uint8_t* keys) {
    Serial.print("keyboard ... : ");
    Serial.print((char)(keys[0]+93));
    Serial.println();
    led_toggle(LED_01);
}
