// Title: line
// Description: Generates a straight line between 2 points, then uses that to
// set an osc pitch Hardware: Daisy Seed Author: Stephen Hensley

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Line       line_seg;
static Oscillator osc_sine;

uint8_t finished;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sine, freq;
    for (size_t i = 0; i < size; i++) {
        if (finished) {
            // Start creating a Line segment from 100 to 500 in 1 seconds
            line_seg.Start(100, 500, 1);
        }

        freq = line_seg.Process(&finished);
        osc_sine.SetFreq(freq);
        sine = osc_sine.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sine;
        }
    }
}

void setup() {
    // initialize Line module
    line_seg.Init(sample_rate);
    finished = 1;

    // set parameters for sine oscillator object
    osc_sine.Init(sample_rate);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);
}

void loop() {}
