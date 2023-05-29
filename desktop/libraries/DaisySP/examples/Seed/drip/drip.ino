#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Drip  drip;
static Metro tick;
bool         gate;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  output;
    bool   trig;
    for (size_t i = 0; i < size; i++) {
        // When the metro ticks, trigger the envelope to start.
        if (tick.Process()) {
            trig = true;
        } else {
            trig = false;
        }

        output = drip.Process(trig);

        out[0][i] = out[1][i] = output;
    }
}

void setup() {
    // Set up metro to pulse every second
    tick.Init(1.0f, sample_rate);

    drip.Init(sample_rate, .1);
}

void loop() {}
