/*
 * this example demonstrates how to use a ramp to interpolate from a start value to an end value over time in a
 * specificed duration.
 *
 * the ramp’s duration, start and end value can be specified. however, the ramping process only starts when the
 *`start()` function is called. if duration, start and end values are changed during ramping the ramping behavior
 * changes. if the same values are changed while the ramp is not ramping no behavior change occurs until the `start()`
 * function is called.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeRamp        mRampAmplitude;
NodeRamp        mRampFrequency;
NodeDAC         mDAC;

bool mToggleDirection = false;

void setup() {
    Klang::lock();

    Klang::connect(mRampAmplitude, Node::CH_OUT_SIGNAL, mVCO, NodeVCOFunction::CH_IN_AMP);
    Klang::connect(mRampFrequency, Node::CH_OUT_SIGNAL, mVCO, NodeVCOFunction::CH_IN_FREQ);
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(220);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

    mRampAmplitude.set_ramp(2.0, 0.0, 0.5);
    mRampFrequency.set_ramp(1.0, 55.0, 440.0);

    Klang::unlock();
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void startRamps() {
    mRampFrequency.set_current_value(55.0);
    mRampFrequency.start();
    mRampAmplitude.start();
}

void stopRamps() {
    mRampFrequency.stop();
    mRampAmplitude.stop();
}

void toggleDirection() {
    mToggleDirection = !mToggleDirection;
    led(LED_00, mToggleDirection);
    if (mToggleDirection) {
        mRampAmplitude.set_duration(2.0);
        mRampAmplitude.set_start(0.5);
        mRampAmplitude.set_destination(0.0);
    } else {
        mRampAmplitude.set_ramp(2.0, 0.0, 0.5);
    }
}

void handleEncoderButtonsPressed(uint8_t pIndex) {
    switch (pIndex) {
        case ENCODER_00:
            toggleDirection();
            startRamps();
            break;
        case ENCODER_01:
            startRamps();
            break;
        case ENCODER_02:
            toggleDirection();
            break;
    }
}

void handleEncoderButtonsReleased(uint8_t pIndex) {
    switch (pIndex) {
        case ENCODER_00:
            break;
        case ENCODER_01:
            stopRamps();
            break;
        case ENCODER_02:
            break;
    }
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            toggleDirection();
            break;
        case EVENT_MOUSE_PRESSED:
            startRamps();
            break;
        case EVENT_MOUSE_RELEASED:
            stopRamps();
            break;
        case EVENT_ENCODER_BUTTON_PRESSED:
            handleEncoderButtonsPressed(encoder_event(data).index);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            handleEncoderButtonsReleased(encoder_event(data).index);
            break;
    }
}
