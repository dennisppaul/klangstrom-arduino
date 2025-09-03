/**
 * this example demonstrates how to use the analog-digital-converter (ADC) and the digital-analog-converter (DAC).
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "DACADC.h"

constexpr float fixed_value = 0.5f;

void setup() {
    system_init();

    adc_init();
    dac_init();
}

void loop() {
    dac_write(fixed_value);
    const float read_value = adc_read();
    console_println("ADC: %f", read_value);
    delay(250);
}
