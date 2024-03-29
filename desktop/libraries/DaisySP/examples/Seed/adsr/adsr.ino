// Title: adsr
// Description: Triangle wave controlled by adsr envelope
// Hardware: Daisy Seed
// Author: Ben Sergentanis

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Adsr       env;
Oscillator osc;
Metro      tick;
bool       gate;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  osc_out, env_out;
    for (size_t i = 0; i < size; i++) {
        // When the metro ticks, trigger the envelope to start.
        if (tick.Process()) {
            gate = !gate;
        }

        // Use envelope to control the amplitude of the oscillator.
        env_out = env.Process(gate);
        osc.SetAmp(env_out);
        osc_out = osc.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = osc_out;
        }
    }
}

void setup() {
    env.Init(sample_rate);
    osc.Init(sample_rate);

    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);

    // Set envelope parameters
    env.SetTime(ADSR_SEG_ATTACK, .1);
    env.SetTime(ADSR_SEG_DECAY, .1);
    env.SetTime(ADSR_SEG_RELEASE, .01);

    env.SetSustainLevel(.25);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_TRI);
    osc.SetFreq(220);
    osc.SetAmp(0.25);
}

void loop() {}
