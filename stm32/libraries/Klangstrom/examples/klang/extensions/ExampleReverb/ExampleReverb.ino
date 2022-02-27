/*
 * this example demonstrates how to apply a reverb effect to a signal.
 * note that `#define KLANG_EXT_NODE_REVERB` is required to use the effect.
 */

#define KLANG_EXT_NODE_REVERB

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeADSR         mADSR;
NodeReverb       mReverb;

uint8_t mFreq = 0;

void setup() {
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mReverb, Node::CH_IN_SIGNAL);
    Klang::connect(mReverb, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOsc.set_frequency(DEFAULT_FREQUENCY);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_KEY_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            event_pressed();
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_KEY_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            event_released();
            break;
        case EVENT_MOUSE_MOVED:
            mouse_moved(mouse_event(data).x, mouse_event(data).y);
            break;
    }
}

void mouse_moved(float x, float y) {
    mReverb.set_roomsize(x);
    mReverb.set_wet(y);
    mReverb.set_dry(1.0 - y);
}

void event_pressed() {
    mOsc.set_frequency((mFreq + 1) * DEFAULT_FREQUENCY);
    mADSR.start();
    mFreq++;
    mFreq %= 8;
}

void event_released() {
    mADSR.stop();
}
