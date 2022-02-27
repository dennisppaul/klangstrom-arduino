/*
 * this example demonstrates how to use the `NodeKernelBlockMulti` class to write a custom node with two inputs and
 * two outputs.
 *
 * move the mouse to change the mix ratio for the three input channels.
 */

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class MNodeKernel : public NodeKernelBlockMulti {
public:
    static const CHANNEL_ID CH_OUT_SIGNAL_LEFT  = 0x00;
    static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 0x01;

    float mix_x = 1.0;
    float mix_y = 1.0;

    void kernel(vector<SIGNAL_TYPE*>& pOutputSignals, vector<SIGNAL_TYPE*>& pInputSignals) {
        for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            pOutputSignals[CH_OUT_SIGNAL_LEFT][i] =
                pInputSignals[0][i] * 0.5 +
                pInputSignals[1][i] * mix_x +
                pInputSignals[2][i] * (1.0 - mix_y);
            pOutputSignals[CH_OUT_SIGNAL_RIGHT][i] =
                pInputSignals[0][i] * 0.5 +
                pInputSignals[1][i] * (1.0 - mix_x) +
                pInputSignals[2][i] * mix_y;
        }
    }
};

static const uint8_t VCO_COUNT = 3;
NodeVCOWavetable     mVCOs[VCO_COUNT];
NodeDAC              mDAC;
MNodeKernel          mKernel;

void setup() {
    mKernel.set_num_output_channels(2);
    mDAC.set_stereo(true);
    Klang::connect(mKernel, MNodeKernel::CH_OUT_SIGNAL_LEFT, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mKernel, MNodeKernel::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    /* setup and patch VCOs */
    for (uint8_t i = 0; i < VCO_COUNT; i++) {
        mVCOs[i].set_frequency(DEFAULT_FREQUENCY * (i + 1));
        mVCOs[i].set_amplitude(0.1 + 0.5 * (VCO_COUNT - i) / VCO_COUNT); /* boost lower frequencies */
        mVCOs[i].set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
        Klang::connect(mVCOs[i], Node::CH_OUT_SIGNAL, mKernel, i);
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    if (event == EVENT_MOUSE_MOVED) {
        mKernel.mix_x = mouse_event(data).x;
        mKernel.mix_y = mouse_event(data).y;
    }
}