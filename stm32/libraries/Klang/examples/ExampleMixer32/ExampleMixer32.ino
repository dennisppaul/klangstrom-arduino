/*
 * this example demonstrates how to mix 32 signals into a single signal.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t NUM_OF_OSC = NodeMixer32::NUM_CH_IN;

NodeDAC          mDAC;
NodeVCOWavetable mOSC[NUM_OF_OSC];
NodeMixer32      mMixer;

void setup() {
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        Klang::connect(mOSC[i], Node::CH_OUT_SIGNAL, mMixer, i);
    }
    Klang::connect(mMixer, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        const float mOffset = 0.02;
        mOSC[i].set_frequency(DEFAULT_FREQUENCY * ((NUM_OF_OSC - i) + mOffset));
        mOSC[i].set_amplitude(0.5f / NUM_OF_OSC);
        mOSC[i].set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
