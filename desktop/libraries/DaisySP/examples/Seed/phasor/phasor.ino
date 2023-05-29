// Title: phasor
// Description: Control osc pitch with ramp wave
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Phasor     ramp;
static Oscillator osc_sine;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sine, freq;
    for (size_t i = 0; i < size; i++) {
        // generate Phasor value (0-1), and scale it between 0 and 300
        freq = ramp.Process() * 300;

        osc_sine.SetFreq(freq);
        sine = osc_sine.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sine;
        }
    }
}

void setup() {
    // initialize Phasor module
    ramp.Init(sample_rate, 1, 0);

    // set parameters for sine oscillator object
    osc_sine.Init(sample_rate);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);
}

void loop() {}
