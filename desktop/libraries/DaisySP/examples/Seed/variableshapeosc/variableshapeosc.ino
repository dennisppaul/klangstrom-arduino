#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

VariableShapeOscillator variosc;
Oscillator              lfo, lfo2;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        float mod  = lfo.Process();
        float mod2 = lfo2.Process();
        variosc.SetSyncFreq(110.f * (mod + 3));
        variosc.SetPW(mod * .8f);
        variosc.SetWaveshape(mod2);

        out[0][i] = out[1][i] = variosc.Process();
    }
}

void setup() {
    variosc.Init(sample_rate);
    variosc.SetSync(true);
    variosc.SetFreq(110.f);

    lfo.Init(sample_rate);
    lfo.SetAmp(1.f);
    lfo.SetFreq(.25f);

    lfo2.Init(sample_rate);
    lfo2.SetAmp(1.f);
    lfo2.SetFreq(.125f);
}

void loop() {}
