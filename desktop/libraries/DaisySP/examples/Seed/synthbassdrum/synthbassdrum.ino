
#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

SyntheticBassDrum bd;
Metro             tick;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        float t = tick.Process();
        if (t) {
            bd.SetAccent(random() / (float)RAND_MAX);
            bd.SetDirtiness(random() / (float)RAND_MAX);
            bd.SetDecay(random() / (float)RAND_MAX);
        }
        out[0][i] = out[1][i] = bd.Process(t);
    }
}

void setup() {
    bd.Init(sample_rate);
    bd.SetFreq(50.f);
    bd.SetDirtiness(.5f);
    bd.SetFmEnvelopeAmount(.6f);

    tick.Init(2.f, sample_rate);
}

void loop() {}
