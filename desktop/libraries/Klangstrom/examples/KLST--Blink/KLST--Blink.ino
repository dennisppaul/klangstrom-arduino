//
//  KLST--Blink
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

void setup() {
    Klang::lock();
    Klang::connect(mVCO,    Node::CH_OUT_SIGNAL, mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_amplitude(0.25);
    mVCO.set_frequency(110);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void loop() {
    klst::led(LED_00, true);    // turn LED_00 on ( `true` is ON )
    mVCO.set_amplitude(0.25);    // set amplitude to 50%
    delay(1000);                // wait for a second
    klst::led(LED_00, false);   // turn LED_00 off ( `false` is OFF )
    mVCO.set_amplitude(0.0);    // set amplitude to 0%
    delay(1000);                // wait for a second
}

void audioblock(float* pOutputLeft, float* pOutputRight, float* pInputLeft, float* pInputRight) {
    /* process next audio block */
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
