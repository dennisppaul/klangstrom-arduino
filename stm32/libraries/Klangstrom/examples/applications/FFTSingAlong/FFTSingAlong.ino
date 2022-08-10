//
//  AppFFTSingAlong
//

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOWavetable mVCO;
NodeDAC          mDAC;
NodeFFT          mFFT;
NodeADC          mADC;

void setup() {
    option(KLST_OPTION_AUDIO_INPUT, KLST_LINE_IN);

    Klang::lock();

    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    Klang::connect(mADC, NodeADC::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mVCO.set_frequency(0);
    mVCO.set_amplitude(0.25);
    mVCO.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mFFT.enable_hamming_window(true);
    mFFT.enable_inline_analysis(false);

    mDAC.set_stereo(true);

    Klang::unlock();

    beats_per_minute(480);
}

void beat(uint32_t pBeat) {
    mFFT.perform_analysis();
    float mFrequency = mFFT.get_frequency_gaussian_interpolation();
    Serial.print("detected frequency ... : ");
    Serial.print(mFrequency);
    Serial.print(" > ");
    Serial.print(KLANG_NODE_FFT_BUFFER_SIZE);
    Serial.print(" > ");
    Serial.print(KLANG_NODE_FFT_BUFFER_EXPANSION);
    Serial.println();
    mVCO.set_frequency(mFrequency);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mADC.process_frame(pInputLeft, pInputRight);
    mFFT.update(pInputLeft);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
