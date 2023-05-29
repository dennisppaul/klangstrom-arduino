#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

VosimOscillator vosim;
Oscillator      lfo;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        float mod = lfo.Process();
        vosim.SetForm2Freq(817.2f * (mod + 1.f));
        vosim.SetShape(mod);

        out[0][i] = out[1][i] = vosim.Process();
    }
}

void setup() {
    vosim.Init(sample_rate);
    vosim.SetFreq(105.f);
    vosim.SetForm1Freq(1390.7);

    lfo.Init(sample_rate);
    lfo.SetAmp(1.f);
    lfo.SetFreq(.5f);
}

void loop() {}
