// Title: blosc
// Description: Band limited oscillator. Avoids aliasing issues.
// Hardware: Daisy Seed
// Author: Ben Sergentanis

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static BlOsc osc;
static Metro tick;

int waveform = 0;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sig;
    for (size_t i = 0; i < size; i++) {
        // switch waveforms
        if (tick.Process()) {
            waveform++;
            osc.SetWaveform(waveform % 3);
        }

        sig = osc.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sig;
        }
    }
}

void setup() {
    osc.Init(sample_rate);

    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);

    // Set parameters for oscillator;
    osc.SetFreq(440);
    osc.SetAmp(0.5);
    osc.SetPw(.5);
}

void loop() {}
