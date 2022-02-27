/*
 *
 */

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOWavetable mTriggerRampFrequency;
NodeEnvelope     mEnvelopeFreq;
NodeADSR         mADSR;
NodeVCOWavetable mOsc;
NodeDAC          mDAC;

void setup() {
    Klang::connect(mTriggerRampFrequency, Node::CH_OUT_SIGNAL, mEnvelopeFreq, NodeEnvelope::CH_IN_TRIGGER);
    Klang::connect(mEnvelopeFreq, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_FREQ);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_AMP);
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mEnvelopeFreq.set_start_value(1.0);
    mEnvelopeFreq.add_stage(0.025, 2.0);
    mEnvelopeFreq.add_stage(0.05, 16.0);
    mEnvelopeFreq.add_stage(0.025, 8.0);
    mEnvelopeFreq.set_value_scale(55.0);

    mTriggerRampFrequency.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
    mTriggerRampFrequency.set_frequency(2);

    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
    mOsc.set_frequency(55);
    mOsc.set_amplitude(0.33);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            mADSR.start();
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            mADSR.stop();
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mTriggerRampFrequency.set_frequency(data[X] * 12);
            mEnvelopeFreq.set_time_scale(data[Y] * 2);
            break;
    }
}
