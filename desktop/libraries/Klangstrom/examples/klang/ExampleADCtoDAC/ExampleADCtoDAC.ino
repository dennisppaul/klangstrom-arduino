//
//  ExampleADCtoDAC.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC mDAC;
NodeADC mADC;

void setup() {
    Klang::lock();

    Klang::connect(mADC,    NodeADC::CH_OUT_SIGNAL_LEFT, mDAC,  NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mADC,    NodeADC::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mDAC.set_stereo(true);

    Klang::unlock();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mADC.process_frame(pInputLeft, pInputRight);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

// @note(microphone does not work when run in desktop environment from within Arduino IDE. however, starting via CLI works.)
// check console output for location of sketch e.g `/var/folders/_s/0x0jq23x495_jz9c6lyrbm480000gn/T/arduino_build_182037/KLST--ExampleADCtoDAC.ino.exec`
