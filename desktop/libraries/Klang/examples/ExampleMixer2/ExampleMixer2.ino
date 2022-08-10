/*
 * this example demonstrates how to mix two signals into a single signal. it also shows how to control the mixing
 * feature with an LFO. the paramter can also be controlled manually with `set_mix(value)` where a value of `0.0`
 * means only signal 0 is output, `1.0` means only signal 1 is output, `0.5` means the two signals are out with
 * equal value.
 *
 * note that the output is not averaged i.e if two signals of `0.7` and `0.9` are mixed, the output will be `1.6` and
 * not `0.8`.
 *
 * there are several mixer nodes available:
 *
 * - `NodeMixer2` :: mixes 2 signals into a single signal
 * - `NodeMixer4` :: mixes 4 signals into a single signal
 * - `NodeMixer8` :: mixes 8 signals into a single signal
 * - `NodeMixer16` :: mixes 16 signals into a single signal
 * - `NodeMixer32` :: mixes 32 signals into a single signal
 * - `NodeMixerMulti` :: mixes multiple signals into a single signal
 * - `NodeMixer4Stereo` :: mixes 4 signals into two signals ( e.g stereo output )
 * - `NodeMixerMultiStereo` :: mixes multiple signals into two signals ( e.g stereo output )
 *
 * channels with fixed input signal channel numbers can be processed faster.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;

NodeDAC          mDAC;
NodeVCOWavetable mOscLeft;
NodeVCOWavetable mOscRight;
NodeVCOWavetable mOscPan;
NodeMap          mMap;
NodeMixer2       mMixer;

void setup() {
    Klang::connect(mOscLeft, Node::CH_OUT_SIGNAL, mMixer, NodeMixer2::CH_IN_SIGNAL_0);
    Klang::connect(mOscRight, Node::CH_OUT_SIGNAL, mMixer, NodeMixer2::CH_IN_SIGNAL_1);
    Klang::connect(mOscPan, Node::CH_OUT_SIGNAL, mMap, Node::CH_IN_SIGNAL);
    Klang::connect(mMap, Node::CH_OUT_SIGNAL, mMixer, NodeMixer2::CH_IN_MIX);
    Klang::connect(mMixer, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOscLeft.set_frequency(DEFAULT_FREQUENCY * 2.0);
    mOscLeft.set_amplitude(0.45);
    mOscLeft.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOscRight.set_frequency(DEFAULT_FREQUENCY * 2.05);
    mOscRight.set_amplitude(0.30);
    mOscRight.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);

    mMap.set_in_min(-1.0, 1.0, 0.25, 0.75);

    mOscPan.set_frequency(0.5);
    mOscPan.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
