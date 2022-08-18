/*
 * this example demonstrates how to mix 4 signals into two output signals ( i.e stereo output ). in addition to the
 * mix paramter ( i.e `set_mix(channel, value)` ) each input signal or channel can also be panned across the two output
 * signal via the `set_pan(channel, value)` method.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOscA;
NodeVCOWavetable mOscB;
NodeVCOWavetable mOscC;
NodeVCOWavetable mOscD;
NodeMixer4Stereo mMixer;

void setup() {
    Klang::connect(mOscA, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_0);
    Klang::connect(mOscB, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_1);
    Klang::connect(mOscC, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_2);
    Klang::connect(mOscD, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_3);
    Klang::connect(mMixer, NodeMixer4Stereo::CH_OUT_SIGNAL_LEFT, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mMixer, NodeMixer4Stereo::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    const float mOffset = 0.04;
    mOscA.set_frequency(DEFAULT_FREQUENCY * (4.0 + mOffset));
    mOscA.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscA.set_amplitude(0.2);
    mOscB.set_frequency(DEFAULT_FREQUENCY * (2.0 + mOffset));
    mOscB.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscB.set_amplitude(0.4);
    mOscC.set_frequency(DEFAULT_FREQUENCY * (1.0 + mOffset));
    mOscC.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscC.set_amplitude(0.6);
    mOscD.set_frequency(DEFAULT_FREQUENCY * (0.5 + mOffset));
    mOscD.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscD.set_amplitude(0.8);

    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_0, 0.4);
    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_1, 0.55);
    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_2, 0.8);
    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_3, 1.0);

    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_0, -1.0);
    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_1, -0.5);
    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_2, 0.0);
    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_3, 1.0);

    mDAC.set_stereo(true);
}

void loop() {}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_0, mouse_event(data).x * 2 - 1);
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_1, mouse_event(data).y * 2 - 1);
            break;
        case EVENT_MOUSE_DRAGGED:
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_2, mouse_event(data).x * 2 - 1);
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_3, mouse_event(data).y * 2 - 1);
            break;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
