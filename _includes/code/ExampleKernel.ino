/*
 * this example demonstrates how to use the `NodeKernel` class to write custom nodes. it supplies all the necessary
 * node infrastructure. only the `float NodeKernel::kernel(float)` method needs to implemented. in this
 * example the input signal is amplified an clamped to create a simple distortion effect.
 * move the mouse to change amplitude and frequency of the oscillator.
 *
 * note that there are a number of different kernel nodes for different input and output configurations:
 *
 * - `NodeKernel` :: processes a signal stream sample-by-sample
 * - `NodeKernelBlock` :: processes a block of samples
 * - `NodeKernelBlock2` :: processes samples from two input signal streams and outputs a single signal stream
 * - `NodeKernelBlockStereo` :: processes samples from two input signal streams into two output signal streams
 * - `NodeKernelBlockMulti` :: processes samples from up to 256 input signal streams into up to 256 output signal streams
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class MNodeKernel : public NodeKernel {
public:
    float       amplitude = 1.0;
    const float clamp     = 0.4;

    float kernel(float s) {
        float t = s * amplitude;
        return t > clamp ? clamp : (t < -clamp ? -clamp : t);
    }
};

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
MNodeKernel      mKernel;

void setup() {
    Klang::connect(mOsc, mKernel);
    Klang::connect(mKernel, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    if (event == EVENT_MOUSE_MOVED) {
        mKernel.amplitude = (1.0 - mouse_event(data).y) * 4.0;
        mOsc.set_frequency(DEFAULT_FREQUENCY * 2 * mouse_event(data).x);
    }
}
