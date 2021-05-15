//
//  ExamplePortamento.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;
NodeADSR            mADSR;
NodePortamento      mPortamento;

uint8_t mFrequencyMult = 0;

void setup()  {
    Klang::connect(mPortamento,   NodePortamento::CH_OUT_VALUE, mOsc,   NodeVCO::CH_IN_FREQ);
    Klang::connect(mOsc,          Node::CH_OUT_SIGNAL,          mADSR,  Node::CH_IN_SIGNAL);
    Klang::connect(mADSR,         Node::CH_OUT_SIGNAL,          mDAC,   NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::EXPONENT);

    mPortamento.set_value(DEFAULT_FREQUENCY);
    mPortamento.set_speed(1.0);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mPortamento.set_speed(data[X] * 10);
            break;
        case EVENT_MOUSE_PRESSED:
            mFrequencyMult++;
            mFrequencyMult %= 6;
            mPortamento.set_value(DEFAULT_FREQUENCY * (mFrequencyMult + 1));
            mADSR.start();
            break;
        case EVENT_MOUSE_RELEASED:
            mADSR.stop();
            break;
    }
}
