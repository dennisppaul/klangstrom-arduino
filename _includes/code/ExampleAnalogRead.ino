/*
 * this example demonstrates how to read values to the ADCs.
 *
 * connect a signal to ADC_00 + ADC_01 and try running the serial plotter to show the measurements.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klangstrom;

void setup() {
    begin_serial_debug(true);
    beats_per_minute(100 * 60);  // sample at 100Hz
}

void loop() {}

void beat(uint32_t pBeat) {
    LED(LED_00, LED_TOGGLE);

    uint16_t mADC_00 = adc(ADC_00);
    uint16_t mADC_01 = adc(ADC_01);

    Serial.print("ADC_00:");
    Serial.print(mADC_00);
    Serial.print(',');
    Serial.print("ADC_01:");
    Serial.print(mADC_01);
    Serial.println();
}
