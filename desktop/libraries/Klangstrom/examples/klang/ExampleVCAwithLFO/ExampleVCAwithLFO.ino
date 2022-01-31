//
//  ExampleVCAwithLFO
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeVCOWavetable mLFOAmplitude;
NodeVCA          mVCA;

void setup() {
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mVCA, Node::CH_IN_SIGNAL);
    Klang::connect(mLFOAmplitude, Node::CH_OUT_SIGNAL, mVCA, NodeVCA::CH_IN_AMPLIFICATION);
    Klang::connect(mVCA, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mLFOAmplitude.set_frequency(1);
    mLFOAmplitude.set_amplitude(0.1);
    mLFOAmplitude.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mLFOAmplitude.set_offset(0.5);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mLFOAmplitude.set_amplitude(data[Y] * 0.5);
            break;
        case EVENT_MOUSE_DRAGGED:
            mLFOAmplitude.set_frequency(data[X] * 20);
            break;
    }
}
