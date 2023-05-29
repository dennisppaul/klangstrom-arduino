#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Resonator res;
Metro     tick;

float frac;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        float t_sig = tick.Process();
        if (t_sig) {
            res.SetFreq(rand() * frac * 770.f + 110.f);  // 110 - 880
            res.SetStructure(rand() * frac);
            res.SetBrightness(rand() * frac * .7f);
            res.SetDamping(rand() * frac * .7f);

            tick.SetFreq(rand() * frac * 7.f + 1);
        }
        float sig = res.Process(t_sig);
        out[0][i] = out[1][i] = sig;
    }
}

void setup() {
    res.Init(.015, 24, sample_rate);
    res.SetStructure(-7.f);

    tick.Init(1.f, sample_rate);

    frac = 1.f / RAND_MAX;
}

void loop() {}
