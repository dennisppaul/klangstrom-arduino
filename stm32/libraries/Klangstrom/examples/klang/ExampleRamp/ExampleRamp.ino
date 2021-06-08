/*
    ExampleRamp

    the ramp’s duration, start and end value can be specified. however, the ramping process
    only starts when the `start()` function is called. if duration, start and end values
    are changed during ramping the ramping behavior changes. if the same values are changed
    while the ramp is not ramping no behavior change occurs until the `start()` function is
    called.
*/

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeRamp        mRampAmplitude;
NodeRamp        mRampFrequency;
NodeDAC         mDAC;

bool mToggleDirection = false;

void setup() {
    Klang::lock();

    Klang::connect(mRampAmplitude,  Node::CH_OUT_SIGNAL,  mVCO,  NodeVCOFunction::CH_IN_AMP);
    Klang::connect(mRampFrequency,  Node::CH_OUT_SIGNAL,  mVCO,  NodeVCOFunction::CH_IN_FREQ);
    Klang::connect(mVCO,            Node::CH_OUT_SIGNAL,  mDAC,  NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(220);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

    mRampAmplitude.set_ramp(2.0, 0.0, 0.5);
    mRampFrequency.set_ramp(1.0, 55.0, 440.0);

    Klang::unlock();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            toggleDirection();
            break;
        case EVENT_MOUSE_PRESSED:
            startRamp();
            break;
        case EVENT_MOUSE_RELEASED:
            stopRamp();
            break;
        case EVENT_ENCODER_BUTTON_PRESSED:
            handleEncoderButtons(data[INDEX]);
            break;
    }
}

void handleEncoderButtons(uint8_t pIndex) {
    switch (pIndex) {
        case ENCODER_00:
            toggleDirection();
            break;
        case ENCODER_01:
            startRamp();
            break;
        case ENCODER_02:
            stopRamp();
            break;
    }
}

void startRamp() {
    mRampAmplitude.start();
    mRampFrequency.set_current_value(55.0);
    mRampFrequency.start();
}

void stopRamp() {
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
