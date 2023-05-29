
#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

SyntheticSnareDrum sd;
Metro              tick;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        bool t = tick.Process();
        if (t) {
            sd.SetAccent(random() / (float)RAND_MAX);
            sd.SetDecay(random() / (float)RAND_MAX);
            sd.SetSnappy(random() / (float)RAND_MAX);
        }
        out[0][i] = out[1][i] = sd.Process(t);
    }
}

void setup() {
    tick.Init(2.f, sample_rate);

    sd.Init(sample_rate);
}

void loop() {}
