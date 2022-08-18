/*
 * this example demonstrates how to use the `NodeKernelBlockStereo` class to write a custom node with three inputs and
 * two outputs.
 * 
 * move the mouse to change the mix ratio for left and right signal.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class MNodeKernel : public NodeKernelBlockStereo {
public:
    float mix = 1.0;

    void kernel(SIGNAL_TYPE* pOutputSignal_LEFT,
                SIGNAL_TYPE* pOutputSignal_RIGHT,
                SIGNAL_TYPE* pInputSignal_A,
                SIGNAL_TYPE* pInputSignal_B) {
        for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            pOutputSignal_LEFT[i]  = mix * pInputSignal_B[i] + (1.0 - mix) * pInputSignal_A[i];
            pOutputSignal_RIGHT[i] = mix * pInputSignal_A[i] + (1.0 - mix) * pInputSignal_B[i];
        }
    }
};

NodeDAC          mDAC;
NodeVCOWavetable mOsc_A;
NodeVCOWavetable mOsc_B;
MNodeKernel      mKernel;

void setup() {
    Klang::connect(mOsc_A, Node::CH_OUT_SIGNAL, mKernel, NodeKernelBlockStereo::CH_IN_SIGNAL_LEFT);
    Klang::connect(mOsc_B, Node::CH_OUT_SIGNAL, mKernel, NodeKernelBlockStereo::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mKernel, NodeKernelBlockStereo::CH_OUT_SIGNAL_LEFT, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mKernel, NodeKernelBlockStereo::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mOsc_A.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc_A.set_amplitude(0.25);
    mOsc_A.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mOsc_B.set_frequency(DEFAULT_FREQUENCY);
    mOsc_B.set_amplitude(0.1);
    mOsc_B.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);

    mDAC.set_stereo(true);
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    if (event == EVENT_MOUSE_MOVED) {
        mKernel.mix = mouse_event(data).y;
    }
}