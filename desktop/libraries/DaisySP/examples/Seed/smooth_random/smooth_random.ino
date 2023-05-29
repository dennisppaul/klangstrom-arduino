#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

SmoothRandomGenerator smooth;
Oscillator            osc;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        osc.SetFreq((fabsf(smooth.Process()) * 330.f) + 110.f);
        out[0][i] = out[1][i] = osc.Process();
    }
}

void setup() {
    smooth.Init(sample_rate);
    smooth.SetFreq(.75f);

    osc.Init(sample_rate);
    osc.SetFreq(440.f);
}

void loop() {}
