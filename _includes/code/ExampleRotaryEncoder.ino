/**
    ExampleRotaryEncoder

    this examples demonstrates how the use the rotary encoders and the events
    they produce. the example connects 3 oscillators to a mixer, which is
    connected to an ADSR. pressing the buttons triggers the ADSR, rotating the
    encoders changes the frequency of each oscillator.
*/

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t NUM_OF_OSC = 3;
NodeVCOWavetable     mVCO[NUM_OF_OSC];
NodeADSR             mADSR;
NodeDAC              mDAC;
NodeMixer4           mMixer;
NodeValue            mValue;

float mOSCFreq[NUM_OF_OSC];
float mBaseFreq = 220;

void setup() {
    Serial.begin(115200);
    Serial.println("ExampleRotaryEncoder");

    Klang::lock();

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        Klang::connect(mVCO[i], Node::CH_OUT_SIGNAL, mMixer, i);
    }

    Klang::connect(mMixer,  Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR,   Node::CH_OUT_SIGNAL, mDAC,  Node::CH_IN_SIGNAL);

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        mOSCFreq[i] = mBaseFreq + i * mBaseFreq;
        mVCO[i].set_frequency(mOSCFreq[i]);
        mVCO[i].set_amplitude(0.7 - i * 0.15);
        mVCO[i].set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    }

    mADSR.set_attack(0.25);
    mADSR.set_decay(0.0);
    mADSR.set_sustain(1.0);
    mADSR.set_release(2.5);

    Klang::unlock();
}

void loop() {
    /*  note that the iteration time for each loop needs to be kept short because all
        events ( e.g encoder, serial ) are processed right after loop is finished.
    */
    delay(10);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    print_encoder_state(event, data);

    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            mADSR.start();
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            mADSR.stop();
            break;
        case EVENT_ENCODER_ROTATED:
            if (data[INDEX] == ENCODER_00) {
                change_freq(0, data[TICK] - data[PREVIOUS_TICK]);
            } else if (data[INDEX] == ENCODER_01) {
                change_freq(1, data[TICK] - data[PREVIOUS_TICK]);
            } else if (data[INDEX] == ENCODER_02) {
                change_freq(2, data[TICK] - data[PREVIOUS_TICK]);
            }
            break;
    }
}

void change_freq(uint8_t i, float mFreqChange) {
    mOSCFreq[i] += mFreqChange;
    mVCO[i].set_frequency(mOSCFreq[i]);
}

void print_encoder_state(const uint8_t event, const float* data) {
    /* print encoder states + toggle LEDs */
    if (event == EVENT_ENCODER_ROTATED) {
        if (data[INDEX] == ENCODER_00) {
            Serial.print("ENCODER_00 ROTATION               : ");
            Serial.print(data[TICK]);
            Serial.print(", ");
            Serial.print(data[TICK] - data[PREVIOUS_TICK]);
            Serial.println();
            led_toggle(LED_00);
        } else if (data[INDEX] == ENCODER_01) {
            Serial.print("ENCODER_01 ROTATION               : ");
            Serial.print(data[TICK]);
            Serial.print(", ");
            Serial.print(data[TICK] - data[PREVIOUS_TICK]);
            Serial.println();
            led_toggle(LED_01);
        } else if (data[INDEX] == ENCODER_02) {
            Serial.print("ENCODER_02 ROTATION               : ");
            Serial.print(data[TICK]);
            Serial.print(", ");
            Serial.print(data[TICK] - data[PREVIOUS_TICK]);
            Serial.println();
            led_toggle(LED_02);
        }
    }

    if (event == EVENT_ENCODER_BUTTON_PRESSED) {
        if (data[INDEX] == ENCODER_00) {
            Serial.println("EVENT_ENCODER_BUTTON_00_PRESSED");
            led(LED_00, true);
        } else if (data[INDEX] == ENCODER_01) {
            Serial.println("EVENT_ENCODER_BUTTON_01_PRESSED");
            led(LED_01, true);
        } else if (data[INDEX] == ENCODER_02) {
            Serial.println("EVENT_ENCODER_BUTTON_02_PRESSED");
            led(LED_02, true);
        }
    }
    if (event == EVENT_ENCODER_BUTTON_RELEASED) {
        if (data[INDEX] == ENCODER_00) {
            Serial.println("EVENT_ENCODER_BUTTON_00_RELEASED");
            led(LED_00, false);
        } else if (data[INDEX] == ENCODER_01) {
            Serial.println("EVENT_ENCODER_BUTTON_01_RELEASED");
            led(LED_01, false);
        } else if (data[INDEX] == ENCODER_02) {
            Serial.println("EVENT_ENCODER_BUTTON_02_RELEASED");
            led(LED_02, false);
        }
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
