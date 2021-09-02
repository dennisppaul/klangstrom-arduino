//
//  ExampleFFT
//

#include "CycleCounter.h"
float mAudioblockDuration = 0;

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeADSR        mADSR;
NodeDAC         mDAC;
NodeFFT         mFFT;

void setup() {
    Klang::lock();

    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mFFT, Node::CH_IN_SIGNAL);
    Klang::connect(mFFT, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(DEFAULT_FREQUENCY * 8);
    mVCO.set_amplitude(0.33);

    mADSR.set_attack(0.01);
    mADSR.set_decay(0.05);
    mADSR.set_sustain(0.5);
    mADSR.set_release(0.25);

    mFFT.enable_hamming_window(false);
    // mFFT.enable_inline_analysis(false);

    Klang::unlock();
}

void beat(uint32_t pBeat) {
    // mFFT.perform_analysis(); // perform analysis manually if inline analysis is disabled
    Serial.print("frequency ................ : ");
    Serial.print(mFFT.get_frequency());
    Serial.print(" > ");
    Serial.print(mFFT.get_frequency_gaussian_interpolation());
    Serial.print(" > ");
    Serial.print(mVCO.get_frequency());
    Serial.print(mFFT.is_hamming_window_enabled() ? "*" : "");
    Serial.println();

    Serial.print("duration of audioblock (μs) ........... : ");
    Serial.println(mAudioblockDuration);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    const uint32_t start = klst_get_cycles();
    mDAC.process_frame(pOutputLeft, pOutputRight);
    const uint32_t delta = klst_get_cycles() - start;
    mAudioblockDuration  = klst_cyclesToMicros(delta);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            mFFT.enable_hamming_window(data[INDEX] != ENCODER_00);
            mADSR.start();
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            mADSR.stop();
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mVCO.set_frequency(DEFAULT_FREQUENCY * (2 + data[X]));
            mVCO.set_amplitude(0.5 * data[Y]);
            break;
        case EVENT_ENCODER_ROTATE:
            const float mDelta = data[TICK] - data[PREVIOUS_TICK];
            if (data[INDEX] == ENCODER_00) {
                mVCO.set_frequency(mVCO.get_frequency() + mDelta);
            } else if (data[INDEX] == ENCODER_01) {
                const float mAmp = fmin(1.0, fmax(0.0, mVCO.get_amplitude()));
                mVCO.set_amplitude(mAmp + mDelta * 0.05);
            }
            break;
    }
}
