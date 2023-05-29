#include "ADSR.h"
#include "Klangstrom.h"
#include "Reverb.h"
#include "Wavetable.h"

using namespace klangstrom;
using namespace klangwellen;

Wavetable fWavetable;
ADSR      fADSR;
Reverb    fReverb;

void setup() {
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SINE);
    fReverb.set_roomsize(0.9);
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
        // /* process as signal ( stereo, single sample ) ... */
        // const float mono_signal = fWavetable.process() * fADSR.process();
        // Signal      stereo_signal(mono_signal);
        // fReverb.process(stereo_signal);
        // output_signal[LEFT][i]  = stereo_signal.left;
        // output_signal[RIGHT][i] = stereo_signal.right;

        // /* ... or as float ( mono, single sample ) ... */
        // output_signal[LEFT][i]  = fReverb.process(fADSR.process(fWavetable.process()));
        // output_signal[RIGHT][i] = output_signal[LEFT][i];
    }
    /* ... or as block ( stereo, fastest ) */
    fWavetable.process(output_signal[LEFT]);
    fADSR.process(output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
    fReverb.process(output_signal[LEFT], output_signal[RIGHT]);
}
