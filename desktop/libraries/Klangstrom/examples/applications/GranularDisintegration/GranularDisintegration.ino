// # weekendproject
// started  12:00
// working: 12:00–15:00 = 3.0h
// working: 18:30–21:00 = 2.5h

#include "KlangNodes.hpp"
#include "KlangstromCard.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayDrawBuffer.h"
#include "KlangstromDisplayFont.h"
#include "KlangstromDisplayFont_5x8.h"
#include "TaskScheduler.hpp"

using namespace klang;
using namespace strom;
using namespace klangstrom;

class SamplerView : public KlangstromDisplayDrawBuffer {
public:
    void draw_in_out(KlangstromDisplay& g, bool pClearBackground = true) {
        if (fSampler != nullptr) {
            if (pClearBackground) {
                g.BSP_rect_fill(x + 1, y + 1, width - 2, height - 1, g.get_background_16i());
            }

            // @TODO use doted lines for in/out?
            const uint16_t xIn  = x + (width - 0.5) * fSampler->get_in();
            const uint16_t xOut = x + (width - 0.5) * fSampler->get_out();
            const uint16_t y0   = y + 1;
            const uint16_t h    = height - 2;

            g.BSP_line_vertical(xIn, y0, h, g.get_color_16i());
            g.BSP_line_vertical(xOut, y0, h, g.get_color_16i());
        }
    }

    void draw(KlangstromDisplay& g) {
        draw_frame(g);
        draw_buffer(g);
        draw_in_out(g);
    }

    void update_sample_data(bool pAverageSamples = false) {
        // @TODO(average intermediate or omit?)
        update_buffer(fSampler->get_buffer(), fSampler->get_buffer_size(), pAverageSamples);
    }

    void set_sampler(NodeSamplerI16* pSampler) {
        fSampler = pSampler;
    }

private:
    NodeSamplerI16* fSampler = nullptr;
};

static const uint8_t TRACK_COUNT                       = 4;
static const uint8_t SPACING                           = 4;
static const uint8_t APP_STATE_MANIPULATE_SAMPLE       = 0;
static const uint8_t APP_STATE_MANIPULATE_VOLUME_SPEED = 1;
static const uint8_t APP_STATE_COUNT                   = 2;
static const uint8_t APP_STATE_SELECT_FILE             = APP_STATE_COUNT;

TaskScheduler               fScheduler;
SamplerView                 fSamplerViews[TRACK_COUNT];
NodeSamplerI16              fSamplers[TRACK_COUNT];
NodeDAC                     fDAC;
NodeMixerMultiStereo        fMixer;
uint8_t                     fTrack        = 0;
uint16_t                    mTextSelected = 0;
vector<String>              mFiles;
KlangstromDisplayDrawBuffer mDrawBuffer(32);
uint8_t                     fAppState = APP_STATE_SELECT_FILE;

void setup() {
    Serial.begin(115200);
    Serial.println("--- Granular Disintegration ---");

    Display.begin();
    Display.clear();

    Display.textFont(&Font_5x8);
    Display.text(10, 10, "Granular Disintegration");

    mDrawBuffer.width         = 128;
    mDrawBuffer.height        = 32;
    mDrawBuffer.x             = 10;
    mDrawBuffer.y             = 10 + Font_5x8.height + SPACING;
    mDrawBuffer.view          = KlangstromDisplayDrawBuffer::VIEW::LINES_SYMETRIC;
    mDrawBuffer.draw_baseline = true;
    mDrawBuffer.draw_frame(Display);

    for (uint8_t i = 0; i < TRACK_COUNT; ++i) {
        SamplerView& fSamplerView  = fSamplerViews[i];
        fSamplerView.width         = 128;
        fSamplerView.height        = 32;
        fSamplerView.x             = 10;
        fSamplerView.y             = 30 + Font_5x8.height + SPACING + mDrawBuffer.height + SPACING + (32 + SPACING) * i;
        fSamplerView.view          = KlangstromDisplayDrawBuffer::VIEW::LINES_SYMETRIC;
        fSamplerView.draw_baseline = true;
        fSamplerView.draw_frame(Display);
        fSamplerView.set_sampler(&fSamplers[i]);
    }

    /* --- patch nodes --- */

    Klang::lock();
    for (uint8_t i = 0; i < TRACK_COUNT; ++i) {
        Klang::connect(fSamplers[i], Node::CH_OUT_SIGNAL, fMixer, i);
    }
    Klang::connect(fMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_LEFT, fDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(fMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_RIGHT, fDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);
    Klang::unlock();

    Card.begin();
    Card.get_file_list(mFiles);
    draw_files_list();

    fScheduler.schedule_task(update_display, 100000);
}

void draw_track_manipulate_sample(uint8_t pTrackID, bool pSelected) {
    const uint16_t x0 = fSamplerViews[pTrackID].x + fSamplerViews[pTrackID].width + SPACING;
    const uint16_t y0 = fSamplerViews[pTrackID].y;
    if (pSelected) {
        Display.color(255, 255, 255);
    } else {
        Display.color(0, 0, 0);
    }
    Display.rect(x0, y0, 6, fSamplerViews[pTrackID].height + 1, true);
}

void draw_track_manipulate_volume_speed(uint8_t pTrackID, bool pSelected) {
    const uint16_t x0 = fSamplerViews[pTrackID].x + fSamplerViews[pTrackID].width + SPACING + 3 + SPACING;
    const uint16_t y0 = fSamplerViews[pTrackID].y;
    const uint16_t y1 = fSamplerViews[pTrackID].y + Font_5x8.height;
    if (pSelected) {
        Display.color(0, 0, 0);
        Display.background(255, 255, 255);
    } else {
        Display.color(255, 255, 255);
        Display.background(0, 0, 0);
    }
    Display.BSP_rect_fill(x0, y0, 72, fSamplerViews[pTrackID].height + 1, Display.get_background_16i());
    const uint16_t x1      = x0 + 5;
    const uint16_t y2      = y0 + 10;
    const uint16_t y3      = y1 + 10;
    String         mVolume = String(fSamplers[pTrackID].get_amplification(), 2);
    String         mSpeed  = String(fSamplers[pTrackID].get_speed(), 2);
    Display.text(x1, y2, String("VOL: " + mVolume).c_str());
    Display.text(x1, y3, String("SPD: " + mSpeed).c_str());
}

void draw_files_list() {
    for (uint16_t i = 0; i < mFiles.size(); i++) {
        if (i != mTextSelected) {
            Display.color(255, 255, 255);
            Display.background(0, 0, 0);
        } else {
            Display.color(0, 0, 0);
            Display.background(255, 255, 255);
        }
        Display.text(mDrawBuffer.width + mDrawBuffer.x + SPACING + 2, 2 + 20 + Font_5x8.height * i, mFiles[i].c_str());
    }
}

void clear_files_list() {
    for (uint16_t i = 0; i < mFiles.size(); i++) {
        Display.background(0, 0, 0);
        Display.color(0, 0, 0);
        Display.text(mDrawBuffer.width + mDrawBuffer.x + SPACING + 2, 2 + 20 + Font_5x8.height * i, mFiles[i].c_str());
    }
}

void change_app_state(uint8_t pAppState) {
    /* clear old state */
    switch (fAppState) {
        case APP_STATE_SELECT_FILE:
            LED(LED_00, LED_OFF);
            clear_files_list();
            for (uint8_t i = 0; i < TRACK_COUNT; ++i) {
                draw_track_manipulate_sample(i, false);
                draw_track_manipulate_volume_speed(i, false);
            }
            break;
        case APP_STATE_MANIPULATE_SAMPLE:
            draw_track_manipulate_sample(fTrack, false);
            LED(LED_01, LED_OFF);
            break;
        case APP_STATE_MANIPULATE_VOLUME_SPEED:
            draw_track_manipulate_volume_speed(fTrack, false);
            LED(LED_02, LED_OFF);
            break;
        default:
            break;
    }
    /* set new state */
    fAppState = pAppState;
    switch (fAppState) {
        case APP_STATE_SELECT_FILE:
            LED(LED_00, LED_ON);
            break;
        case APP_STATE_MANIPULATE_SAMPLE:
            for (uint8_t i = 0; i < TRACK_COUNT; ++i) {
                fSamplerViews[i].update_sample_data();
                Display.color(255, 255, 255);
                Display.background(0, 0, 0);
                fSamplerViews[i].draw_buffer(Display);
            }
            draw_track_manipulate_sample(fTrack, true);
            LED(LED_01, LED_ON);
            break;
        case APP_STATE_MANIPULATE_VOLUME_SPEED:
            draw_track_manipulate_volume_speed(fTrack, true);
            LED(LED_02, LED_ON);
            break;
        default:
            break;
    }
}

void handle_file_open() {
    Serial.print("--- opening file: ");
    Serial.print(mFiles[mTextSelected]);
    bool mOpenError = Card.open(mFiles[mTextSelected]);
    Serial.println(mOpenError ? " FAILED" : " OK");

    if (Card.is_open()) {
        Klang::lock();
        KlangstromWaveFile mWAV;
        int                mError = Card.load_WAV(&mWAV);
        if (!mError) {
            for (uint8_t i = 0; i < TRACK_COUNT; ++i) {
                fSamplers[i].set_buffer(mWAV.get_sample_data(KlangstromWaveFile::CHANNEL_LEFT));
                fSamplers[i].set_buffer_size(mWAV.number_of_samples);
                fSamplers[i].set_amplification(1.0 / TRACK_COUNT);
                fSamplers[i].loop(true);
                fSamplers[i].start();
            }
        } else {
            Serial.print("--- loading WAV failed : ");
            Serial.println(mError);
        }
        Klang::unlock();
        Card.close();
    }

    change_app_state(APP_STATE_MANIPULATE_SAMPLE);
}

void handle_file_select(const float* data) {
    if (encoder_rotated(data).delta > 0) {
        mTextSelected++;
    } else {
        mTextSelected += mFiles.size() - 1;
    }
    mTextSelected %= mFiles.size();
    draw_files_list();
}

void handle_app_state_select_file(const uint8_t event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (encoder_pressed(data).index == ENCODER_00) {
                handle_file_open();
            }
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_rotated(data).index == ENCODER_00) {
                handle_file_select(data);
            }
            break;
    }
}

void handle_app_state_manipulate_sample(const uint8_t event, const float* data) {
    NodeSamplerI16& mSampler = fSamplers[fTrack];
    switch (event) {
        case EVENT_MOUSE_PRESSED:
            if (data[X] > 0.5) {
                mSampler.set_out(data[X]);
            } else {
                mSampler.set_in(data[X]);
            }
            fSamplerViews[fTrack].draw_in_out(Display, true);
            fSamplerViews[fTrack].draw_buffer(Display, false);
            break;
        case EVENT_MOUSE_DRAGGED:
            mSampler.set_speed(data[X] * 2.0 - 1.0);
            break;
    }
    switch (event) {
        case EVENT_ENCODER_ROTATED:
            const static float mSpeed = 0.01;
            if (encoder_rotated(data).index == ENCODER_00) {
                mSampler.set_in(mSampler.get_in() + encoder_rotated(data).delta * mSpeed);
            }
            if (encoder_rotated(data).index == ENCODER_01) {
                mSampler.set_out(mSampler.get_out() + encoder_rotated(data).delta * mSpeed);
            }
            fSamplerViews[fTrack].draw_in_out(Display, true);
            fSamplerViews[fTrack].draw_buffer(Display, false);
            break;
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (encoder_pressed(data).index == ENCODER_00) {
                draw_track_manipulate_sample(fTrack, false);
                fTrack++;
                fTrack %= TRACK_COUNT;
                draw_track_manipulate_sample(fTrack, true);
            }
            if (encoder_pressed(data).index == ENCODER_01) {
                change_app_state(APP_STATE_MANIPULATE_VOLUME_SPEED);
            }
            break;
    }
}

void handle_app_state_manipulate_volume_speed(const uint8_t event, const float* data) {
    NodeSamplerI16& mSampler = fSamplers[fTrack];
    switch (event) {
        case EVENT_ENCODER_ROTATED:
            if (encoder_rotated(data).index == ENCODER_00) {
                const float mAmplification = mSampler.get_amplification() + encoder_rotated(data).delta * 0.05;
                mSampler.set_amplification(KlangMath::clamp(mAmplification, 0, 20));
                draw_track_manipulate_volume_speed(fTrack, true);
            }
            if (encoder_rotated(data).index == ENCODER_01) {
                mSampler.set_speed(mSampler.get_speed() + encoder_rotated(data).delta * 0.1);
                draw_track_manipulate_volume_speed(fTrack, true);
            }
            break;
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (encoder_pressed(data).index == ENCODER_00) {
                draw_track_manipulate_volume_speed(fTrack, false);
                fTrack++;
                fTrack %= TRACK_COUNT;
                draw_track_manipulate_volume_speed(fTrack, true);
            }
            if (encoder_pressed(data).index == ENCODER_01) {
                change_app_state(APP_STATE_MANIPULATE_SAMPLE);
            }
            break;
    }
}

void event_receive(const uint8_t event, const float* data) {
    switch (fAppState) {
        case APP_STATE_SELECT_FILE:
            handle_app_state_select_file(event, data);
            break;
        case APP_STATE_MANIPULATE_SAMPLE:
            handle_app_state_manipulate_sample(event, data);
            break;
        case APP_STATE_MANIPULATE_VOLUME_SPEED:
            handle_app_state_manipulate_volume_speed(event, data);
            break;
        default:
            break;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    fDAC.process_frame(pOutputLeft, pOutputRight);
    mDrawBuffer.update_buffer(pOutputRight, KLANG_SAMPLES_PER_AUDIO_BLOCK);
}

void update_display() {
    Display.color(255, 255, 255);
    Display.background(0, 0, 0);
    mDrawBuffer.draw_buffer(Display);
}

void loop() {
    fScheduler.update(micros());
}