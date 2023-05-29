#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

ClockedNoise noise;
Oscillator   lfo;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        noise.SetFreq(fabsf(lfo.Process()));
        out[0][i] = out[1][i] = noise.Process();
    }
}

void setup() {
    noise.Init(sample_rate);

    lfo.Init(sample_rate);
    lfo.SetAmp(sample_rate / 3.f);
    lfo.SetFreq(.125f);
}

void loop() {}
