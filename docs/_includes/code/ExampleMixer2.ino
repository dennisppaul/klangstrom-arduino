//
//  ExampleMixer2.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOscLeft;
NodeVCOWavetable    mOscRight;
NodeVCOWavetable    mOscPan;
NodeMap             mMap;
NodeMixer2          mMixer;

void setup()  {
    Klang::connect(mOscLeft,  Node::CH_OUT_SIGNAL, mMixer,    NodeMixer2::CH_IN_SIGNAL_LEFT);
    Klang::connect(mOscRight, Node::CH_OUT_SIGNAL, mMixer,    NodeMixer2::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mOscPan,   Node::CH_OUT_SIGNAL, mMap,      Node::CH_IN_SIGNAL);
    Klang::connect(mMap,      Node::CH_OUT_SIGNAL, mMixer,    NodeMixer2::CH_IN_MIX);
    Klang::connect(mMixer,    Node::CH_OUT_SIGNAL, mDAC,      NodeDAC::CH_IN_SIGNAL_LEFT);

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

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
