#include "Klangstrom.h"

using namespace klangstrom;

void setup() {
    option(KLST_OPTION_AUDIO_INPUT, KLST_MIC);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        output_signal[LEFT][i]  = input_signal[LEFT][i];
        output_signal[RIGHT][i] = input_signal[RIGHT][i];
    }
}
