#include "ADSR.h"
#include "Klangstrom.h"
#include "Trigger.h"
#include "Wavetable.h"

using namespace klangstrom;
using namespace klangwellen;

Wavetable fWavetable;
Wavetable fWavetable_DSP;
Wavetable fLFO;
ADSR      fADSR;
ADSR      fADSR_DSP;
Trigger   fTrigger;

void setup() {
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SINE);
    fWavetable.set_frequency(220.0);
    fWavetable_DSP.set_waveform(KlangWellen::WAVEFORM_TRIANGLE);
    fWavetable_DSP.set_frequency(110.0);

    const static uint8_t pulses_per_beat = 4;

    fLFO.set_waveform(KlangWellen::WAVEFORM_SINE);
    fLFO.set_frequency(pulses_per_beat);

    fTrigger.trigger_rising_edge(true);
    fTrigger.trigger_falling_edge(true);
    fTrigger.set_callback(trigger);

    beats_per_minute(120 * pulses_per_beat);
}

void loop() {}

void beat(uint32_t beat_counter) {
    LED(LED_00, LED_TOGGLE);
    if (beat_counter % 2) {
        fADSR.start();
        LED(LED_00, LED_ON);
    } else {
        fADSR.stop();
        LED(LED_00, LED_OFF);
    }
}

void trigger(uint8_t event) {
    if (event == Trigger::EVENT_RISING_EDGE) {
        fADSR_DSP.start();
        LED(LED_01, LED_ON);
    } else {
        fADSR_DSP.stop();
        LED(LED_01, LED_OFF);
    }
}

void audioblock(float** input_signal, float** output_signal) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        fTrigger.process(fLFO.process());
        output_signal[LEFT][i]  = fWavetable.process() * fADSR.process();
        output_signal[RIGHT][i] = fWavetable_DSP.process() * fADSR_DSP.process();
    }
}
