#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Overdrive  drive;
Oscillator osc, lfo;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        drive.SetDrive(fabsf(lfo.Process()));
        float sig = drive.Process(osc.Process());
        out[0][i] = out[1][i] = sig;
    }
}

void setup() {
    osc.Init(sample_rate);
    lfo.Init(sample_rate);
    lfo.SetAmp(.8f);
    lfo.SetWaveform(Oscillator::WAVE_TRI);
    lfo.SetFreq(.25f);
}

void loop() {}
