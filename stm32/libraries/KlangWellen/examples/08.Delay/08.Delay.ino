// @TODO Delay is broken, fix it!

#include "ADSR.h"
#include "Delay.h"
#include "KlangWellen.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangstrom;
using namespace klangwellen;

Wavetable fWavetable;
ADSR      fADSR;
Delay     fDelay;

void setup() {
    Serial.begin(115200);
    Serial.println("--------");
    Serial.println("08.Delay");
    Serial.println("--------");

    fWavetable.set_waveform(KlangWellen::WAVEFORM_TRIANGLE);
    fWavetable.set_amplitude(0.4);
    fDelay.set_echo_length(0.05);
    fDelay.set_decay_rate(0.7);
    klangstrom::beats_per_minute(120 * 2);
}

void loop() {}

void beat(uint32_t beat_counter) {
    if (beat_counter % 2 == 0) {
        fWavetable.set_frequency((55.0 * (1 + (beat_counter / 2) % 4)));
        fADSR.start();
    } else {
        fADSR.stop();
    }
    if (beat_counter % 8 == 0) {
        fDelay.set_echo_length(0.05f + 0.045f * ((beat_counter / 8) % 10));
    }
}

void audioblock(float** input_signal, float** output_signal) {
    /* process as float ( mono, single sample ) ... */
    // for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
    //     output_signal[LEFT][i]  = fDelay.process(fADSR.process(fWavetable.process()));
    //     output_signal[RIGHT][i] = output_signal[LEFT][i];
    // }
    /* ... or as block ( stereo, fastest ) */
    fWavetable.process(output_signal[LEFT]);
    fADSR.process(output_signal[LEFT]);
    fDelay.process(output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
