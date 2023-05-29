#include "KlangWellen.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable fWavetable;

void setup() {
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SAWTOOTH, 16);
    fWavetable.set_frequency(55);
    fWavetable.set_amplitude(0.5);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        output_signal[LEFT][i] = fWavetable.process();
    }
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}