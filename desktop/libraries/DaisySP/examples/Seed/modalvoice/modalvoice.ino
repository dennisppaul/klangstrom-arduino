#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

#include <stdlib.h>

ModalVoice modal;
Metro      tick;
Oscillator lfo;

// A minor pentatonic
float freqs[5] = {440.f, 523.25f, 587.33f, 659.25f, 783.99f};
bool  sus      = false;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        bool t = tick.Process();
        if (t) {
            modal.SetSustain(sus = !sus);
        }

        if (t || modal.GetAux() > .1f) {
            modal.SetFreq(freqs[rand() % 5]);
        }

        float sig = fabsf(lfo.Process());
        modal.SetStructure(sig);
        modal.SetBrightness(.1f + sig * .1f);

        out[0][i] = out[1][i] = modal.Process(t);
    }
}

void setup() {
    modal.Init(sample_rate);
    modal.SetDamping(.5);

    tick.Init(2.f, sample_rate);

    lfo.Init(sample_rate);
    lfo.SetFreq(.005f);
    lfo.SetAmp(1.f);
}

void loop() {}
