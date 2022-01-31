// # weekendproject
// started  12:30
// working: 12:30–13:00 = 1.5h
// working: 14:00–15:00 = 1.0h
// working: 21:00–23:00 = 2.0h
// finished 09:30–12:00 = 2.5h

#include <vector>

#include "KlangNodes.hpp"
#include "KlangstromCard.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayDrawBuffer.h"
#include "KlangstromDisplayFont_5x8.h"
#include "TaskScheduler.hpp"

using namespace std;
using namespace klang;
using namespace strom;
using namespace klangstrom;

const static uint8_t NUM_STEPS = 16;

class Vec2i {
public:
    int16_t x;
    int16_t y;
    Vec2i() : x(0), y(0) {}
    Vec2i(int16_t pX, int16_t pY) : x(pX), y(pY) {}

    void mult(int16_t m) {
        x *= m;
        y *= m;
    }

    void add(Vec2i v) {
        x += v.x;
        y += v.y;
    }

    void add(int16_t pX, int16_t pY) {
        x += pX;
        y += pY;
    }
};

class Pad {
public:
    Vec2i   position;
    int16_t size;
    bool    is_triggered;
    bool    is_set;
    bool    is_selected;
    Pad() : position(0, 0), size(0), is_triggered(false), is_set(false), is_selected(false) {}

    void draw(KlangstromDisplay& g) {
        Display.color(255, 255, 255);
        g.rect(position.x + 1, position.y + 1, size - 2, size - 2, true);
        Display.color(0, 0, 0);
        if (is_triggered) {
            g.rect(position.x + 4, position.y + 4, size - 8, size - 8, false);
        }
        if (is_selected) {
            g.rect(position.x + 2, position.y + 2, size - 4, size - 4, false);
            g.rect(position.x + 3, position.y + 3, size - 6, size - 6, false);
        }
        if (is_set) {
            g.rect(position.x + 5, position.y + 5, size - 10, size - 10, true);
        }
    }
};

class Instrument {
public:
    const static uint8_t PAD_WIDTH  = 4;
    const static uint8_t PAD_HEIGHT = 4;

    NodeSamplerI16 sampler;
    String         name;

    Instrument(int16_t pPadSize) {
        for (int i = 0; i < NUM_PADS; i++) {
            Pad& p       = fPads[i];
            p.size       = pPadSize;
            p.position.x = i % PAD_WIDTH;
            p.position.y = i / PAD_HEIGHT;
            p.position.mult(pPadSize);
        }
    }

    void reset() {
        for (int i = 0; i < NUM_PADS; i++) {
            Pad& p         = fPads[i];
            p.is_triggered = false;
        }
    }

    void move_pads(int16_t x, int16_t y) {
        for (int i = 0; i < NUM_PADS; i++) {
            Pad& p = fPads[i];
            p.position.x += x;
            p.position.y += y;
        }
    }

    void draw(KlangstromDisplay& g) {
        for (int i = 0; i < NUM_PADS; i++) {
            Pad& p = fPads[i];
            p.draw(g);
        }
    }

    void toggle_pad(KlangstromDisplay& g, int8_t pID) {
        Pad& p   = fPads[pID];
        p.is_set = !p.is_set;
        p.draw(g);
    }

    void set_pad(KlangstromDisplay& g, int8_t pID) {
        Pad& p   = fPads[pID];
        p.is_set = true;
        p.draw(g);
    }

    void play_pad(KlangstromDisplay& g, int8_t pStep) {
        Pad& p = fPads[pStep];
        if (p.is_set) {
            sampler.start();
        }
    }

    void select_pad(KlangstromDisplay& g, int8_t pStep, bool pIsSelected) {
        Pad& p        = fPads[pStep];
        p.is_selected = pIsSelected;
        p.draw(g);
    }

    void unselect_pads() {
        for (int i = 0; i < NUM_PADS; i++) {
            Pad& p        = fPads[i];
            p.is_selected = false;
        }
    }

    void trigger_pad(int8_t pStep) {
        Pad& p         = fPads[pStep];
        p.is_triggered = true;
    }

    void release_pad(int8_t pStep) {
        Pad& p         = fPads[pStep];
        p.is_triggered = false;
    }

    void draw_pad(KlangstromDisplay& g, int8_t pStep) {
        Pad& p = fPads[pStep];
        p.draw(g);
    }

private:
    const static uint8_t NUM_PADS = NUM_STEPS;
    Pad                  fPads[NUM_PADS];
};

const static uint8_t STATE_INSTRUMENT = 0;
const static uint8_t STATE_BPM        = 1;
const static uint8_t STATE_VOLUME     = 2;
const static uint8_t STATE_COUNT      = 3;
uint8_t              fState           = STATE_INSTRUMENT;

const static uint16_t       INSTRUMENT_X = 10;
const static uint16_t       INSTRUMENT_Y = 30;
const static uint16_t       PAD_SIZE     = 24;
TaskScheduler               fScheduler;
NodeDAC                     fDAC;
NodeMixerMultiStereo        fMixer;
KlangstromDisplayDrawBuffer fDrawBuffer(32);
vector<Instrument*>         fInstruments;
uint16_t                    fBPM                = 120;
int8_t                      fBeatStep           = 0;
int8_t                      fPadSelected        = 0;
int8_t                      fInstrumentSelected = 0;
bool                        fBeatTriggered      = false;
uint16_t                    fRedrawReleased     = 0;
uint16_t                    fRedrawTriggered    = 0;

void update_display() {
    Display.color(255, 255, 255);
    Display.background(0, 0, 0);
    fDrawBuffer.draw_buffer(Display);
}

void draw_instrument_list() {
    const int16_t x = INSTRUMENT_X + Instrument::PAD_WIDTH * PAD_SIZE + 15;
    const int16_t y = INSTRUMENT_Y;
    for (uint16_t i = 0; i < fInstruments.size(); i++) {
        if (i != fInstrumentSelected || fState != STATE_INSTRUMENT) {
            Display.color(255, 255, 255);
            Display.background(0, 0, 0);
        } else {
            Display.color(0, 0, 0);
            Display.background(255, 255, 255);
        }
        Display.text(x, y + Font_5x8.height * i, fInstruments[i]->name.c_str());
    }
}

void update_display_BPM() {
    Display.color(255, 255, 255);
    Display.background(0, 0, 0);
    Display.text(fDrawBuffer.x,
                 fDrawBuffer.y - 30,
                 String("BPM   : ").c_str());
    if (fState == STATE_BPM) {
        Display.color(0, 0, 0);
        Display.background(255, 255, 255);
    }
    Display.text(fDrawBuffer.x + Font_5x8.width * 8,
                 fDrawBuffer.y - 30,
                 String(fBPM).c_str());
}

void update_display_VOLUME() {
    Display.color(255, 255, 255);
    Display.background(0, 0, 0);
    Display.text(fDrawBuffer.x,
                 fDrawBuffer.y - 20,
                 String("VOLUME: ").c_str());
    if (fState == STATE_VOLUME) {
        Display.color(0, 0, 0);
        Display.background(255, 255, 255);
    }
    Display.text(fDrawBuffer.x + Font_5x8.width * 8,
                 fDrawBuffer.y - 20,
                 String(fMixer.get_master()).c_str());
}

void setup() {
    Serial.begin(115200);

    Display.begin();
    Display.background(0, 0, 0);
    Display.color(255, 255, 255);
    Display.clear();

    Display.textFont(&Font_5x8);
    Display.text(10, 10, "Happy Drum Machine");

    fDrawBuffer.width         = 128;
    fDrawBuffer.height        = 64;
    fDrawBuffer.x             = 10;
    fDrawBuffer.y             = Display.height() - fDrawBuffer.height - 10;
    fDrawBuffer.view          = KlangstromDisplayDrawBuffer::VIEW::LINES_SYMETRIC;
    fDrawBuffer.draw_baseline = true;
    fDrawBuffer.draw_frame(Display);

    /* --- load samples --- */

    vector<String> mFiles;
    Card.begin();
    Card.get_file_list(mFiles);

    for (uint16_t i = 0; i < mFiles.size(); i++) {
        Instrument* mInstrument = new Instrument(PAD_SIZE);
        mInstrument->move_pads(INSTRUMENT_X, INSTRUMENT_Y);
        fInstruments.push_back(mInstrument);

        bool mOpenError = Card.open(mFiles[i]);
        if (mOpenError) {
            Serial.println("--- error opening card");
            return;
        }
        KlangstromWaveFile mWAV;
        int                mError = Card.load_WAV(&mWAV);
        if (!mError) {
            mInstrument->sampler = NodeSamplerI16();
            mInstrument->sampler.set_buffer(mWAV.get_sample_data(KlangstromWaveFile::CHANNEL_LEFT));
            mInstrument->sampler.set_buffer_size(mWAV.number_of_samples);
            mInstrument->sampler.loop(false);
        } else {
            Serial.print("--- loading WAV failed : ");
            Serial.println(mError);
        }

        mFiles[i].toUpperCase();
        mInstrument->name = mFiles[i].substring(0, mFiles[i].length() - 4).c_str();
        Card.close();
    }

    fInstruments[fInstrumentSelected]->draw(Display);
    fInstruments[fInstrumentSelected]->select_pad(Display, fPadSelected, true);
    draw_instrument_list();
    update_display_BPM();
    update_display_VOLUME();

    /* --- patch nodes --- */

    Klang::lock();
    for (uint8_t i = 0; i < fInstruments.size(); ++i) {
        Klang::connect(fInstruments[i]->sampler, Node::CH_OUT_SIGNAL, fMixer, i);
    }
    Klang::connect(fMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_LEFT, fDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(fMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_RIGHT, fDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);
    Klang::unlock();

    beats_per_minute(fBPM * 4);
    fScheduler.schedule_task(update_display, 100000);
}

void handle_beat() {
    fInstruments[fInstrumentSelected]->release_pad(fBeatStep);
    fRedrawReleased = fBeatStep;
    fBeatStep++;
    fBeatStep %= NUM_STEPS;
    fInstruments[fInstrumentSelected]->trigger_pad(fBeatStep);
    fRedrawTriggered = fBeatStep;
    for (uint8_t i = 0; i < fInstruments.size(); ++i) {
        fInstruments[i]->play_pad(Display, fBeatStep);
    }
}

void beat(uint32_t pBeat) {
    fBeatTriggered = true;
    handle_beat();
}

void loop() {
    fScheduler.update(micros());
    if (fBeatTriggered) {
        /* note that it is better to only issue drawing commands in `loop` */
        fInstruments[fInstrumentSelected]->draw_pad(Display, fRedrawReleased);
        fInstruments[fInstrumentSelected]->draw_pad(Display, fRedrawTriggered);
        fBeatTriggered = false;
    }
}

void handle_state_instrument_selection(const float* data) {
    if (encoder_rotated(data).delta != 0) {
        fInstruments[fInstrumentSelected]->reset();
        fInstrumentSelected += encoder_rotated(data).delta;
        if (fInstrumentSelected < 0) {
            fInstrumentSelected = fInstruments.size() - 1;
        }
        fInstrumentSelected %= fInstruments.size();
        fInstruments[fInstrumentSelected]->unselect_pads();
        fInstruments[fInstrumentSelected]->draw(Display);
        fInstruments[fInstrumentSelected]->select_pad(Display, fPadSelected, true);
        draw_instrument_list();
    }
}

void handle_state_bpm(const float* data) {
    if (encoder_rotated(data).delta != 0) {
        fBPM += encoder_rotated(data).delta;
        if (fBPM < 1) {
            fBPM = 1;
        }
        beats_per_minute(fBPM * 4);
        update_display_BPM();
    }
}

void handle_state_volume(const float* data) {
    if (encoder_rotated(data).delta != 0) {
        fMixer.set_master(fMixer.get_master() + 0.05 * encoder_rotated(data).delta);
        if (fMixer.get_master() < 0) {
            fMixer.set_master(0);
        }
        update_display_VOLUME();
    }
}

void event_receive(const uint8_t event, const float* data) {
    Instrument* mInstrument = fInstruments[fInstrumentSelected];
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            if (encoder_pressed(data).index == ENCODER_00) {
                mInstrument->toggle_pad(Display, fPadSelected);
            }
            if (encoder_pressed(data).index == ENCODER_01) {
                fState++;
                fState %= STATE_COUNT;
                update_display_BPM();
                update_display_VOLUME();
                draw_instrument_list();
            }
            break;
        case EVENT_ENCODER_ROTATED:
            if (encoder_rotated(data).index == ENCODER_00) {
                mInstrument->select_pad(Display, fPadSelected, false);
                fPadSelected += encoder_rotated(data).delta;
                if (fPadSelected < 0) {
                    fPadSelected = NUM_STEPS - 1;
                }
                fPadSelected %= NUM_STEPS;
                mInstrument->select_pad(Display, fPadSelected, true);
            }
            if (encoder_rotated(data).index == ENCODER_01) {
                switch (fState) {
                    case STATE_INSTRUMENT:
                        handle_state_instrument_selection(data);
                        break;
                    case STATE_BPM:
                        handle_state_bpm(data);
                        break;
                    case STATE_VOLUME:
                        handle_state_volume(data);
                        break;
                    default:
                        break;
                }
            }
            break;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    fDAC.process_frame(pOutputLeft, pOutputRight);
    fDrawBuffer.update_buffer(pOutputRight, KLANG_SAMPLES_PER_AUDIO_BLOCK);
}
