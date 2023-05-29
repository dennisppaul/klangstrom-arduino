#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Particle   particle;
Oscillator lfo;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        particle.SetDensity(fabsf(lfo.Process()));
        out[0][i] = out[1][i] = particle.Process();
    }
}

void setup() {
    lfo.Init(sample_rate);
    lfo.SetAmp(.5f);
    lfo.SetFreq(.1f);

    particle.Init(sample_rate);
    particle.SetSpread(2.f);
}

void loop() {}
