#include "Nodes.hpp"
#include "606bd1.wav.h"
#include "606ch.wav.h"
#include "606sd1.wav.h"

using namespace std;
using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeMixer2          mMixerMaster;
NodeMixer2          mMixer1;
NodeMixer2          mMixer2;
NodeSampler         mSampleBass(_606bd1_data, _606bd1_data_length);
NodeSampler         mSampleHihat(_606ch_data, _606ch_data_length);
NodeSampler         mSampleSnare(_606sd1_data, _606sd1_data_length);
NodeValue           mZero;
NodeVCA             mVCA;

float mBPM;
float mFilterCutoff;
float mFilterResonance;
float mAmplification;
bool  mButton01Shift;
bool  mButton02Shift;

uint8_t mPatternID    = 0;
uint8_t mBassPattern  = 0;
uint8_t mSnarePattern = 0;
uint8_t mHihatPattern = 0;

void setup() {
    Klang::lock();

    Klang::connect(mSampleSnare,   Node::CH_OUT_SIGNAL, mMixer1,      NodeMixer2::CH_IN_SIGNAL_LEFT);
    Klang::connect(mZero,          Node::CH_OUT_SIGNAL, mMixer1,      NodeMixer2::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mSampleHihat,   Node::CH_OUT_SIGNAL, mMixer2,      NodeMixer2::CH_IN_SIGNAL_LEFT);
    Klang::connect(mSampleBass,    Node::CH_OUT_SIGNAL, mMixer2,      NodeMixer2::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mMixer1,        Node::CH_OUT_SIGNAL, mMixerMaster, NodeMixer2::CH_IN_SIGNAL_LEFT);
    Klang::connect(mMixer2,        Node::CH_OUT_SIGNAL, mMixerMaster, NodeMixer2::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mMixerMaster,   Node::CH_OUT_SIGNAL, mVCA,         Node::CH_IN_SIGNAL);
    Klang::connect(mVCA,           Node::CH_OUT_SIGNAL, mDAC,         NodeDAC::CH_IN_SIGNAL_LEFT);

    mSampleBass.loop(false);
    mSampleHihat.loop(false);
    mSampleSnare.loop(false);

    set_defaults();
    
    Klang::unlock();
}

void loop() {
    delay(50);
}

void set_defaults() {
    mAmplification    = 6.0;
    mBPM              = 120 * 2;
    mButton01Shift    = false;
    mButton02Shift    = false;
    mPatternID    = 0;
    mBassPattern  = 1;
    mSnarePattern = 0;
    mHihatPattern = 0;
    beats_per_minute(mBPM);
    led(LED_01, mButton01Shift);
    led(LED_02, mButton02Shift);
    mVCA.set_amplification(mAmplification);
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_00:
            if (data[BUTTON_STATE]) {
                set_defaults();
            }
            break;
        case EVENT_ENCODER_BUTTON_01:
            if (data[BUTTON_STATE]) {
                mButton01Shift = !mButton01Shift;
            }
            led(LED_01, mButton01Shift);
            break;
        case EVENT_ENCODER_BUTTON_02:
            if (data[BUTTON_STATE]) {
                mButton02Shift = !mButton02Shift;
            }
            led(LED_02, mButton02Shift);
            break;
        case EVENT_ENCODER_ROTATE_00:
                mBassPattern = map_tick(data[TICK]);
            break;
        case EVENT_ENCODER_ROTATE_01:
            if (mButton01Shift) {
                handle_change_amp(data[TICK] - data[PREVIOUS_TICK]);
            } else {
                mSnarePattern = map_tick(data[TICK]);
            }
            break;
        case EVENT_ENCODER_ROTATE_02:
            if (mButton02Shift) {
                handle_change_bpm(data[TICK] - data[PREVIOUS_TICK]);
            } else {
                mHihatPattern = map_tick(data[TICK]);
            }
            break;
    }
}

uint8_t map_tick(const float pTick) {
    const uint16_t mTick = (int16_t)pTick;
   return ( mTick / 4) % 256;
}

void handle_change_bpm(float mEncoderChange) {
    mBPM += mEncoderChange * 0.5;
    beats_per_minute(mBPM);
}

void handle_change_amp(float mEncoderChange) {
    mAmplification += mEncoderChange * 0.1;
    mAmplification = KlangMath::clamp(mAmplification, 0.0, 12.0);
    mVCA.set_amplification(mAmplification);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void beat(uint32_t pBeat) {
    led_toggle(LED_00);
    mPatternID++;
    mPatternID %= 8;
    if (trigger(mPatternID, mBassPattern)) {
        mSampleBass.trigger();
    }
    if (trigger(mPatternID, mSnarePattern)) {
        mSampleSnare.trigger();
    }
    if (trigger(mPatternID, mHihatPattern)) {
        mSampleHihat.trigger();
    }
}

bool trigger(uint8_t pIndex, uint8_t pPattern) {
    const uint8_t mMask = (uint8_t)(1 << pIndex);
    return ( mMask & pPattern ) > 0;
}
