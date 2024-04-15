/* @note make sure to select `Tools > USB support (if available) > USB Host Mouse+Keyboard+MIDI` */

#include "Arduino.h"
#include "Klangstrom.h"
#include "USBHost.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("--------------------");
    Serial.println("USBHostMouseKeyboard");
    Serial.println("--------------------");

    USBHost.init();
    USBHost.register_key_pressed(key_pressed);
    USBHost.register_key_released(key_released);
    // USBHost.register_keyboard_defaults();
    USBHost.register_mouse_pressed(mouse_pressed);
    USBHost.register_mouse_released(mouse_released);
    USBHost.register_mouse_moved(mouse_moved);
    // USBHost.register_mouse_defaults();
    USBHost.register_event_receive(event_receive);
}

void loop() {
    USBHost.process();
    LED(LED_01, USBHost.is_mouse_connected());
    LED(LED_03, USBHost.is_keyboard_connected());
}

void beat(uint32_t beat) {
    LED(LED_00, LED_TOGGLE);
}

void key_pressed(uint8_t key) {
    Serial.print("key_pressed ... : ");
    Serial.print(key);
    Serial.println();
}

void key_released(uint8_t key) {
    Serial.print("key_released .. : ");
    Serial.print(key);
    Serial.println();
}

void mouse_pressed(EVENT_DATA button, int8_t x, int8_t y) {
    Serial.print("mouse_pressed . :");
    Serial.print(" BUTTON:");
    Serial.print((int)button);
    Serial.print(" x:");
    Serial.print(x);
    Serial.print(" y:");
    Serial.print(y);
    Serial.println();
}

void mouse_released(EVENT_DATA button, int8_t x, int8_t y) {
    Serial.print("mouse_released  :");
    Serial.print(" BUTTON:");
    Serial.print((int)button);
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
        Serial.print(" KEY(");
        for (size_t i = 0; i < KEYBOARD_NUM_KEYS; i++) {
            Serial.print(keyboard_event(data).keys[i]);
            Serial.print(",");
        }
        Serial.print(")");
        Serial.print(" CODE(");
        for (size_t i = 0; i < KEYBOARD_NUM_KEYS; i++) {
            Serial.print((int)keyboard_event(data).keycodes[i]);
            Serial.print(",");
        }
        Serial.print(")");
        Serial.print(" SHIFT_LEFT: ");
        Serial.print(keyboard_event(data).SHIFT_LEFT);
        Serial.print(" GUI_LEFT: ");
        Serial.print(keyboard_event(data).GUI_LEFT);
        Serial.println();
        LED(LED_04, LED_TOGGLE);
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
        LED(LED_02, LED_TOGGLE);
    }
}
