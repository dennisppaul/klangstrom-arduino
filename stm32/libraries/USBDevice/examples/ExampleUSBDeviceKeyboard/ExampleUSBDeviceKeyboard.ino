/*
 * this example demonstrates how to use a board as a USB Device keyboard.
 *
 * @note make sure to select `Tools > USB support > USB Device Keyboard` */
* /

#include "Klangstrom.h"
#include "USBDevice.h"

using namespace klangstrom;

char mChar = 'a';

void setup() {
    usb_device_init();
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            led(LED_00, true);
            transmit_keyboard_key_pressed(mChar);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            led(LED_00, false);
            transmit_keyboard_key_released(mChar);
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_event(data).delta > 0) {
                mChar++;
            } else {
                mChar--;
            }
            break;
    }
}
