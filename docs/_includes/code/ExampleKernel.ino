//
//  ExampleKernel
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class MNodeKernel : public NodeKernel {
public:
    float       amplitude = 1.0;
    const float clamp     = 0.4;

    SIGNAL_TYPE kernel(SIGNAL_TYPE s) {
        SIGNAL_TYPE t = s * amplitude;
        return t > clamp ? clamp : (t < -clamp ? -clamp : t);
    }
};

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
MNodeKernel      mKernel;

void setup() {
    Klang::connect(mOsc, mKernel);
    Klang::connect(mKernel, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    if (event == EVENT_MOUSE_MOVED) {
        mKernel.amplitude = (1.0 - data[Y]) * 4.0;
        mOsc.set_frequency(DEFAULT_FREQUENCY * 2 * data[X]);
    }
}
