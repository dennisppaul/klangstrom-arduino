#include "KlangWellen.h"
#include "Klangstrom.h"
#include "SAM.h"
#include "Vocoder.h"
#include "Wavetable.h"

using namespace klangstrom;
using namespace klangwellen;

SAM       fSAM(48000);
Wavetable fWavetable;
Vocoder   fVocoder{13, 3}; // this still works on KLST_SHEEP but only with `fastest` compiler option

void setup() {
    fSAM.speak("hello world");

    fWavetable.set_waveform(KlangWellen::WAVEFORM_SAWTOOTH);
    fWavetable.set_frequency(55);
    fWavetable.set_amplitude(1.0);

    fVocoder.set_reaction_time(0.03);
    fVocoder.set_formant_shift(1.0);
    fVocoder.set_volume(4);

    beats_per_minute(80);
}

void loop() {}

void beat(uint32_t beat_counter) {
    fWavetable.set_frequency(27.5 * KlangWellen::pow(2, beat_counter % 4));
    fSAM.speak_from_buffer();
}

void audioblock(float** input_signal, float** output_signal) {
    float mModulator[KLANG_SAMPLES_PER_AUDIO_BLOCK];
    float mCarrier[KLANG_SAMPLES_PER_AUDIO_BLOCK];
    fSAM.process(mModulator);
    fWavetable.process(mCarrier);
    fVocoder.process(mCarrier, mModulator, output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
