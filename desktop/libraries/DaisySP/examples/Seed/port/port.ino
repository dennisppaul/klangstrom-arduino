// Title: port
// Description: Slides between random notes
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Port       slew;
static Metro      metro;
static Oscillator osc_sine;

float freq;

void audioblock(float** in, float** out) {
    size_t  size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float   sine, slewed_freq;
    uint8_t tic;
    for (size_t i = 0; i < size; i++) {
        tic = metro.Process();
        if (tic) {
            freq = rand() % 500;
        }

        slewed_freq = slew.Process(freq);
        osc_sine.SetFreq(slewed_freq);

        sine = osc_sine.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sine;
        }
    }
}

void setup() {
    // set params for Port object
    slew.Init(sample_rate, .09);

    metro.Init(1, sample_rate);

    // set parameters for sine oscillator object
    osc_sine.Init(sample_rate);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);
}

void loop() {}
