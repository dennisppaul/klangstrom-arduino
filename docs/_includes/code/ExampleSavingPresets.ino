//
// ExampleSavingPresets
//

#include "Klangstrom.h"
#include <EEPROM.h>

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

/* define struct to hold presets */
struct Presets {
    float frequency = 440.0;
    float amplitude = 0.4;
};

Presets mPresets;

void setup() {
    begin_serial_debug(true);

    /* hold button 00 at start up to reset presets */
    if (button_state(KLST_BUTTON_ENCODER_00)) {
        serial_debug.println("reset presets");
        EEPROM.put(0, mPresets);
    } else {
        serial_debug.println("read presets");
        EEPROM.get(0, mPresets);
    }

    Klang::lock();
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    mVCO.set_amplitude(mPresets.amplitude);
    mVCO.set_frequency(mPresets.frequency);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void loop() {
    led(LED_00, true);
    mVCO.set_amplitude(mPresets.amplitude);
    delay(500);
    led(LED_00, false);
    mVCO.set_amplitude(0.0);
    delay(500);
    Serial.print(mPresets.amplitude);
    Serial.print(", ");
    Serial.print(mPresets.frequency);
    Serial.println();
}

void event_receive(const EVENT_TYPE event, const void* data) {
    if (event == EVENT_ENCODER_BUTTON_PRESSED) {
        if (data[INDEX] == ENCODER_00) {
            serial_debug.println("write presets 0");
            mPresets.amplitude = 0.4;
            mPresets.frequency = 440.0;
            mVCO.set_amplitude(mPresets.amplitude);
            mVCO.set_frequency(mPresets.frequency);
            EEPROM.put(0, mPresets);
        } else if (data[INDEX] == ENCODER_01) {
            serial_debug.println("write presets 1");
            mPresets.amplitude = 0.5;
            mPresets.frequency = 220.0;
            mVCO.set_amplitude(mPresets.amplitude);
            mVCO.set_frequency(mPresets.frequency);
            EEPROM.put(0, mPresets);
        } else if (data[INDEX] == ENCODER_02) {
            serial_debug.println("write presets 2");
            mPresets.amplitude = 0.6;
            mPresets.frequency = 110.0;
            mVCO.set_amplitude(mPresets.amplitude);
            mVCO.set_frequency(mPresets.frequency);
            EEPROM.put(0, mPresets);
        }
    }
}

void audioblock(float* output_signal[LEFT], float* output_signal[RIGHT],
                float* input_signal[LEFT], float* input_signal[RIGHT]) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}
