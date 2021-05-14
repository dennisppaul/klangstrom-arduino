//
//  ExampleMute.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;
NodeMute            mMute;

void setup()  {
    Klang::connect(mOsc,    Node::CH_OUT_SIGNAL,  mMute,   Node::CH_IN_SIGNAL);
    Klang::connect(mMute,   Node::CH_OUT_SIGNAL,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.33);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mMute.fill_muted_audioblock_with_zeros(true);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_KEY_PRESSED:
            mMute.set_mute(true);
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_KEY_RELEASED:
            mMute.set_mute(false);
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mOsc.set_frequency(DEFAULT_FREQUENCY * floor(1 + data[X] * 8));
            break;

    }
}
