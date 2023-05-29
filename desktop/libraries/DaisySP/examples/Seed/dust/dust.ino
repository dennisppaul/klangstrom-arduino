#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Dust       dust;
Oscillator lfo;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        dust.SetDensity(fabsf(lfo.Process()));
        out[0][i] = out[1][i] = dust.Process();
    }
}

void setup() {
    dust.Init();
    lfo.Init(sample_rate);
    lfo.SetFreq(.1f);
    lfo.SetAmp(1.f);
}

void loop() {}
