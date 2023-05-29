// Title: dcblock
// Description: Removes offset from signals
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static DcBlock    block;
static Oscillator osc_sine;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  output;
    for (size_t i = 0; i < size; i++) {
        output = osc_sine.Process();

        // add dc to signal
        output += 1;

        // remove dc from signal
        output = block.Process(output);

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = output;
        }
    }
}

void setup() {
    block.Init(sample_rate);

    // set parameters for sine oscillator object
    osc_sine.Init(sample_rate);
    osc_sine.SetWaveform(Oscillator::WAVE_SIN);
    osc_sine.SetFreq(100);
    osc_sine.SetAmp(0.25);
}

void loop() {}
