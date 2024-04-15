#include "Klangstrom.h"
#include "Sampler.h"

using namespace klangwellen;

Sampler fSampler{KlangWellen::DEFAULT_SAMPLING_RATE / 8};

void setup() {
    Serial.begin(115200);
    Serial.println("----------");
    Serial.println("07.Sampler");
    Serial.println("----------");

    for (int32_t i = 0; i < fSampler.get_buffer_length(); i++) {
        float ratio = 1.0 - (float)i / fSampler.get_buffer_length();
        // fSampler.get_buffer()[i] = KlangWellen::random() * 0.2f * ratio * 127; // for SamplerI8
        fSampler.get_buffer()[i] = KlangWellen::random() * 0.2f * ratio;
    }
    klangstrom::beats_per_minute(120 * 4);
}

void loop() {}

void beat(uint32_t beat_counter) {
    if (beat_counter % 2) {
        fSampler.rewind();
        fSampler.play();
    } else {
        fSampler.stop();
    }
}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        output_signal[LEFT][i] = fSampler.process();
    }
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
