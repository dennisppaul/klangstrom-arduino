#include "Klangstrom.h"
#include "Waveshaper.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable  fWavetable;
Waveshaper fWaveshaper;
uint8_t    fWaveshaperType = Waveshaper::SIN;

void setup() {
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SINE);
    fWavetable.set_amplitude(4.0);
    fWavetable.set_frequency(55);

    fWaveshaper.set_type(Waveshaper::ATAN);
    fWaveshaper.set_output_gain(0.2);

    klangstrom::beats_per_minute(120 * 4);
}

void loop() {}

void beat(uint32_t beat_counter) {
    fWavetable.set_frequency(55 + 55 * ((beat_counter / 4) % 2));
    if (beat_counter % 8 == 0) {
        fWaveshaperType++;
        fWaveshaperType %= Waveshaper::NUM_WAVESHAPER_TYPES;
        fWaveshaper.set_type(fWaveshaperType);
    }
}

void audioblock(float** input_signal, float** output_signal) {
    fWavetable.process(output_signal[LEFT]);
    fWaveshaper.process(output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
