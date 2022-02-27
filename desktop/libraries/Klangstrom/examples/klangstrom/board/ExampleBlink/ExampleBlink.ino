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
    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    mVCO.set_amplitude(0.5);
    mVCO.set_frequency(440.0);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
    Klang::unlock();
}

void loop() {
    LED(LED_00, LED_ON);        // turn LED_00 on 
    mVCO.set_amplitude(0.25);   // set amplitude to 25%
    delay(1000);                // wait for a second
    LED(LED_00, LED_OFF);       // turn LED_00 off 
    mVCO.set_amplitude(0.0);    // set amplitude to 0%
    delay(1000);                // wait for a second
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
