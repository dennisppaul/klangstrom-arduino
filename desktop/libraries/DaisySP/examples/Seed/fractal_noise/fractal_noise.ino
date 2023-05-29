#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

FractalRandomGenerator<ClockedNoise, 5> fract;
Oscillator                              lfo[2];

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        fract.SetFreq(fabsf(lfo[0].Process()));
        fract.SetColor(fabsf(lfo[1].Process()));
        out[0][i] = out[1][i] = fract.Process();
    }
}

void setup() {
    fract.Init(sample_rate);
    fract.SetFreq(sample_rate / 10.f);

    lfo[0].Init(sample_rate);
    lfo[1].Init(sample_rate);

    lfo[0].SetFreq(.25f);
    lfo[0].SetAmp(sample_rate / 3.f);
    lfo[1].SetFreq(.1f);
    lfo[1].SetAmp(1.f);
}

void loop() {}
