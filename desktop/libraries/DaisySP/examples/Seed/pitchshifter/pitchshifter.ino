// Title: pitchshifter
// Description: Pitchshift input audio up an octave
// Hardware: Daisy Seed
// Author: Ben Sergentanis

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

PitchShifter ps;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  shifted, unshifted;
    for (size_t i = 0; i < size; i++) {
        unshifted = in[0][i];
        shifted   = ps.Process(unshifted);

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = (chn % 2) == 0 ? shifted : unshifted;
        }
    }
}

void setup() {
    ps.Init(sample_rate);
    // set transposition 1 octave up (12 semitones)
    ps.SetTransposition(12.0f);
}

void loop() {}
