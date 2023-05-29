#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

Comb              flt;
static WhiteNoise noise;

float buf[9600];

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  output;
    for (size_t i = 0; i < size; i++) {
        output = noise.Process();

        output = 0.5 * flt.Process(output);

        out[0][i] = out[1][i] = output;
    }
}

void setup() {
    for (int i = 0; i < 9600; i++) {
        buf[i] = 0.0f;
    }

    // initialize Comb object
    flt.Init(sample_rate, buf, 9600);
    flt.SetFreq(500);

    // initialize noise
    noise.Init();
}

void loop() {}
