/*
 * this example demonstrates how to us a Low Frequency Oscillator ( LFO ) to manipulte the amplitude of another
 * oscillator. note, how the `set_offset(offset)` method is used to change the output of the LFO from
 * (-1.0 ... 1.0) to (0.0 ... amplitude).
 */

#include "Klangstrom.h"
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
    Klang::connect(mVCA, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOsc.set_frequency(DEFAULT_FREQUENCY);

    mLFOAmplitude.set_frequency(1);
    mLFOAmplitude.set_amplitude(0.1);
    mLFOAmplitude.set_offset(0.05);
    mLFOAmplitude.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mLFOAmplitude.set_amplitude(mouse_event(data).y * 0.5);
            mLFOAmplitude.set_offset(mLFOAmplitude.get_amplitude());
            mLFOAmplitude.set_frequency(mouse_event(data).x * 20);
            break;
    }
}
