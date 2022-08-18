/*
 * this example demonstrates how to apply a delay effect to a signal. the maximum delay line size can be defined with:
 *
 *    #define NODE_DELAY_BUFFER_SIZE 1024
 *
 * note that the value is defined in samples ( e.g 1 sec requires 48000 samples if audio rate is 48KHz )
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeADSR         mADSR;
NodeDelay        mDelay;

uint8_t mFrequencyMult = 0;

void setup() {
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDelay, Node::CH_IN_SIGNAL);
    Klang::connect(mDelay, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::EXPONENT);
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            mFrequencyMult++;
            mFrequencyMult %= 4;
            mOsc.set_frequency(DEFAULT_FREQUENCY * (mFrequencyMult + 1));
            mADSR.start();
            break;
        case EVENT_KEY_RELEASED:
            mADSR.stop();
            break;
        case EVENT_MOUSE_MOVED:
            mDelay.set_duration(0.5);
            mDelay.set_wet(0.5);
            mDelay.set_feedback(mouse_event(data).x);
            mDelay.set_filter(mouse_event(data).y);
            break;
        case EVENT_MOUSE_DRAGGED:
            break;
    }
}
