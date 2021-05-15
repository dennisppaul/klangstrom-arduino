//
//  ExamplePatch16
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

/**
 *       [ NODE_PATCH_FILTER_ADSR ]
 *       +------------------------+
 *       |                        |
 * IN00--| OSC_LEFT        SIGNAL |--OUT00
 * IN01--| OSC_RIGHT              |
 *       |                        |
 *       +------------------------+
 */
class MNodePatchFilterADSR : public NodePatch16 {
    public:
        NodeVCFMoogLHBP mFilter;
        NodeADSR        mADSR;
        NodeMixer2      mMixer;

        MNodePatchFilterADSR() {
            Klang::connect(input(0),    Node::CH_OUT_SIGNAL,    mMixer,     NodeMixer2::CH_IN_SIGNAL_LEFT);
            Klang::connect(input(1),    Node::CH_OUT_SIGNAL,    mMixer,     NodeMixer2::CH_IN_SIGNAL_RIGHT);
            Klang::connect(mMixer,      Node::CH_OUT_SIGNAL,    mFilter,    Node::CH_IN_SIGNAL);
            Klang::connect(mFilter,     Node::CH_OUT_SIGNAL,    mADSR,      Node::CH_IN_SIGNAL);
            Klang::connect(mADSR,       Node::CH_OUT_SIGNAL,    output(0),  Node::CH_IN_SIGNAL);

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

NodeDAC                 mDAC;
NodeVCOWavetable           mOscLeft;
NodeVCOWavetable           mOscRight;
MNodePatchFilterADSR    mPatch;

void setup() {
    Klang::connect(mOscLeft,            Node::CH_OUT_SIGNAL,  mPatch.input(0),  Node::CH_IN_SIGNAL);
    Klang::connect(mOscRight,           Node::CH_OUT_SIGNAL,  mPatch.input(1),  Node::CH_IN_SIGNAL);
    Klang::connect(mPatch.output(0),    Node::CH_OUT_SIGNAL,  mDAC,             NodeDAC::CH_IN_SIGNAL_LEFT);

    mOscLeft.set_frequency(DEFAULT_FREQUENCY * 2);
    mOscLeft.set_amplitude(0.5);
    mOscLeft.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);

    mOscRight.set_frequency(DEFAULT_FREQUENCY * 2);
    mOscRight.set_amplitude(0.5);
    mOscRight.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
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
            mOscLeft.set_frequency(DEFAULT_FREQUENCY * (2 + data[X]));
            break;
    }
}
