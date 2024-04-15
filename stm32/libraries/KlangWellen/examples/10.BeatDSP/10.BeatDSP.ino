#include "ADSR.h"
#include "BeatDSP.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangstrom;
using namespace klangwellen;

Wavetable fWavetable;
Wavetable fWavetable_DSP;
ADSR      fADSR;
ADSR      fADSR_DSP;
BeatDSP   fBeat;

void setup() {
    Serial.begin(115200);
    Serial.println("----------");
    Serial.println("10.BeatDSP");
    Serial.println("----------");

    fWavetable.set_waveform(KlangWellen::WAVEFORM_SINE);
    fWavetable.set_frequency(220.0);
    fWavetable_DSP.set_waveform(KlangWellen::WAVEFORM_TRIANGLE);
    fWavetable_DSP.set_frequency(110.0);

    fBeat.set_callback(beat_dsp);
    fBeat.set_bpm(120 * 2);

    beats_per_minute(120 * 2);
}

void loop() {}

void beat(uint32_t beat_counter) {
    if (beat_counter % 2) {
        fADSR.start();
        LED(LED_00, LED_ON);
    } else {
        fADSR.stop();
        LED(LED_00, LED_OFF);
    }
}

void beat_dsp(uint32_t beat_counter) {
    if (beat_counter % 2) {
        fADSR_DSP.start();
        LED(LED_01, LED_ON);
    } else {
        fADSR_DSP.stop();
        LED(LED_01, LED_OFF);
    }
}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        fBeat.process();
        output_signal[LEFT][i]  = fWavetable.process() * fADSR.process();
        output_signal[RIGHT][i] = fWavetable_DSP.process() * fADSR_DSP.process();
    }
}
