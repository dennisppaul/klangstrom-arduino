#define KLANG_EXT_NODE_REVERB
#include "KlangNodes.hpp"
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
NodeDistortion      mDistortion;
#ifdef KLANG_EXT_NODE_REVERB
NodeReverb          mReverb;
#endif

const uint8_t TRACK_LENGTH = 16;

float mBPM;
float mFilterCutoff;
float mFilterResonance;
float mAmplification;
float mDistortionAmount;

array<uint8_t, TRACK_LENGTH> mTrackBass {
    1, 0, 0, 0,
    0, 0, 0, 0,
    1, 0, 0, 0,
    0, 0, 1, 1,
};
array<uint8_t, TRACK_LENGTH> mTrackSnare {
    0, 0, 0, 0,
    1, 0, 0, 0,
    0, 0, 0, 0,
    1, 0, 0, 0,
};
array<uint8_t, TRACK_LENGTH> mTrackHihat {
    0, 1, 0, 1,
    0, 1, 1, 1,
    0, 1, 0, 1,
    1, 1, 1, 1,
};
uint8_t mTrackID = 0;

void setup() {
    Klang::lock();

    Klang::connect(mSampleSnare,   Node::CH_OUT_SIGNAL, mMixer1,      NodeMixer2::CH_IN_SIGNAL_0);
    Klang::connect(mZero,          Node::CH_OUT_SIGNAL, mMixer1,      NodeMixer2::CH_IN_SIGNAL_1);
    Klang::connect(mSampleHihat,   Node::CH_OUT_SIGNAL, mMixer2,      NodeMixer2::CH_IN_SIGNAL_0);
    Klang::connect(mSampleBass,    Node::CH_OUT_SIGNAL, mMixer2,      NodeMixer2::CH_IN_SIGNAL_1);
    Klang::connect(mMixer1,        Node::CH_OUT_SIGNAL, mMixerMaster, NodeMixer2::CH_IN_SIGNAL_1);
    Klang::connect(mMixer2,        Node::CH_OUT_SIGNAL, mMixerMaster, NodeMixer2::CH_IN_SIGNAL_0);
    Klang::connect(mMixerMaster,   Node::CH_OUT_SIGNAL, mDistortion,  Node::CH_IN_SIGNAL);
    Klang::connect(mDistortion,    Node::CH_OUT_SIGNAL, mVCA,         Node::CH_IN_SIGNAL);
#ifdef KLANG_EXT_NODE_REVERB
    Klang::connect(mVCA,           Node::CH_OUT_SIGNAL, mReverb,      Node::CH_IN_SIGNAL);
    Klang::connect(mReverb,        Node::CH_OUT_SIGNAL, mDAC,         NodeDAC::CH_IN_SIGNAL_LEFT);
#else
    Klang::connect(mVCA,           Node::CH_OUT_SIGNAL, mDAC,         NodeDAC::CH_IN_SIGNAL_LEFT);
#endif

    mSampleBass.loop(false);
    mSampleHihat.loop(false);
    mSampleSnare.loop(false);

    mDistortion.set_clip(0.1);
    mDistortion.set_type(NodeDistortion::TYPE::FOLDBACK);

#ifdef KLANG_EXT_NODE_REVERB
    mReverb.setroomsize(0.5);
    mReverb.setwet(0.4);
    mReverb.setdry(0.8);
#endif

    Klang::unlock();

    set_defaults();
}

void loop() {
    delay(50);
}

void set_defaults() {
    mBPM                    = 120 * 2;
    mAmplification          = 3.0;
    mDistortionAmount       = 5.0f;
    mVCA.set_amplification(mAmplification);
    mDistortion.set_amplification(mDistortionAmount);
    beats_per_minute(mBPM);
}

void shuffle_patterns() {
    for (uint8_t i = 0; i < TRACK_LENGTH; ++i) {
        mTrackBass[i]  = random(0, 3);
        mTrackSnare[i] = random(0, 2);
        mTrackHihat[i] = random(0, 6);
    }
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            handleEncoderButton(data[INDEX]);
            break;
        case EVENT_ENCODER_ROTATED:
            handleEncoderRotate(data[INDEX], data[TICK], data[PREVIOUS_TICK]);
            break;
    }
}

void handleEncoderButton(uint8_t pIndex) {
    switch (pIndex) {
        case ENCODER_00:
            set_defaults();
            break;
        case ENCODER_01:
            shuffle_patterns();
            break;
    }
}

void handleEncoderRotate(uint8_t pIndex, int pTick, int pPrevTick) {
    switch (pIndex) {
        case ENCODER_00:
            handle_change_distortion(pTick - pPrevTick);
            break;
        case ENCODER_01:
            handle_change_amplification(pTick - pPrevTick);
            break;
        case ENCODER_02:
            handle_change_bpm(pTick - pPrevTick);
            break;
    }
}

void handle_change_distortion(float mEncoderChange) {
    mDistortionAmount += mEncoderChange * 0.2f;
    mDistortionAmount = KlangMath::clamp(mDistortionAmount, 1.0, 20.0);
    mDistortion.set_amplification(mDistortionAmount);
}


void handle_change_amplification(float mEncoderChange) {
    mAmplification += mEncoderChange * 0.1f;
    mAmplification = KlangMath::clamp(mAmplification, 0.0, 12.0);
    mVCA.set_amplification(mAmplification);
}

void handle_change_bpm(float mEncoderChange) {
    mBPM += mEncoderChange * 0.5;
    beats_per_minute(mBPM);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void beat(uint32_t pBeat) {
    mTrackID++;
    mTrackID %= TRACK_LENGTH;

    led(LED_00, mTrackBass[mTrackID]);
    if (mTrackBass[mTrackID]) {
        mSampleBass.start();
    }

    led(LED_01, mTrackSnare[mTrackID]);
    if (mTrackSnare[mTrackID]) {
        mSampleSnare.start();
    }

    led(LED_02, mTrackHihat[mTrackID]);
    if (mTrackHihat[mTrackID]) {
        mSampleHihat.start();
    }
}
