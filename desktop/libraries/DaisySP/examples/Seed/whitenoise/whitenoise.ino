// Title: whitenoise
// Description: Generates whitenoise
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static WhiteNoise nse;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sig;
    for (size_t i = 0; i < size; i++) {
        sig = nse.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sig;
        }
    }
}

void setup() {
    nse.Init();
}

void loop() {}
