//
//  ExampleMixer4.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOscA;
NodeVCOWavetable    mOscB;
NodeVCOWavetable    mOscC;
NodeVCOWavetable    mOscD;
NodeMixer4          mMixer;

void setup()  {
    Klang::connect(mOscA,     Node::CH_OUT_SIGNAL, mMixer,    NodeMixer4::CH_IN_SIGNAL_0);
    Klang::connect(mOscB,     Node::CH_OUT_SIGNAL, mMixer,    NodeMixer4::CH_IN_SIGNAL_1);
    Klang::connect(mOscC,     Node::CH_OUT_SIGNAL, mMixer,    NodeMixer4::CH_IN_SIGNAL_2);
    Klang::connect(mOscD,     Node::CH_OUT_SIGNAL, mMixer,    NodeMixer4::CH_IN_SIGNAL_3);
    Klang::connect(mMixer,    Node::CH_OUT_SIGNAL, mDAC,      NodeDAC::CH_IN_SIGNAL_LEFT);

    const float mOffset = 0.02;

    mOscA.set_frequency(DEFAULT_FREQUENCY * (4.0 + mOffset));
    mOscA.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOscB.set_frequency(DEFAULT_FREQUENCY * (2.0 + mOffset));
    mOscB.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOscC.set_frequency(DEFAULT_FREQUENCY * (1.0 + mOffset));
    mOscC.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOscD.set_frequency(DEFAULT_FREQUENCY * (0.5 + mOffset));
    mOscD.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_2, data[X]);
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_3, data[Y]);
            break;
        case EVENT_MOUSE_DRAGGED:
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_0, data[X]);
            mMixer.set_mix(NodeMixer4::SIGNAL_CHANNEL::SIGNAL_1, data[Y]);
            break;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
