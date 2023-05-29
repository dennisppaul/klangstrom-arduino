// Title: adenv
// Description: Triangle wave controlled by AD envelope
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Metro      tick;
static AdEnv      adenv;
static Oscillator osc;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        if (tick.Process()) {
            adenv.Trigger();
        }

        float env_out = adenv.Process();
        float sig     = osc.Process();
        sig *= env_out;

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sig;
        }
    }
}

void setup() {
    adenv.Init(sample_rate);
    tick.Init(1, sample_rate);
    osc.Init(sample_rate);

    // Set oscillator parameters
    osc.SetFreq(440);
    osc.SetAmp(0.5);
    osc.SetWaveform(osc.WAVE_TRI);

    // Set envelope parameters
    adenv.SetTime(ADENV_SEG_ATTACK, 0.15);
    adenv.SetTime(ADENV_SEG_DECAY, 0.35);
    adenv.SetMin(0.0);
    adenv.SetMax(0.25);
    adenv.SetCurve(0);  // linear
}

void loop() {}
