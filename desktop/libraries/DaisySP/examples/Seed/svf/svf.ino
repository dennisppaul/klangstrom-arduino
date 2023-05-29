// Title: svf
// Description: Lowpass out left, highpass out right
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Oscillator osc;
Svf        filt;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sig;
    for (size_t i = 0; i < size; i++) {
        sig = osc.Process();
        filt.Process(sig);

        out[0][i] = filt.Low();
        out[1][i] = filt.High();
    }
}

void setup() {
    // Initialize Oscillator, and set parameters.
    osc.Init(sample_rate);
    osc.SetWaveform(osc.WAVE_POLYBLEP_SAW);
    osc.SetFreq(250.0);
    osc.SetAmp(0.5);
    // Initialize Filter, and set parameters.
    filt.Init(sample_rate);
    filt.SetFreq(500.0);
    filt.SetRes(0.85);
    filt.SetDrive(0.8);
}

void loop() {}
