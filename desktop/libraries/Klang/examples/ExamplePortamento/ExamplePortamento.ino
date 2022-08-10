/*
 * this example demonstrates how to apply a portamento to the frequency of an oscillator. instead of setting the
 * frequency directly, it is changed over time. the speed of the change is controlled by the portamento value set.
 * with the `set_value(value)` function.
 *
 * press the mouse to trigger a sound and change the speed of change by moving the mouse.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeADSR         mADSR;
NodePortamento   mPortamento;

uint8_t mFrequencyMult = 0;

void setup() {
    Klang::connect(mPortamento, NodePortamento::CH_OUT_VALUE, mOsc, NodeVCOWavetable::CH_IN_FREQ);
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::EXPONENT);

    mPortamento.set_value(DEFAULT_FREQUENCY);
    mPortamento.set_speed(1.0);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mPortamento.set_speed(mouse_event(data).x * 10);
            break;
        case EVENT_MOUSE_PRESSED:
            mFrequencyMult++;
            mFrequencyMult %= 6;
            mPortamento.set_value(DEFAULT_FREQUENCY * (mFrequencyMult + 1));
            mADSR.start();
            break;
        case EVENT_MOUSE_RELEASED:
            mADSR.stop();
            break;
    }
}
