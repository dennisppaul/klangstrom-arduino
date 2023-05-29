// Title: analogbassdrum
// Description: Demonstrates AnalogBassDrum with randomized parameters.
// Hardware: Daisy Seed
// Author: Ben Sergentanis

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

float sample_rate = KLANG_AUDIO_RATE;

AnalogBassDrum bd;
Metro          tick;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        bool t = tick.Process();
        if (t) {
            bd.SetTone(.7f * random() / (float)RAND_MAX);
            bd.SetDecay(random() / (float)RAND_MAX);
            bd.SetSelfFmAmount(random() / (float)RAND_MAX);
        }

        out[0][i] = out[1][i] = bd.Process(t);
    }
}

void setup() {
    bd.Init(sample_rate);
    bd.SetFreq(50.f);

    tick.Init(2.f, sample_rate);
}

void loop() {}
