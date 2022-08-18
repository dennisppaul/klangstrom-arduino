/*
 * this example demonstrates how to mix 4 signals into a single signal. the `set_mix(channel, value)` method sets the
 * mix value where a value of `0.0` means the signal will not be mixed and a value of `1.0` means the signal is fully
 * mixed into the output.
 *
 * note that the output is not averaged i.e if e.g four signals of value `0.7`, `0.6`, `1.0` and `0.9` are mixed, the
 * output will be `3.2` and not `0.8`.
 *
 * the mouse position changes the mix value of the signals.
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
NodeMixer4       mMixer;

void setup() {
    Klang::connect(mOscA, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_0);
    Klang::connect(mOscB, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_1);
    Klang::connect(mOscC, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_2);
    Klang::connect(mOscD, Node::CH_OUT_SIGNAL, mMixer, NodeMixer4::CH_IN_SIGNAL_3);
    Klang::connect(mMixer, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_0, 0.2);
    mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_1, 0.2);
    mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_2, 0.2);
    mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_3, 0.2);

    const float mOffset = 0.02;

    mOscA.set_frequency(DEFAULT_FREQUENCY * (4.0 + mOffset));
    mOscA.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOscB.set_frequency(DEFAULT_FREQUENCY * (2.0 + mOffset));
    mOscB.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOscC.set_frequency(DEFAULT_FREQUENCY * (1.0 + mOffset));
    mOscC.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOscD.set_frequency(DEFAULT_FREQUENCY * (8.0 + mOffset));
    mOscD.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void loop() {}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_2, mouse_event(data).x * 0.2);
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_3, mouse_event(data).y * 0.2);
            break;
        case EVENT_MOUSE_DRAGGED:
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_0, mouse_event(data).x * 0.2);
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_1, mouse_event(data).y * 0.2);
            break;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
