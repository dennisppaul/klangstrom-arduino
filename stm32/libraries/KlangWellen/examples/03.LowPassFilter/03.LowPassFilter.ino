#include "FilterLowPassMoogLadder.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable               fWavetable;
FilterLowPassMoogLadder fFilter;

void setup() {
    Serial.begin(115200);
    Serial.println("----------------");
    Serial.println("03.LowPassFilter");
    Serial.println("----------------");

    fWavetable.set_waveform(KlangWellen::WAVEFORM_SQUARE);
    fWavetable.set_frequency(55);
    fFilter.set_resonance(0.85f);
    klangstrom::beats_per_minute(120 * 4);
}

void loop() {}

void beat(uint32_t beat_counter) {
    fFilter.set_frequency(1.0f + 100.0f * ((beat_counter) % 32));
    fWavetable.set_frequency(55 + 55 * ((beat_counter / 4) % 2));
}

void audioblock(float** input_signal, float** output_signal) {
    /* process as block ... */
    fWavetable.process(output_signal[LEFT]);
    fFilter.process(output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);

    // /* ... or as single samples */
    // for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
    //     output_signal[LEFT][i] = fFilter.process(fWavetable.process());
    // }
    // KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
