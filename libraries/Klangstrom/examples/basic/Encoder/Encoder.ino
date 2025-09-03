/**
 * this example demonstrates how to use encoders.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Encoder.h"

Encoder* encoder_LEFT;
Encoder* encoder_RIGHT;

void setup() {
    system_init();

    encoder_LEFT  = encoder_create(ENCODER_LEFT);
    encoder_RIGHT = encoder_create(ENCODER_RIGHT);
}

void loop() {}

void encoder_event(Encoder* encoder, uint8_t event) {
    if (event == ENCODER_EVENT_ROTATION) {
        console_println("ENCODER_EVENT_ROTATION(%i): %i ", encoder->device_type, encoder->rotation);
    }
    if (event == ENCODER_EVENT_BUTTON) {
        if (encoder->button_pressed == ENCODER_PRESSED && encoder->device_type == ENCODER_LEFT) {
            if (encoder_RIGHT->started) {
                encoder_stop(encoder_RIGHT);
                console_println("ENCODER_RIGHT stopped");
            } else {
                encoder_start(encoder_RIGHT);
                console_println("ENCODER_RIGHT started");
            }
        }
        console_println("ENCODER_EVENT_BUTTON  (%i): %i ", encoder->device_type, encoder->button_pressed);
    }
}
