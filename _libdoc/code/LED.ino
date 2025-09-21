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
    led_toggle(LED_LEFT);
    led_set(LED_RIGHT, fLEDIntensity / 255.0);
    fLEDIntensity += 16;

    console_timestamp(true);
    console_println("LED_LEFT : %f", led_get(LED_LEFT));
    console_println("LED_RIGHT: %f", led_get(LED_RIGHT));

    delay(500);
}
