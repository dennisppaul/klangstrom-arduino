#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

GrainletOscillator grainlet, subgrainlet;
Oscillator         lfo;
Metro              tick;

float notes[] = {329.63f, 392.f, 440.f, 392.f,
                 587.33f, 523.25f, 587.33f, 659.25};
int   idx     = 0;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        if (tick.Process()) {
            grainlet.SetFreq(notes[idx] * .5f);
            subgrainlet.SetFreq(notes[idx] * .25f);
            idx = (idx + 1) % 8;
        }

        float sig = fabsf(lfo.Process());
        grainlet.SetShape(sig * .5f);
        grainlet.SetFormantFreq(2000.f - 2000.f * sig);
        subgrainlet.SetShape(.5f - sig * .5f);
        subgrainlet.SetFormantFreq(2000.f * sig);

        out[0][i] = grainlet.Process();
        out[1][i] = subgrainlet.Process();
    }
}

void setup() {
    grainlet.Init(sample_rate);
    grainlet.SetBleed(.5f);

    subgrainlet.Init(sample_rate);
    subgrainlet.SetBleed(1.f);

    grainlet.SetFreq(notes[idx] * .5f);
    subgrainlet.SetFreq(notes[idx] * .25f);

    lfo.Init(sample_rate);
    lfo.SetAmp(1.f);
    lfo.SetFreq(.125f);

    tick.Init(11.f, sample_rate);
}

void loop() {}
