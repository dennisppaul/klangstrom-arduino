/*
 * this example demonstrates how to apply a phaser efffect to an input signal derived from line in ( e.g a microphone ).
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodePhaser mPhaser;
NodeADC    mADC;
NodeDAC    mDAC;

void setup() {
    Klang::lock();

    Klang::connect(mADC, NodeADC::CH_OUT_SIGNAL, mPhaser, Node::CH_IN_SIGNAL);
    Klang::connect(mPhaser, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    Klang::unlock();
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mADC.process_frame(pInputLeft, pInputRight);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    if (event == EVENT_MOUSE_MOVED) {
        mPhaser.set_wet(0.5);
        mPhaser.set_feedback(mouse_event(data).x * 2.0);
        mPhaser.set_sweeprate(mouse_event(data).y * 2.0);
    }
}
