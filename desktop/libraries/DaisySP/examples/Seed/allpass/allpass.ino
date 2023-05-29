// Title: allpass
// Description: Allpass filter frequency is swept with sine wave oscillator as
// input Hardware: Daisy Seed Author: Ben Sergentanis

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

float sample_rate = KLANG_AUDIO_RATE;

static Oscillator osc, lfo;
static Allpass    allpass;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sig;
    for (size_t i = 0; i < size; i++) {
        sig     = osc.Process();
        float l = .01 + lfo.Process();
        allpass.SetFreq(l);
        sig = allpass.Process(sig);

        out[0][i] = out[1][i] = sig;
    }
}

void setup() {
    osc.Init(sample_rate);
    lfo.Init(sample_rate);

    float buff[9600];
    for (int i = 0; i < 9600; i++) {
        buff[i] = 0.0f;
    }

    allpass.Init(sample_rate, buff, (int)9600);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(0.5);

    // Set parameters for oscillator
    lfo.SetWaveform(osc.WAVE_SIN);
    lfo.SetFreq(1);
    lfo.SetAmp(0.01);
}

void loop() {}
