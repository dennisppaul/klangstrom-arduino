#include "ADSR.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangstrom;
using namespace klangwellen;

Wavetable fWavetable;
ADSR      fADSR;

void setup() {
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SINE);
}

void loop() {}

void beat(uint32_t beat_counter) {
    if (beat_counter % 2) {
        fADSR.start();
    } else {
        fADSR.stop();
    }
}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        output_signal[LEFT][i] = fWavetable.process() * fADSR.process();
    }
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
