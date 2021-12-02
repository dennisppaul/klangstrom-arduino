//
// ExampleUSBHostMouseKeyboardMIDI
//

/* @note make sure to select `Tools > USB support (if available) > USB Host Mouse+Keyboard+MIDI` */

#include "USBHost.h"

using namespace klangstrom;

void setup() {
    KLST_SERIAL_00.println("-------------");
    KLST_SERIAL_00.println("USB Host Demo");
    KLST_SERIAL_00.println("-------------");
    usb_host_init();
}

void loop() {
    usb_host_process();
}

void beat(uint32_t beat) {
    // @todo(check if transmit works)
    transmit_midi_note_on(0, 47, 100);
    led_toggle(LED_02);
}

void receive_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity) {
    KLST_SERIAL_00.print("note_on .... : ");
    KLST_SERIAL_00.print(channel);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(note);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(velocity);
    KLST_SERIAL_00.println();
    led_toggle(LED_01);
}

void receive_midi_note_off(const uint8_t channel, const uint8_t note) {
    KLST_SERIAL_00.print("note_off ... : ");
    KLST_SERIAL_00.print(channel);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(note);
    KLST_SERIAL_00.println();
    led_toggle(LED_01);
}

void receive_midi_control_change(const uint8_t channel, const uint8_t controller, const uint8_t value) {
    KLST_SERIAL_00.print("CC ......... : ");
    KLST_SERIAL_00.print(channel);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(controller);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(value);
    KLST_SERIAL_00.println();
    led_toggle(LED_01);
}

void receive_midi_pitch_bend(const uint8_t channel, const int16_t value) {
    KLST_SERIAL_00.print("pitch bend . : ");
    KLST_SERIAL_00.print(channel);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(value);
    KLST_SERIAL_00.println();
}

void receive_midi_sys_ex(const uint8_t type, const uint8_t data1, const uint8_t data2) {
    KLST_SERIAL_00.print("sys ex ..... : ");
    KLST_SERIAL_00.print(type);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(data1);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(data2);
    KLST_SERIAL_00.println();
}

void receive_mouse(uint8_t x, uint8_t y, uint8_t* buttons) {
    KLST_SERIAL_00.print("mouse ...... : ");
    KLST_SERIAL_00.print(x);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(y);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(buttons[0]);
    KLST_SERIAL_00.println();
    led_toggle(LED_01);
}

void receive_keyboard(uint8_t* keys) {
    KLST_SERIAL_00.print("keyboard ... : ");
    KLST_SERIAL_00.print((char)(keys[0]+93));
    KLST_SERIAL_00.println();
    led_toggle(LED_01);
}
