// Title: pluck
// Description: Plucked major chord
// Hardware: Daisy Seed
// Author: Stephen Hensley

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

// Helper Modules
static Metro tick;
static Pluck plk;

// MIDI note numbers for a major triad
const float kArpeggio[3] = {48.0f, 52.0f, 55.0f};
uint8_t     arp_idx;
float       init_buff[256];  // buffer for Pluck impulse

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sig_out, freq, trig;
    for (size_t i = 0; i < size; i++) {
        // When the Metro ticks:
        // advance the kArpeggio, and trigger the Pluck.
        trig = 0.0f;
        if (tick.Process()) {
            freq = mtof(kArpeggio[arp_idx]);  // convert midi nn to frequency.
            arp_idx =
                (arp_idx + 1) % 3;  // advance the kArpeggio, wrapping at the end.
            plk.SetFreq(freq);
            trig = 1.0f;
        }
        sig_out = plk.Process(trig);
        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sig_out;
        }
    }
}

void setup() {
    // Set up Metro to pulse every half second
    tick.Init(2, sample_rate);
    // Set up Pluck algo
    plk.Init(sample_rate, init_buff, 256, PLUCK_MODE_RECURSIVE);
    plk.SetDecay(0.95f);
    plk.SetDamp(0.9f);
    plk.SetAmp(0.3f);

    arp_idx = 0;
}

void loop() {}
