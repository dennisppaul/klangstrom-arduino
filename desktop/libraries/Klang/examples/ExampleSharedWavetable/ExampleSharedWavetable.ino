/*
 * this example demonstrates how to use one wavetable shared by multiple oscillators.
 *
 * see `ExampleOscillators` for a more detailed explanation on oscillators ( VCO ).
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t  NUM_OF_OSC     = 4;
static const uint16_t WAVETABLE_SIZE = 256;
static const float    FREQ_OFFSET    = 0.02;
static const float    BASE_FREQ      = 110;
SIGNAL_TYPE*          mWavetable     = new SIGNAL_TYPE[WAVETABLE_SIZE];

NodeDAC          mDAC;
NodeVCOWavetable mOSC[NUM_OF_OSC];
NodeMixerMulti   mMixer;

void setup() {
    NodeVCOWavetable::fill_wavetable(NodeVCOWavetable::WAVEFORM::SINE, mWavetable, WAVETABLE_SIZE);
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        mOSC[i] = NodeVCOWavetable(mWavetable, WAVETABLE_SIZE);
        Klang::connect(mOSC[i], Node::CH_OUT_SIGNAL, mMixer, i);
    }
    Klang::connect(mMixer, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    set_frequency(1);

    beats_per_minute(120 * 6);
}

void loop() {}

void set_frequency(uint8_t pOffset) {
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        mOSC[i].set_frequency(BASE_FREQ * (i + pOffset + FREQ_OFFSET));
        mOSC[i].set_amplitude(0.175 - (float)i / (float)NUM_OF_OSC * 0.125);
    }
}

void beat(uint32_t pBeat) {
    set_frequency(random(1, 12));
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
