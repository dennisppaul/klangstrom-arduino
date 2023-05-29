// Title: oscillator
// Description: Control a sine wave freq with a knob
// Hardware: Daisy Seed
// Author: Ben Sergentanis
// Breadboard
// https://raw.githubusercontent.com/electro-smith/DaisyExamples/master/seed/Osc/resources/Osc_bb.png
// Schematic:
// https://raw.githubusercontent.com/electro-smith/DaisyExamples/master/seed/Osc/resources/Osc_schem.png

#include "Klangstrom.h"
#include "DaisySP.h"

using namespace daisysp;

size_t num_channels = KLANG_OUTPUT_CHANNELS;
float  sample_rate  = KLANG_AUDIO_RATE;

static Oscillator osc;

float pitchknob;

void audioblock(float** in, float** out) {
    size_t size = KLANG_SAMPLES_PER_AUDIO_BLOCK;
    // Convert Pitchknob MIDI Note Number to frequency
    osc.SetFreq(mtof(pitchknob));
    for (size_t i = 0; i < size; i++) {
        float sig = osc.Process();

        for (size_t chn = 0; chn < num_channels; chn++) {
            out[chn][i] = sig;
        }
    }
}

void setup() {
    osc.Init(sample_rate);
    osc.SetFreq(440);
    osc.SetAmp(0.5);
    osc.SetWaveform(osc.WAVE_TRI);
}

void loop() { pitchknob = 24.0 + ((analogRead(0) / 1023.0) * 60.0); }
