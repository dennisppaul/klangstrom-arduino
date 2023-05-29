// Title: autowah
// Description: Wahs automatically
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static AdEnv      env;
static Oscillator osc;
static Metro      tick;
static Autowah    autowah;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  osc_out, env_out;
    for (size_t i = 0; i < size; i++) {
        // When the metro ticks, trigger the envelope to start.
        if (tick.Process()) {
            env.Trigger();
        }

        // Use envelope to control the amplitude of the oscillator.
        // Apply autowah on the signal.
        env_out = env.Process();
        osc.SetAmp(env_out);
        osc_out = osc.Process();
        osc_out = autowah.Process(osc_out);

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = osc_out;
        }
    }
}

void setup() {
    env.Init(sample_rate);
    osc.Init(sample_rate);
    autowah.Init(sample_rate);

    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);

    // set adenv parameters
    env.SetTime(ADENV_SEG_ATTACK, 0.01);
    env.SetTime(ADENV_SEG_DECAY, 0.5);
    env.SetMin(0.0);
    env.SetMax(0.5);
    env.SetCurve(0);  // linear

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SAW);
    osc.SetFreq(220);
    osc.SetAmp(0.25);

    // set autowah parameters
    autowah.SetLevel(.1);
    autowah.SetDryWet(100);
    autowah.SetWah(1);
}

void loop() {}
