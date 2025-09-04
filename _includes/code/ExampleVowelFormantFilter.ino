#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC mDAC;
NodeVowelFormantFilter mFilter;
uint8_t mVowels = 0;

void setup() {
    Klang::lock();
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mFilter, Node::CH_IN_SIGNAL);
    Klang::connect(mFilter, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    mVCO.set_frequency(55.0);
    mVCO.set_amplitude(0.165);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SQUARE);
    Klang::unlock();
}

void loop() {
    mVowels++;
    mVowels %= NodeVowelFormantFilter::NUM_OF_VOWELS;
    mFilter.set_vowel(mVowels);

    led_toggle(LED_00);
    delay(1000);
}

void audioblock(float *output_signal[LEFT], float *output_signal[RIGHT],
                float *input_signal[LEFT], float *input_signal[RIGHT]) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}
