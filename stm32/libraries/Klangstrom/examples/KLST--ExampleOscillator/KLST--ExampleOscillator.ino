//
//  ExampleADSR.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

float mFreq = 55;
int mCounter = 0;

void setup() {
    Klang::lock();

    Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(mFreq);
    mVCO.set_amplitude(0.5);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);

    Klang::unlock();
}

void loop() {
    led(LED_00, true);
    delay(1000);
    led(LED_00, true);
    delay(1000);
    mCounter++;
    if (mCounter > 10) {
        mCounter = 1;
    }
    mVCO.set_frequency(mFreq * mCounter);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
