#include "FilterLowPassMoogLadder.h"
#include "Klangstrom.h"
#include "Ramp.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable               fWavetable;
FilterLowPassMoogLadder fFilter;
Ramp                    fRampFilterResonance;
Ramp                    fRampFilterFrequency;
Ramp                    fRampFrequency;

void setup() {
    Serial.begin(115200);
    Serial.println("-------");
    Serial.println("15.Ramp");
    Serial.println("-------");

    fWavetable.set_waveform(KlangWellen::WAVEFORM_SAWTOOTH);
    fWavetable.set_frequency(27.5);
    fFilter.set_resonance(0.3);
    fFilter.set_frequency(1);

    fRampFilterResonance.set_start(0.1);
    fRampFilterResonance.set_end(0.85);
    fRampFilterResonance.set_duration(0.5);
    fRampFilterFrequency.set(1, 2000, 1.0);
    fRampFrequency.set(27.5, 55, 2.0);

    klangstrom::beats_per_minute(120 * 2);
}

void loop() {}

void beat(uint32_t beat_counter) {
    if (beat_counter % 4 == 0) {
        fRampFilterResonance.start();
        fRampFilterFrequency.start();
    }
    if (beat_counter % 8 == 0) {
        fRampFrequency.start();
    }
}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        fWavetable.set_frequency(fRampFrequency.process());
        fFilter.set_resonance(fRampFilterResonance.process());
        fFilter.set_frequency(fRampFilterFrequency.process());
        output_signal[LEFT][i] = fFilter.process(fWavetable.process());
    }
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
