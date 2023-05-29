// Title: decimator
// Description: Generates a pure sine wave, and then crushes 8 bits, and
// variably downsamples the tone. Hardware: Daisy Seed Author: Stephen Hensley

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Oscillator osc;
static Decimator  decim;
static Phasor     phs;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  osc_out, decimated_out;
    float  downsample_amt;
    for (size_t i = 0; i < size; i++) {
        // Generate a pure sine wave
        osc_out = osc.Process();
        // Modulate downsample amount via Phasor
        downsample_amt = phs.Process();
        decim.SetDownsampleFactor(downsample_amt);
        // downsample and bitcrush
        decimated_out = decim.Process(osc_out);
        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = decimated_out;
        }
    }
}

void setup() {
    osc.Init(sample_rate);
    phs.Init(sample_rate, 0.5f);
    decim.Init();

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(220);
    osc.SetAmp(0.25);
    // Set downsampling, and bit crushing values.
    decim.SetDownsampleFactor(0.4f);
    decim.SetBitsToCrush(8);
}

void loop() {}
