#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

ZOscillator zosc;
Oscillator  lfo1, lfo2;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        float mod1 = lfo1.Process();
        float mod2 = lfo2.Process();

        zosc.SetFormantFreq(250.f * (1.f + mod1 * .8f));
        zosc.SetMode(mod2);

        out[0][i] = out[1][i] = zosc.Process();
    }
}

void setup() {
    zosc.Init(sample_rate);
    zosc.SetFreq(80.f);
    zosc.SetShape(1.f);

    lfo1.Init(sample_rate);
    lfo1.SetAmp(1.f);
    lfo1.SetFreq(1.f);

    lfo2.Init(sample_rate);
    lfo2.SetAmp(1.f);
    lfo2.SetFreq(.5f);
}

void loop() {}
