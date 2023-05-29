// Title: StereoDelay
// Description: Applies stereo delay to input signal
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

// two DelayLine of 24000 floats.
DelayLine<float, 24000> del_left, del_right;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        float dry_left, dry_right, wet_left, wet_right;

        // Read Dry from I/O
        dry_left  = in[0][i];
        dry_right = in[1][i];

        // Read Wet from Delay Lines
        wet_left  = del_left.Read();
        wet_right = del_right.Read();

        // Write to Delay with some feedback
        del_left.Write((wet_left * 0.5) + dry_left);
        del_right.Write((wet_right * 0.5) + dry_right);

        // Mix Dry and Wet and send to I/O
        out[0][i] = wet_left * 0.707 + dry_left * 0.707;
        out[1][i] = wet_right * 0.707 + dry_right * 0.707;
    }
}

void setup() {
    // Init Delay Lines
    del_left.Init();
    del_right.Init();

    // Set Delay Times in Samples
    del_left.SetDelay(12000.0f);
    del_right.SetDelay(8000.0f);
}

void loop() {}
