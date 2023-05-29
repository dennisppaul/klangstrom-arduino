// Title: biquad
// Description: Biquadratic resonant filter.
// Hardware: Daisy Seed
// Author: Ben Sergentanis

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Biquad     flt;
static Oscillator osc, lfo;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  saw, freq, output;
    for (size_t i = 0; i < size; i++) {
        freq = 10000 + (lfo.Process() * 10000);
        saw  = osc.Process();

        flt.SetCutoff(freq);
        output = flt.Process(saw);

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = output;
        }
    }
}

void setup() {
    // initialize Biquad and set parameters
    flt.Init(sample_rate);
    flt.SetRes(0.7);

    // set parameters for sine oscillator object
    lfo.Init(sample_rate);
    lfo.SetWaveform(Oscillator::WAVE_TRI);
    lfo.SetAmp(1);
    lfo.SetFreq(.4);

    // set parameters for sine oscillator object
    osc.Init(sample_rate);
    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    osc.SetFreq(100);
    osc.SetAmp(0.25);
}

void loop() {}
