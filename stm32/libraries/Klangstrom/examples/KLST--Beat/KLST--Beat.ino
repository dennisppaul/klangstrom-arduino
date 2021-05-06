//
//  KLST--Beat
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

float mFreq = 110.0;
bool mLEDToggle = false;
uint32_t mBeatDuration = 0;
uint32_t mBeatDurationBase = 1000;
uint32_t mBeatDurationMax = 120000;
uint32_t mBeatDurationInc = 1000;

void setup() {
    Klang::lock();

    Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(mFreq);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

    Klang::unlock();

    klst::beats_per_minute(120);
}

void beat(uint32_t pBeat) {
    mLEDToggle = !mLEDToggle;
    klst::led(LED_01, mLEDToggle);
    mVCO.set_frequency(mFreq * ( mLEDToggle ? 1 : 2 ) - 110 * mBeatDuration / mBeatDurationMax);
    mBeatDuration += mBeatDurationInc;
    if (mBeatDuration > mBeatDurationMax) {
        mBeatDuration = 0;
    }
    klst::beats_per_minute_ms(mBeatDurationBase + mBeatDuration);
}

void loop() {
    delay(500);
    klst::led_toggle(LED_00);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
