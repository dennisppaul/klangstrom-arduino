#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Oscillator        osc, lfo;
SampleRateReducer sr;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        sr.SetFreq(fabsf(lfo.Process()));
        out[0][i] = out[1][i] = sr.Process(osc.Process());
    }
}

void setup() {
    osc.Init(sample_rate);
    osc.SetFreq(440.f);

    lfo.Init(sample_rate);
    lfo.SetFreq(.1f);
    lfo.SetAmp(.25f);

    sr.Init();
}

void loop() {}
