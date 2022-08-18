/*
 * this example demonstrates how to use a board as a USB Device mouse.
 *
 * @note make sure to select `Tools > USB support > USB Device Mouse`
 */

#include "Klangstrom.h"
#include "USBDevice.h"

using namespace klangstrom;

void setup() {
    usb_device_init();
}

void loop() {}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (encoder_event(data).index == ENCODER_00) {
                led(LED_00, true);
                transmit_mouse_button(1);  // LEFT
            }
            if (encoder_event(data).index == ENCODER_01) {
                led(LED_01, true);
                transmit_mouse_button(2);  // RIGHT
            }
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            if (encoder_event(data).index == ENCODER_00) {
                led(LED_00, false);
                transmit_mouse_button(0);  // NONE
            }
            if (encoder_event(data).index == ENCODER_01) {
                led(LED_01, false);
                transmit_mouse_button(0);  // NONE
            }
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_event(data).index == ENCODER_00) {
                transmit_mouse_moved(encoder_event(data).delta * 10, 0);
            }
            if (encoder_event(data).index == ENCODER_01) {
                transmit_mouse_moved(0, encoder_event(data).delta * 10);
            }
            break;
    }
}
