#include "Klangstrom.h"
#include "KlangNodes.hpp"
#include "KlangstromCard.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayDrawBuffer.h"
#include "KlangstromDisplayFont.h"
#include "KlangstromDisplayFont_7x10.h"

using namespace klang;
using namespace klangstrom;

KlangstromWaveFile mWAV;

NodeDAC        mDAC;
NodeVCA        mVCA;
NodeVCA        mVCARight;
NodeSamplerI16 mSampler;
NodeSamplerI16 mSamplerRight;

uint16_t                    mTextSelected = 0;
vector<String>              mFiles;
KlangstromDisplayDrawBuffer mDrawBuffer(32);
static const uint8_t        SPACING        = 4;
bool                        mEncoder01Mode = true;

void setup() {
    Serial.begin(115200);
    Serial.println("--- AudioFilePlayer");

    Display.begin();
    Display.clear();

    Display.textFont(&Font_7x10);
    Display.text(20, 20, "KLANGSTROM");

    mDrawBuffer.width         = 128;
    mDrawBuffer.height        = 32;
    mDrawBuffer.x             = 20;
    mDrawBuffer.y             = 20 + Font_7x10.height + SPACING;
    mDrawBuffer.view          = KlangstromDisplayDrawBuffer::VIEW::LINES_SYMETRIC;
    mDrawBuffer.draw_baseline = true;
    mDrawBuffer.draw_frame(Display);

    Klang::connect(mSampler, Node::CH_OUT_SIGNAL, mVCA, Node::CH_IN_SIGNAL);
    Klang::connect(mSamplerRight, Node::CH_OUT_SIGNAL, mVCARight, Node::CH_IN_SIGNAL);
    Klang::connect(mVCA, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mVCARight, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    Card.begin();
    Card.get_file_list(mFiles);
    draw_files_list();

    LED(LED_00, mEncoder01Mode);
    LED(LED_01, !mEncoder01Mode);
    beats_per_minute(480);
}

void loop() {}

void draw_files_list() {
    for (uint16_t i = 0; i < mFiles.size(); i++) {
        if (i != mTextSelected) {
            Display.color(255, 255, 255);
            Display.background(0, 0, 0);
        } else {
            Display.color(0, 0, 0);
            Display.background(255, 255, 255);
        }
        Display.text(20, mDrawBuffer.height + mDrawBuffer.y + SPACING + 2 + Font_7x10.height * i, mFiles[i].c_str());
    }
}

void handle_file_open() {
    Serial.print("--- opening file: ");
    Serial.print(mFiles[mTextSelected]);
    bool mOpenError = Card.open(mFiles[mTextSelected]);
    Serial.println(mOpenError ? " FAILED" : " OK");

    if (Card.is_open()) {
        Klang::lock();
        mWAV.purge();

        int mError = Card.load_WAV(&mWAV);

        if (!mError) {
            mSampler.set_buffer(mWAV.get_sample_data(KlangstromWaveFile::CHANNEL_LEFT));
            mSampler.set_buffer_size(mWAV.number_of_samples);
            mSampler.loop(true);
            mSampler.start();

            if (mWAV.header->NumOfChan == 2) {
                mDAC.set_stereo(true);
                mSamplerRight.set_buffer(mWAV.get_sample_data(KlangstromWaveFile::CHANNEL_RIGHT));
                mSamplerRight.set_buffer_size(mWAV.number_of_samples);
                mSamplerRight.loop(true);
                mSamplerRight.start();
            } else {
                mDAC.set_stereo(false);
            }

        } else {
            Serial.print("--- loading WAV failed : ");
            Serial.println(mError);
        }
        Klang::unlock();
        Card.close();
    }
}

void handle_file_select(const float* data) {
    if (encoder_event(data).delta > 0) {
        mTextSelected++;
    } else {
        mTextSelected += mFiles.size() - 1;
    }
    mTextSelected %= mFiles.size();
    draw_files_list();
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (encoder_event(data).index == ENCODER_00) {
                handle_file_open();
            }
            if (encoder_event(data).index == ENCODER_01) {
                mEncoder01Mode = !mEncoder01Mode;
                LED(LED_00, mEncoder01Mode);
                LED(LED_01, !mEncoder01Mode);
            }
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_event(data).index == ENCODER_00) {
                handle_file_select(data);
            }
            if (encoder_event(data).index == ENCODER_01) {
                if (mEncoder01Mode) {
                    mSampler.set_speed(mSampler.get_speed() + encoder_event(data).delta * 0.1);
                    mSamplerRight.set_speed(mSampler.get_speed());
                } else {
                    float mAmp = mVCA.get_amplification();
                    mAmp += encoder_event(data).delta * 0.05;
                    mAmp = KlangMath::clamp(mAmp, 0, 20);
                    mVCA.set_amplification(mAmp);
                    mVCARight.set_amplification(mAmp);
                }
            }
            break;
    }
}

void audioblock(float* output_signal[LEFT], float* output_signal[RIGHT],
                float* input_signal[LEFT], float* input_signal[RIGHT]) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
    mDrawBuffer.update_buffer(output_signal[RIGHT], KLANG_SAMPLES_PER_AUDIO_BLOCK);
}

void beat(uint32_t pBeat) {
    Display.color(255, 255, 255);
    Display.background(0, 0, 0);
    mDrawBuffer.draw_buffer(Display);
}
