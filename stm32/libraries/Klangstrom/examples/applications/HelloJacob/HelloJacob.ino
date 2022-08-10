#define KLANG_EXT_NODE_REVERB

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace std;
using namespace klang;
using namespace klangstrom;

NodeVCOWavetable    mOscLeft;
NodeVCOWavetable    mOscRight;
NodeDAC             mDAC;
NodeADSR            mADSR;
NodeMixer2          mMixer;
NodeVCFMoogLP       mFilter;
NodeVCA             mVCA;
#ifdef KLANG_EXT_NODE_REVERB
NodeReverb          mReverb;
#endif

float mBaseFreq;
float mFreq;
float mBPM;
float mFreqOffset;
float mFilterCutoff;
float mFilterResonance;
bool  mFilterToggle;

array<uint8_t, 32> mTrack {
    0, 0, 1, 1,
    0, 0, 2, 2,
    0, 0, 1, 1,
    0, 0, 4, 4,
    0, 0, 1, 1,
    0, 0, 2, 2,
    0, 0, 1, 1,
    0, 0, 4, 4,
};
uint8_t mTrackID = 0;

void setup() {
    Klang::lock();

    Klang::connect(mOscLeft,  Node::CH_OUT_SIGNAL, mMixer,    NodeMixer2::CH_IN_SIGNAL_0);
    Klang::connect(mOscRight, Node::CH_OUT_SIGNAL, mMixer,    NodeMixer2::CH_IN_SIGNAL_1);
    Klang::connect(mMixer,    Node::CH_OUT_SIGNAL, mFilter,   Node::CH_IN_SIGNAL);
    Klang::connect(mFilter,   Node::CH_OUT_SIGNAL, mVCA,      Node::CH_IN_SIGNAL);
    Klang::connect(mVCA,      Node::CH_OUT_SIGNAL, mADSR,     Node::CH_IN_SIGNAL);
#ifdef KLANG_EXT_NODE_REVERB
    Klang::connect(mADSR,     Node::CH_OUT_SIGNAL, mReverb,   Node::CH_IN_SIGNAL);
    Klang::connect(mReverb,   Node::CH_OUT_SIGNAL, mDAC,      NodeDAC::CH_IN_SIGNAL_LEFT);
#else
    Klang::connect(mADSR,     Node::CH_OUT_SIGNAL, mDAC,      NodeDAC::CH_IN_SIGNAL_LEFT);
#endif
    mOscLeft.set_frequency(mFreq);
    mOscLeft.set_amplitude(0.5);
    mOscLeft.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
    mOscRight.set_frequency(mFreq);
    mOscRight.set_amplitude(0.25);
    mOscRight.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);

    mADSR.set_attack(0.005f);
    mADSR.set_decay(0.1f);
    mADSR.set_sustain(0.00001f);
    mADSR.set_release(0.0f);

    mVCA.set_amplification(4.0);

#ifdef KLANG_EXT_NODE_REVERB
    mReverb.set_roomsize(0.5);
    mReverb.set_wet(0.5);
    mReverb.set_dry(0.5);
#endif

    Klang::unlock();

    set_defaults();
    beats_per_minute(mBPM);
}

void loop() {
    delay(50);
}

void set_defaults() {
    mBaseFreq        = 55;
    mFreq            = mBaseFreq;
    mBPM             = 240;
    mFreqOffset      = 2;
    mFilterCutoff    = 6000.0;
    mFilterResonance = 0.5;
    mFilterToggle    = false;
    update_freq();
    update_filter();
    beats_per_minute(mBPM);
}

void event_receive(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            handleEncoderButton(encoder_event(data).index);
            break;
        case EVENT_ENCODER_ROTATED:
            handleEncoderRotate(encoder_event(data).index, encoder_event(data).delta);
            break;
    }
}


void handleEncoderButton(uint8_t pIndex) {
    switch (pIndex) {
        case ENCODER_00:
            shuffle_track();
            break;
        case ENCODER_01:
            set_defaults();
            break;
        case ENCODER_02:
            mFilterToggle = !mFilterToggle;
            led(LED_02, mFilterToggle);
            break;
    }
}


void handleEncoderRotate(uint8_t pIndex, float pDelta) {
    switch (pIndex) {
        case ENCODER_00:
            handle_change_freq_offset(pDelta);
            break;
        case ENCODER_01:
            handle_change_bpm(pDelta);
            break;
        case ENCODER_02:
            if (mFilterToggle) {
                handle_change_filter_cutoff(pDelta);
            } else {
                handle_change_filter_resonance(pDelta);
            }
            break;
    }
}

void shuffle_track() {
    for (uint8_t i = 0; i < mTrack.size() / 4; i++) {
        const uint8_t mID = i * 4 + 2;
        mTrack[mID] = random(0, 5);
        mTrack[mID + 1] = mTrack[mID];
    }
}

void update_freq() {
    mOscLeft.set_frequency(mFreq);
    const float mFreqOffsetRatio = 1.0 + mFreqOffset * 0.01;
    mOscRight.set_frequency(mFreq * mFreqOffsetRatio);
}

void update_filter() {
    mFilter.set_resonance(mFilterResonance);
    mFilter.set_cutoff_Hz(mFilterCutoff);
}

void handle_change_bpm(float mEncoderChange) {
    mBPM += mEncoderChange * 0.5;
    beats_per_minute(mBPM);
}

void handle_change_filter_cutoff(float mEncoderChange) {
    mFilterCutoff += mEncoderChange * 55.0;
    mFilterCutoff = KlangMath::clamp(mFilterCutoff, 100, 10000);
    mFilter.set_cutoff_Hz(mFilterCutoff);
}

void handle_change_filter_resonance(float mEncoderChange) {
    mFilterResonance += mEncoderChange * 0.01;
    mFilterResonance = KlangMath::clamp(mFilterResonance, 0.01, 0.99);
    mFilter.set_resonance(mFilterResonance);
}

void handle_change_freq_offset(float mEncoderChange) {
    mFreqOffset += mEncoderChange * 0.1;
    update_freq();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void beat(uint32_t pBeat) {
    led_toggle(LED_01);
    mTrackID++;
    mTrackID %= mTrack.size();
    mFreq = mBaseFreq + mBaseFreq * mTrack[mTrackID];
    update_freq();
    mADSR.start();
}
