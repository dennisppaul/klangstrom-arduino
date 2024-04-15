/*
 * this example demonstrates how to use a board as a USB Device keyboard.
 *
 * @note make sure to select `Tools > USB support > USB Device Keyboard`
 */

#include "Klangstrom.h"
#include "USBDevice.h"

using namespace klangstrom;

char mChar = 'a';

void setup() {
    Serial.begin(115200);
    Serial.println("-----------------");
    Serial.println("USBDeviceKeyboard");
    Serial.println("-----------------");

    USBDevice.init();
}

void loop() {}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            LED(LED_00, true);
            transmit_keyboard_key_pressed(mChar);
            Serial.println("key_pressed");
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            LED(LED_00, false);
            transmit_keyboard_key_released(mChar);
            Serial.println("key_released");
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_event(data).delta > 0) {
                mChar++;
            } else {
                mChar--;
            }
            Serial.println(mChar);
            break;
    }
}
