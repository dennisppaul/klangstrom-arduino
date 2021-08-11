//
//  ExampleADSRTriggered
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction  mTriggerRampFrequency;
NodeVCOFunction  mVCO;
NodeADSR         mADSR;
NodeDAC          mDAC;

void setup() {
    Klang::lock();

    Klang::connect(mTriggerRampFrequency, Node::CH_OUT_SIGNAL, mADSR, NodeADSR::CH_IN_TRIGGER);
    Klang::connect(mVCO,                  Node::CH_OUT_SIGNAL, mADSR, NodeADSR::CH_IN_SIGNAL);
    Klang::connect(mADSR,                 Node::CH_OUT_SIGNAL, mDAC,  NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);

    mTriggerRampFrequency.set_frequency(2);

    mADSR.set_attack_ms(0.5);
    mADSR.set_decay_ms(50);
    mADSR.set_sustain(0.5);
    mADSR.set_release_ms(10);

    Klang::unlock();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            break;
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mTriggerRampFrequency.set_frequency(data[X] * 12);
            mVCO.set_frequency(DEFAULT_FREQUENCY * (2 + data[Y]));
            break;
    }
}
