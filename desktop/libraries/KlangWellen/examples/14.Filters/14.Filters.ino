#include "Filter.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable fWavetable;
Filter    fFilter;

void setup() {
    Serial.begin(115200);
    Serial.println("----------");
    Serial.println("14.Filters");
    Serial.println("----------");

    fWavetable.set_waveform(KlangWellen::WAVEFORM_SQUARE);
    fWavetable.set_frequency(55);
    klangstrom::beats_per_minute(120 * 4);
}

void loop() {}

void beat(uint32_t beat_counter) {
    fWavetable.set_frequency(55 + 55 * ((beat_counter / 4) % 2));
    if (beat_counter % 8 == 0) {
        fFilter.set((beat_counter / 8) % Filter::NUM_FILTER_TYPES, 0.0, 1000, 2);
    }
}

void audioblock(float** input_signal, float** output_signal) {
    fWavetable.process(output_signal[LEFT]);
    fFilter.process(output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
