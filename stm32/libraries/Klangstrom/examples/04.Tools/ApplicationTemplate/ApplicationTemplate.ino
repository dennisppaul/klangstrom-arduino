#include "KlangNodes.hpp"
#include "Klangstrom.h"

// @TODO merge with `SchedulingTasks`

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;
bool            toggle_amplitude = false;

float         output_buffer_left[KLANG_SAMPLES_PER_AUDIO_BLOCK];
float         output_buffer_right[KLANG_SAMPLES_PER_AUDIO_BLOCK];
volatile bool schedule_audio = false;
volatile bool schedule_beat  = false;

void setup() {
    Serial.begin(115200);
    Serial.println("-------------------");
    Serial.println("ApplicationTemplate");
    Serial.println("-------------------");

    Klang::lock();
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    mVCO.set_amplitude(0.0);
    mVCO.set_frequency(440.0);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();

    beats_per_minute(120);
}

void loop() {
    process_audio();
    process_beat();
}

void process_beat() {
    if (schedule_beat) {
        schedule_beat = false;
        LED(LED_00, LED_TOGGLE);
        toggle_amplitude = !toggle_amplitude;
        if (toggle_amplitude) {
            mVCO.set_amplitude(0.25);
        } else {
            mVCO.set_amplitude(0.0);
        }
    }
}

void process_audio() {
    if (schedule_audio) {
        schedule_audio = false;
        mDAC.process_frame(output_buffer_left, output_buffer_right);
    }
}

void beat(uint32_t pBeat) {
    schedule_beat = true;
}

void audioblock(float** input_signal, float** output_signal) {
    KLANG_COPY_AUDIO_BUFFER(output_signal[LEFT], output_buffer_left);
    KLANG_COPY_AUDIO_BUFFER(output_signal[RIGHT], output_buffer_right);
    schedule_audio = true;
}
