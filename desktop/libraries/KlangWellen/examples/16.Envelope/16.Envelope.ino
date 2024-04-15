#include "Envelope.h"
#include "FilterLowPassMoogLadder.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangwellen;

Wavetable fWavetable;
Envelope  fFrequencyEnvelope;
Envelope  fAmplitudeEnvelope;

void setup() {
    Serial.begin(115200);
    Serial.println("-----------");
    Serial.println("16.Envelope");
    Serial.println("-----------");

    fWavetable.set_waveform(KlangWellen::WAVEFORM_SAWTOOTH);
    fWavetable.set_frequency(55.0);
    fWavetable.set_amplitude(0.0);

    fFrequencyEnvelope.add_stage(55.0, 0.5);
    fFrequencyEnvelope.add_stage(110.0);

    fAmplitudeEnvelope.add_stage(0.0, 0.25);
    fAmplitudeEnvelope.add_stage(0.3, 0.125);
    fAmplitudeEnvelope.add_stage(0.0, 0.125);
    fAmplitudeEnvelope.add_stage(0.6, 0.1);
    fAmplitudeEnvelope.add_stage(0.0);
    fAmplitudeEnvelope.enable_loop(true);
    fAmplitudeEnvelope.start();

    klangstrom::beats_per_minute(120 * 2);
}

void loop() {}

void beat(uint32_t beat_counter) {
    if (beat_counter % 4 == 0) {
        fFrequencyEnvelope.start();
    }
}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        fWavetable.set_frequency(fFrequencyEnvelope.process());
        fWavetable.set_amplitude(fAmplitudeEnvelope.process());
        output_signal[LEFT][i] = fWavetable.process();
    }
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
