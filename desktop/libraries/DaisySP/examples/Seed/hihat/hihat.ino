#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

HiHat<> hihat;
Metro   tick;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        bool t = tick.Process();
        if (t) {
            hihat.SetDecay(random() / (float)RAND_MAX);
            hihat.SetSustain((random() / (float)RAND_MAX) > .8f);
            hihat.SetTone(random() / (float)RAND_MAX);
            hihat.SetNoisiness(random() / (float)RAND_MAX);
        }
        out[0][i] = out[1][i] = hihat.Process(t);
    }
}

void setup() {
    hihat.Init(sample_rate);

    tick.Init(8.f, sample_rate);
}

void loop() {}
