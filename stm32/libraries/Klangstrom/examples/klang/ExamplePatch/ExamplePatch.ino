//
//  ExamplePatch
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class NodePatchFilterADSR : public NodePatch {
    public:
        NodeVCFMoogLHBP mFilter;
        NodeADSR        mADSR;

        NodePatchFilterADSR() {
            Klang::connect(input(), Node::CH_OUT_SIGNAL,    mFilter,    Node::CH_IN_SIGNAL);
            Klang::connect(mFilter, Node::CH_OUT_SIGNAL,    mADSR,      Node::CH_IN_SIGNAL);
            Klang::connect(mADSR,   Node::CH_OUT_SIGNAL,    output(),   Node::CH_IN_SIGNAL);

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
NodeVCOWavetable       mOsc;
NodePatchFilterADSR mPatch;

void setup() {
    Klang::connect(mOsc,    Node::CH_OUT_SIGNAL,  mPatch,   Node::CH_IN_SIGNAL);
    Klang::connect(mPatch,  Node::CH_OUT_SIGNAL,  mDAC,     NodeDAC::CH_IN_SIGNAL_LEFT);
    mPatch.setup(); // @TODO("get rid of this method call … but constructor does not work … yet")

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_KEY_PRESSED:
            mPatch.start();
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_KEY_RELEASED:
            mPatch.stop();
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mOsc.set_frequency(DEFAULT_FREQUENCY * (2 + data[X]));
            break;
    }
}
