//
// ExampleUSBDeviceMouseKeyboard
//

/* @note make sure to select `Tools > USB support (if available) > USB Device Mouse+Keyboard` */

#include "USBDevice.h"

using namespace klangstrom;

void setup() {
    usb_device_init();
}

void loop() {
    led_blinking_task();
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            transmit_keyboard_key_pressed('c');
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            transmit_keyboard_key_released();
            break;
        case EVENT_ENCODER_ROTATED:
            int mScale = 5;
            if (data[INDEX] == ENCODER_00) {
                int mDelta = data[TICK] - data[PREVIOUS_TICK];
                transmit_mouse_moved(mDelta * mScale, 0);
            } else if (data[INDEX] == ENCODER_01) {
                int mDelta = data[TICK] - data[PREVIOUS_TICK];
                transmit_mouse_moved(0, mDelta * mScale);
            }
            break;
    }
}

void led_blinking_task() {
    static uint32_t start_ms = 0;
    static bool led_state = false;

    uint32_t blink_interval_ms = usb_device_mouse_keyboard_state() * 250 + 250;

    // Blink every interval ms
    if ( millis() - start_ms < blink_interval_ms) return; // not enough time
    start_ms += blink_interval_ms;

    led(LED_01, led_state);
    led_state = 1 - led_state; // toggle
}
