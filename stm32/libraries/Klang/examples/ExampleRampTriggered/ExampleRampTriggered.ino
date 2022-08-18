/*
 * this example demonstrates how to use a ramp to interpolate from a start value to an end value over time in a
 * specificed duration.
 *
 * in this example the ramp is started and stopped by a trigger signal produced by an oscillator. whenever the
 * trigger signal passes a threshold, the ramp is started or stopped depending on the direction ( i.e rising edge or
 * falling edge ).
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeVCOFunction mTrigger;
NodeRamp        mRampFrequency;
NodeRamp        mRampAmplitude;
NodeDAC         mDAC;
NodeVCFMoogLP   mFilter;
NodeSplitter    mSplitter;

bool mToggleDirection = true;

void setup() {
    Klang::lock();

    Klang::connect(mTrigger, Node::CH_OUT_SIGNAL, mSplitter, Node::CH_IN_SIGNAL);
    Klang::connect(mSplitter, Node::CH_OUT_SIGNAL, mRampAmplitude, NodeRamp::CH_IN_TRIGGER);
    Klang::connect(mSplitter, Node::CH_OUT_SIGNAL, mRampFrequency, NodeRamp::CH_IN_TRIGGER);
    Klang::connect(mRampFrequency, Node::CH_OUT_SIGNAL, mVCO, NodeVCOFunction::CH_IN_FREQ);
    Klang::connect(mRampAmplitude, Node::CH_OUT_SIGNAL, mVCO, NodeVCOFunction::CH_IN_AMP);
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mFilter, Node::CH_IN_SIGNAL);
    Klang::connect(mFilter, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mVCO.set_frequency(220);
    mVCO.set_amplitude(0.25);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);

    mTrigger.set_frequency(1);
    mTrigger.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

    mRampFrequency.set_ramp(0.25, 27.5, 880.0);
    mRampAmplitude.set_ramp(0.25, 0.5, 0.0);

    Klang::unlock();
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void toggleDirection() {
    mToggleDirection = !mToggleDirection;
    led(LED_00, mToggleDirection);
    if (mToggleDirection) {
        mRampFrequency.set_duration(0.25);
        mRampFrequency.set_start(880);
        mRampFrequency.set_destination(27.5);
    } else {
        mRampFrequency.set_ramp(0.25, 27.5, 880.0);
    }
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            toggleDirection();
            mRampAmplitude.start();
            break;
        case EVENT_MOUSE_RELEASED:
            mRampAmplitude.stop();
            break;
        case EVENT_MOUSE_MOVED:
            mTrigger.set_frequency(mouse_event(data).y * 5.0);
            break;
    }
}