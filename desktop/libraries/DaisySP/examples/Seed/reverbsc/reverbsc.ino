// Title: reverbsc
// Description: Applies reverb to input signal
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

ReverbSc verb;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        float out1, out2;
        verb.Process(in[0][i], in[1][i], &out1, &out2);

        out[0][i] = out1;
        out[1][i] = out2;
    }
}

void setup() {
    verb.Init(sample_rate);
    verb.SetFeedback(0.85f);
    verb.SetLpFreq(18000.0f);
}

void loop() {}
