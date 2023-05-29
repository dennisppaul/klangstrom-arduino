// Title: bitcrush
// Description: Steps through different bitdepth and downsample settings
// Hardware: Daisy Seed
// Author: Ben Sergentanis

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Oscillator osc, lfo;
static Bitcrush   bitcrush;
static Metro      metro;

uint8_t depth;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    float  sig;
    for (size_t i = 0; i < size; i++) {
        sig = osc.Process();

        if (metro.Process()) {
            depth++;
            depth %= 6;
        }

        bitcrush.SetBitDepth(depth + 2);
        bitcrush.SetCrushRate((depth + 2) * 2000);
        sig = bitcrush.Process(sig);

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sig;
        }
    }
}

void setup() {
    depth = 1;
    osc.Init(sample_rate);
    bitcrush.Init(sample_rate);
    metro.Init(1.0f, sample_rate);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(440);
    osc.SetAmp(0.5);

    // set parameters for bitcrusher
    bitcrush.SetBitDepth(6);
    bitcrush.SetCrushRate(10000);
}

void loop() {}
