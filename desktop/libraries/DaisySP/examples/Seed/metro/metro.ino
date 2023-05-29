// Title: metro
// Description: Sets oscillator to random notes on a steady metro
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Metro      metro;
static Oscillator osc_sine;

void audioblock(float** in, float** out) {
    size_t  size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float   sine, freq;
    uint8_t tic;
    for (size_t i = 0; i < size; i++) {
        tic = metro.Process();
        if (tic) {
            freq = rand() % 500;
            osc_sine.SetFreq(freq);
        }

        sine = osc_sine.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sine;
        }
    }
}

void setup() {
    // initialize Metro object at 2 hz
    metro.Init(2, sample_rate);

    // set parameters for sine oscillator object
    osc_sine.Init(sample_rate);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);
}

void loop() {}
