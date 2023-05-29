#include "ADSR.h"
#include "Clamp.h"
#include "Klangstrom.h"
#include "Wavetable.h"

using namespace klangstrom;
using namespace klangwellen;

Wavetable fWavetable;
ADSR      fADSR;
Clamp     fClamp;

void setup() {
    fWavetable.set_waveform(KlangWellen::WAVEFORM_SINE);
    fClamp.set_min(-0.1);
    fClamp.set_max(0.1);
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
    fWavetable.process(output_signal[LEFT]);
    fADSR.process(output_signal[LEFT]);
    fClamp.process(output_signal[LEFT]);
    KlangWellen::copy(output_signal[LEFT], output_signal[RIGHT]);
}
