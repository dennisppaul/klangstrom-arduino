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
    Serial.println("USB Host Mouse Keyboard MIDI");
    Serial.println("-------------");
    Serial.println(__DATE__);
    Serial.println(__TIME__);
    usb_host_init();
    usb_host_callback_defaults();
    // usb_host_callback_key_pressed(key_pressed);
    // usb_host_callback_key_released(key_released);
    // usb_host_callback_mouse_pressed(mouse_pressed);
    // usb_host_callback_mouse_released(mouse_released);
    // usb_host_callback_mouse_moved(mouse_moved);
    // usb_host_callback_event_receive(event_receive);
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

void key_pressed(uint8_t key) {
    Serial.print("key_pressed ... : ");
    Serial.print((char)key);
    Serial.println();
}

void key_released(uint8_t key) {
    Serial.print("key_released .. : ");
    Serial.print((char)key);
    Serial.println();
}

void mouse_pressed(EVENT_DATA button, int8_t x, int8_t y) {
    Serial.print("mouse_pressed . :");
    Serial.print(" BUTTON:");
    Serial.print(button);
    Serial.print(" x:");
    Serial.print(x);
    Serial.print(" y:");
    Serial.print(y);
    Serial.println();
}

void mouse_released(EVENT_DATA button, int8_t x, int8_t y) {
    Serial.print("mouse_released  :");
    Serial.print(" BUTTON:");
    Serial.print(button);
    Serial.print(" x:");
    Serial.print(x);
    Serial.print(" y:");
    Serial.print(y);
    Serial.println();
}

void mouse_moved(const bool* buttons, int8_t x, int8_t y) {
    Serial.print("mouse_moved ..  :");
    Serial.print(" BUTTONS: ");
    Serial.print(buttons[MOUSE_BUTTON_LEFT] ? '+' : '-');
    Serial.print(buttons[MOUSE_BUTTON_MIDDLE] ? '+' : '-');
    Serial.print(buttons[MOUSE_BUTTON_RIGHT] ? '+' : '-');
    Serial.print(" X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.print(y);
    Serial.println();
}

void event_receive(EVENT_TYPE event, const void* data) {
    if (event == EVENT_KEYBOARD) {
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
    if (event == EVENT_MOUSE) {
        Serial.print("EVENT_MOUSE ... :");
        Serial.print(" BUTTONS: ");
        Serial.print(mouse_event(data).LEFT ? '+' : '-');
        Serial.print(mouse_event(data).MIDDLE ? '+' : '-');
        Serial.print(mouse_event(data).RIGHT ? '+' : '-');
        Serial.print(" X: ");
        Serial.print(mouse_event(data).x);
        Serial.print(" Y: ");
        Serial.print(mouse_event(data).y);
        Serial.println();
        led_toggle(LED_03);
    }
}
