/*
 * this examples demonstrates how to pass a signal from *input* (ADC) directly through to the *output* (DAC).
 * note, that this example assumes a stereo input signal and output signal and therefore sets the DAC to stereo
 * with `set_stereo(true)`.
 * 
 * it also shows how to select the input line with the `option()` method.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC mDAC;
NodeADC mADC;

void setup() {
    option(KLST_OPTION_AUDIO_INPUT, KLST_LINE_IN);

    Klang::lock();

    Klang::connect(mADC, NodeADC::CH_OUT_SIGNAL_LEFT, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mADC, NodeADC::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mDAC.set_stereo(true);

    Klang::unlock();
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mADC.process_frame(pInputLeft, pInputRight);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

/*
 * @note( microphone does not work when run in desktop environment from within Arduino IDE. however, starting via CLI
 * works. check console output for location of sketch
 * ( e.g `/var/folders/_s/0x0jq23x495_jz9c6lyrbm480000gn/T/arduino_build_182037/KLST--ExampleADCtoDAC.ino.exec` ))
 */