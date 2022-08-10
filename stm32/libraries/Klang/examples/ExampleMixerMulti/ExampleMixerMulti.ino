/*
 * this example demonstrates how to mix 32 ( theoretically up to 256 ) signals into a single signal.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t NUM_OF_OSC  = 11;
static const float   M_FREQUENCY = 110.0;

NodeDAC          mDAC;
NodeVCOWavetable mOSC[NUM_OF_OSC];
NodeMixerMulti   mMixer;

void setup() {
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        Klang::connect(mOSC[i], Node::CH_OUT_SIGNAL, mMixer, i);
    }
    Klang::connect(mMixer, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        const float mOffset = 5.13 * M_FREQUENCY * (float)i / NUM_OF_OSC;
        mOSC[i].set_frequency(M_FREQUENCY * 0.5 + mOffset);
        mOSC[i].set_amplitude(0.1 + 0.9 * (float)(NUM_OF_OSC - i) / NUM_OF_OSC);
        mOSC[i].set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
        mMixer.set_mix(i, 1.0 / NUM_OF_OSC);
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
