// jitter module example

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Jitter     jitter;
static Oscillator osc;
bool              gate;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  osc_out, jitter_out;
    for (size_t i = 0; i < size; i++) {
        // Use jitter to control the amplitude of the oscillator.
        jitter_out = jitter.Process();
        osc.SetAmp(jitter_out);
        osc_out = osc.Process();

        out[0][i] = osc_out;
        out[1][i] = osc_out;
    }
}

void setup() {
    osc.Init(sample_rate);

    // set jitter parameters
    jitter.Init(sample_rate);
    jitter.SetAmp(1);
    jitter.SetCpsMin(1);
    jitter.SetCpsMax(25);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_TRI);
    osc.SetFreq(440);
    osc.SetAmp(0.25);
}

void loop() {}
