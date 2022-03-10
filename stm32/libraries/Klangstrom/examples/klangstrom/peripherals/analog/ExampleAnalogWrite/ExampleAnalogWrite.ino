/*
 * this example demonstrates how to write values to the DACs.
 *
 * connect an oscilloscope to DAD_00 + DAC_01 to visualize the output.
 */

#include "KlangNodes.hpp"

using namespace klangstrom;

uint16_t mDAC_00 = 0;
uint16_t mDAC_01 = 0;

void setup() {
    begin_serial_debug(true);
    beats_per_minute(100 * 60);  // sample at 100Hz
}

void beat(uint32_t pBeat) {
    LED(LED_00, LED_TOGGLE);

    mDAC_00 += 256;
    mDAC_01 += 2048;
    DAC(DAC_00, mDAC_00);
    DAC(DAC_01, mDAC_01);

    Serial.print("DAC_00:");
    Serial.print(mDAC_00);
    Serial.print(',');
    Serial.print("DAC_01:");
    Serial.print(mDAC_01);
    Serial.println();
}
