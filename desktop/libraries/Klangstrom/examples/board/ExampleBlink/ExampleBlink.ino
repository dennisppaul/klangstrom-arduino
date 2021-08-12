//
// ExampleBlink
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeDAC         mDAC;

void setup() {
    Klang::lock();
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_amplitude(0.25);
    mVCO.set_frequency(110);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void loop() {
    led(LED_00, true);          // turn LED_00 on ( `true` is ON )
    mVCO.set_amplitude(0.25);   // set amplitude to 25%
    delay(1000);                // wait for a second
    led(LED_00, false);         // turn LED_00 off ( `false` is OFF )
    mVCO.set_amplitude(0.0);    // set amplitude to 0%
    delay(1000);                // wait for a second
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
