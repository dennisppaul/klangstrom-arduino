/**
 * this example demonstrates how to use LEDs.
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "LED.h"

uint8_t fLEDIntensity = 0;

void setup() {
    system_init();
    led_init();

    led_on(KLST_LED_ALL);
}

void loop() {
    led_toggle(0);
    led_set(1, fLEDIntensity / 255.0);
    fLEDIntensity += 16;

    console_timestamp(true);
    console_println("LED 00: %f", led_get(0));
    console_println("LED 01: %f", led_get(1));

    delay(500);
}
