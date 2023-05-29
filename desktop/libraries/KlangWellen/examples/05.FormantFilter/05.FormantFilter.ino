#include "FilterVowelFormant.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable          fWavetable;
FilterVowelFormant fFilter;

void setup() {
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SAWTOOTH);
    fWavetable.set_frequency(35);
    fWavetable.set_amplitude(0.125f);
    fFilter.set_vowel(FilterVowelFormant::VOWEL_U);
    klangstrom::beats_per_minute(120 * 10);
}

void loop() {}

void beat(uint32_t beat_counter) {
    static const int mLerpDuration = 10;
    const uint8_t    mVowel        = (beat_counter / mLerpDuration) % (FilterVowelFormant::NUM_OF_VOWELS - 1);
    const uint8_t    mVowelOffset  = beat_counter / (mLerpDuration * 5);
    const uint8_t    mNextVowel    = (mVowel + mVowelOffset) % (FilterVowelFormant::NUM_OF_VOWELS - 1);
    float            lerp          = (beat_counter % mLerpDuration) / (float)mLerpDuration;
    fFilter.lerp_vowel(mVowel, mNextVowel, lerp);
    fWavetable.set_frequency(35 + ((beat_counter / 3) % 5) * 4);
    fWavetable.set_amplitude(((beat_counter / 2) % 13) / 13.0 * 0.1 + 0.05);
}

void audioblock(float** input_signal, float** output_signal) {
    fWavetable.process(output_signal[LEFT]);
    fFilter.process(output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
