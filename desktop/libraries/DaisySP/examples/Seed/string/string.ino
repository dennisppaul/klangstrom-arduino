#include <algorithm>

#include "DaisySP.h"
#include "Klangstrom.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

// Helper Modules
static Metro      tick;
static StringOsc  str;
static Oscillator lfo;

// MIDI note numbers for a major triad
const float kArpeggio[3] = {48.0f, 52.0f, 55.0f};
uint8_t     arp_idx;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    for (size_t i = 0; i < size; i++) {
        // When the Metro ticks:
        // advance the kArpeggio, and trigger the String.
        bool trig = tick.Process();
        if (trig) {
            // convert midi nn to frequency.
            float freq = mtof(kArpeggio[arp_idx]);

            // advance the kArpeggio, wrapping at the end.
            arp_idx = (arp_idx + 1) % 3;
            str.SetFreq(freq);
        }

        // modulate the string parameters using the lfo
        float sig = lfo.Process();

        str.SetBrightness(fabsf(sig));
        str.SetDamping(fabsf(sig) + .2f);
        str.SetNonLinearity(sig);

        // output the sample
        out[0][i] = out[1][i] = str.Process(trig);
    }
}

void setup() {
    // Set up Metro to pulse every second
    tick.Init(2.0f, sample_rate);

    // Set up String algo
    str.Init(sample_rate);
    str.SetDamping(.8f);
    str.SetNonLinearity(.1f);
    str.SetBrightness(.5f);

    // setup lfo at .25Hz
    lfo.Init(sample_rate);
    lfo.SetAmp(1.f);
    lfo.SetFreq(.1f);

    arp_idx = 0;
}

void loop() {}
