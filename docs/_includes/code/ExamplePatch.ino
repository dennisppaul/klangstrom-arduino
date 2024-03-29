/*
 * this example demonstrates how to create a patch. patches are containers of nodes. they can be used to simplify
 * complex node structures. a patch has an input ( `input()` ) and an ouput channel ( `output()` ) that can be
 * connected to other nodes internally.
 *
 * in this example a filter and an envelope are combined in a patch. the output of an oscillator is connected to the
 * patch. use keyboard, mouse or encoder to start and stop the patch.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class NodePatchFilterADSR : public NodePatch {
public:
    NodeVCFMoogLHBP mFilter;
    NodeADSR        mADSR;

    NodePatchFilterADSR() {
        Klang::connect(input(), Node::CH_OUT_SIGNAL, mFilter, Node::CH_IN_SIGNAL);
        Klang::connect(mFilter, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
        Klang::connect(mADSR, Node::CH_OUT_SIGNAL, output(), Node::CH_IN_SIGNAL);

        mADSR.set_attack(0.01);
        mADSR.set_decay(0.05);
        mADSR.set_sustain(0.5);
        mADSR.set_release(0.25);

        mFilter.set_cutoff(0.1);
        mFilter.set_resonance(0.5);
    }

    void start() {
        mADSR.start();
    }

    void stop() {
        mADSR.stop();
    }
};

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;
NodePatchFilterADSR mPatch;

void setup() {
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mPatch, Node::CH_IN_SIGNAL);
    Klang::connect(mPatch, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
        case EVENT_MOUSE_PRESSED:
        case EVENT_KEY_PRESSED:
            mPatch.start();
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
        case EVENT_MOUSE_RELEASED:
        case EVENT_KEY_RELEASED:
            mPatch.stop();
            break;
    }
}
