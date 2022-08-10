/*
 * this example is a simpler demonstration of an envelope. in this example, the envelope controls the frequency of an
 * oscillator while the amplitude is controlled by an ADSR ( which is a special type of envelope ).
 *
 * for an extensive demonstration of envelopes see `ExampleEnvelope`.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeEnvelope     mEnvelopeFreq;
NodeADSR         mADSR;
NodeVCOWavetable mOsc;
NodeDAC          mDAC;

void setup() {
    Klang::connect(mEnvelopeFreq, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_FREQ);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_AMP);
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mEnvelopeFreq.set_start_value(55.0);
    mEnvelopeFreq.add_stage(0.2, 110.0);
    mEnvelopeFreq.add_stage(0.2, 880.0);
    mEnvelopeFreq.add_stage(0.25, 110.0);
    mEnvelopeFreq.add_stage(0.1, 55.0);

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
            mEnvelopeFreq.start();
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            mADSR.stop();
            break;
    }
}
