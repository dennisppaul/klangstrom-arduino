/*
 * this example demonstrates how to trigger an ADSR envelope from a signale ( in this example the signal is generated
 * by a second oscillator ). whenever the signal passes a threshold ( default is `0.0` ) on a *rising edge* the ADSR
 * will be started and when the signal passes a threshold on a *falling edge* the ADSR will be stopped.
 *
 * rotate the encoder to change the trigger frequency.
 */

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mTriggerRampFrequency;
NodeVCOFunction mVCO;
NodeADSR        mADSR;
NodeDAC         mDAC;

void setup() {
    Klang::lock();

    Klang::connect(mTriggerRampFrequency, Node::CH_OUT_SIGNAL, mADSR, NodeADSR::CH_IN_TRIGGER);
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mADSR, NodeADSR::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mVCO.set_frequency(DEFAULT_FREQUENCY * 3);
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
        case EVENT_ENCODER_ROTATED:
            mTriggerRampFrequency.set_frequency(mTriggerRampFrequency.get_frequency() + encoder_event(data).delta);
            break;
        case EVENT_MOUSE_MOVED:
            mTriggerRampFrequency.set_frequency(mouse_event(data).x * 20);
            break;
    }
}
