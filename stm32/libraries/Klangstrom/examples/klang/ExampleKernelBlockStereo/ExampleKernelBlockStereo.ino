//
//  ExampleKernelBlockStereo
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class MNodeKernel : public NodeKernelBlockStereo {
public:
    float mix = 1.0;

    void kernel(SIGNAL_TYPE* pOutputSignal_A,
                SIGNAL_TYPE* pOutputSignal_B,
                SIGNAL_TYPE* pInputSignal_A,
                SIGNAL_TYPE* pInputSignal_B) {
        for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            pOutputSignal_A[i] = mix * pInputSignal_B[i] + (1.0 - mix) * pInputSignal_A[i];
            pOutputSignal_B[i] = mix * pInputSignal_A[i] + (1.0 - mix) * pInputSignal_B[i];
        }
    }
};

NodeDAC          mDAC;
NodeVCOWavetable mOsc_LEFT;
NodeVCOWavetable mOsc_RIGHT;
MNodeKernel      mKernel;

void setup() {
    Klang::connect(mOsc_LEFT, Node::CH_OUT_SIGNAL, mKernel, NodeKernelBlockStereo::CH_IN_SIGNAL_A);
    Klang::connect(mOsc_RIGHT, Node::CH_OUT_SIGNAL, mKernel, NodeKernelBlockStereo::CH_IN_SIGNAL_B);
    Klang::connect(mKernel, NodeKernelBlockStereo::CH_OUT_SIGNAL_A, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mKernel, NodeKernelBlockStereo::CH_OUT_SIGNAL_B, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mOsc_LEFT.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc_LEFT.set_amplitude(0.25);
    mOsc_LEFT.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOsc_RIGHT.set_frequency(DEFAULT_FREQUENCY);
    mOsc_RIGHT.set_amplitude(0.1);
    mOsc_RIGHT.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);

    mDAC.set_stereo(true);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    if (event == EVENT_MOUSE_MOVED) {
        mKernel.mix = data[Y];
    }
}