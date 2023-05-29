#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static FormantOscillator form[2];
static Metro             tick;
static Oscillator        lfo;

float freqs[3][2] = {{390, 2300}, {610, 1900}, {820, 1530}};
int   idx         = 0;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        if (tick.Process()) {
            idx = (idx + 1) % 3;
        }

        form[0].SetPhaseShift(lfo.Process());
        form[1].SetPhaseShift(lfo.Process());

        form[0].SetFormantFreq(freqs[idx][0]);
        form[1].SetFormantFreq(freqs[idx][1]);
        out[0][i] = out[1][i] = (form[0].Process() * .6f + form[1].Process() * .4f);
    }
}

void setup() {
    form[0].Init(sample_rate);
    form[0].SetCarrierFreq(200.f);

    form[1].Init(sample_rate);
    form[1].SetCarrierFreq(200.f);

    lfo.Init(sample_rate);
    lfo.SetAmp(1.f);
    lfo.SetFreq(.2f);

    tick.Init(1.f, sample_rate);
}

void loop() {}
