#include "FilterLowPassMoogLadder.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable               fLFO;
Wavetable               fWavetable;
FilterLowPassMoogLadder fFilter;

void setup() {
        Serial.begin(115200);
    Serial.println("------");
    Serial.println("09.LFO");
    Serial.println("------");

    fLFO.set_waveform(KlangWellen::WAVEFORM_SINE);
    fLFO.set_oscillation_range(55, 1000);
    fLFO.set_frequency(0.5);
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SQUARE);
    fWavetable.set_frequency(27.5);
    fFilter.set_resonance(0.85f);
}

void loop() {}

void beat(uint32_t beat_counter) {
    fWavetable.set_frequency(27.5 + 27.5 * ((beat_counter / 8) % 2));
}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        fFilter.set_frequency(fLFO.process());
        output_signal[LEFT][i] = fFilter.process(fWavetable.process());
    }
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
