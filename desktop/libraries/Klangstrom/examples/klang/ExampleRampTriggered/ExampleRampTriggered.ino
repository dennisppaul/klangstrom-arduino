/*
    ExampleRamp

    the ramp’s duration, start and end value can be specified. however, the ramping process
    only starts when the `start()` function is called. if duration, start and end values
    are changed during ramping the ramping behavior changes. if the same values are changed
    while the ramp is not ramping no behavior change occurs until the `start()` function is
    called.
*/

#include "KlangNodes.hpp"
#include "CycleCounter.h"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeVCOFunction mTriggerRampAmplitude;
NodeVCOFunction mTriggerRampFrequency;
NodeRamp        mRampAmplitude;
NodeRamp        mRampFrequency;
NodeDAC         mDAC;
NodeVCFMoogLP   mFilter;

bool mToggleDirection     = false;
float mAudioblockDuration = 0;
uint8_t mBlocksUsed       = 0;

// duration of audioblock (μs) ........... : 540.29 (20210609 14:33:50)

void setup() {
    begin_serial_debug(true);

    Klang::lock();

    Klang::connect(mTriggerRampAmplitude, Node::CH_OUT_SIGNAL, mRampAmplitude, NodeRamp::CH_IN_TRIGGER);
    Klang::connect(mTriggerRampFrequency, Node::CH_OUT_SIGNAL, mRampFrequency, NodeRamp::CH_IN_TRIGGER);
    Klang::connect(mRampAmplitude,        Node::CH_OUT_SIGNAL, mVCO,           NodeVCOFunction::CH_IN_AMP);
    Klang::connect(mRampFrequency,        Node::CH_OUT_SIGNAL, mVCO,           NodeVCOFunction::CH_IN_FREQ);
    Klang::connect(mVCO,                  Node::CH_OUT_SIGNAL, mFilter,        Node::CH_IN_SIGNAL);
    Klang::connect(mFilter,               Node::CH_OUT_SIGNAL, mDAC,           NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(220);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);

    mTriggerRampAmplitude.set_frequency(1);
    mTriggerRampAmplitude.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    mTriggerRampFrequency.set_frequency(1);
    mTriggerRampFrequency.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

    mRampAmplitude.set_ramp(0.5, 0.0, 0.3);
    mRampFrequency.set_ramp(0.5, 22.5, 880.0);

    Klang::unlock();
}

void beat(uint32_t pBeat) {
    klangstrom::led_toggle(LED_00);
    Serial.print("duration of audioblock (μs) ........... : ");
    Serial.println(mAudioblockDuration);
    Serial.print("number of audioblock used ............. : ");
    Serial.println(mBlocksUsed);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    const uint32_t start = klst_get_cycles();
    mDAC.process_frame(pOutputLeft, pOutputRight);
    const uint32_t delta = klst_get_cycles() - start;
    mAudioblockDuration = klst_cyclesToMicros(delta);
    mBlocksUsed = AudioBlockPool::instance().blocks_used_max();
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
        case EVENT_MOUSE_MOVED:
            mTriggerRampAmplitude.set_frequency(data[X]*10);
            mTriggerRampFrequency.set_frequency(data[Y]*10);
            break;
        case EVENT_ENCODER_BUTTON_PRESSED:
            handleEncoderButtonsPressed(data[INDEX]);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            handleEncoderButtonsReleased(data[INDEX]);
            break;
    }
}

void handleEncoderButtonsPressed(uint8_t pIndex) {
    switch (pIndex) {
        case ENCODER_00:
            toggleDirection();
            startRamp();
            break;
        case ENCODER_01:
            startRamp();
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
            stopRamp();
            break;
        case ENCODER_02:
            break;
    }
}

void startRamp() {
    mRampAmplitude.start();
    mRampFrequency.start();
}

void stopRamp() {
    mRampAmplitude.stop();
}

void toggleDirection() {
    mToggleDirection = !mToggleDirection;
    led(LED_00, mToggleDirection);
    if (mToggleDirection) {
        mRampAmplitude.set_duration(0.5);
        mRampAmplitude.set_start(0.3);
        mRampAmplitude.set_destination(0.0);
    } else {
        mRampAmplitude.set_ramp(0.5, 0.0, 0.3);
    }
}
