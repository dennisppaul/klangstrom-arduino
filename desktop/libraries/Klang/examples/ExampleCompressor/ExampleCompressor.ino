/*
 * this example demonstrates how to apply a compressor effect to a signal.
 *
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeADSR        mADSR;
NodeDelay       mDelay;
NodeCompressor  mCompressor;
NodeDAC         mDAC;

bool mADSRToggle = false;

void setup() {
    Klang::lock();

    Klang::connect(mVCO, mADSR);
    Klang::connect(mADSR, mDelay);
    Klang::connect(mDelay, mCompressor);
    Klang::connect(mCompressor, mDAC);

    mVCO.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);
    mVCO.set_amplitude(0.5);

    mADSR.set_attack(0.01);
    mADSR.set_decay(0.05);
    mADSR.set_sustain(0.5);
    mADSR.set_release(0.25);

    mDelay.set_duration(0.47);
    mDelay.set_wet(0.6);
    mDelay.set_feedback(0.85);
    mDelay.set_filter(0.6);

    Klang::unlock();

    beats_per_minute(480);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void beat(uint32_t pBeat) {
    mVCO.set_frequency(DEFAULT_FREQUENCY * (pBeat % 3 + 1));
    mADSRToggle = !mADSRToggle;
    if (mADSRToggle) {
        mADSR.start();
    } else {
        mADSR.stop();
    }
}

void handle_keyboard_event(char key) {
    switch (key) {
        case '1':
            mCompressor.set_preset(NodeCompressor::PRESET_2_TO_1);
            break;
        case '2':
            mCompressor.set_preset(NodeCompressor::PRESET_4_TO_1);
            break;
        case '3':
            mCompressor.set_preset(NodeCompressor::PRESET_8_TO_1);
            break;
        case '4':
            mCompressor.set_preset(NodeCompressor::PRESET_FINAL_LIMITER);
            break;
        case '5':
            mCompressor.set_preset(NodeCompressor::PRESET_HARMONIC_ENHANCER);
            break;
        case '6':
            mCompressor.set_preset(NodeCompressor::PRESET_BAND_COMP_BAND);
            break;
        case '7':
            mCompressor.set_preset(NodeCompressor::PRESET_END_COMP_BAND);
            break;
    }
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            handle_keyboard_event(keyboard_event(data).key);
            break;
        case EVENT_MOUSE_MOVED:
            mCompressor.set_parameter(NodeCompressor::PARAM_RATIO, (int)(mouse_event(data).x * 14.0) + 2);
            mCompressor.set_parameter(NodeCompressor::PARAM_REL, (int)(mouse_event(data).y * 400.0));
            break;
    }
}
