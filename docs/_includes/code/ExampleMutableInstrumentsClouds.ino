#include "KlangNodes.hpp"
#include "NodeMutableInstrumentsClouds.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC mDAC;
NodeADC mADC;
NodeMutableInstrumentsClouds mClouds;

void setup() {
    KLANG_LOG("KLANG_SAMPLES_PER_AUDIO_BLOCK: %i", KLANG_SAMPLES_PER_AUDIO_BLOCK);
    Klang::lock();

    Klang::connect(mADC,    NodeADC::CH_OUT_SIGNAL,  mClouds, Node::CH_IN_SIGNAL);
    Klang::connect(mClouds, Node::CH_OUT_SIGNAL,     mDAC,    NodeDAC::CH_IN_SIGNAL);

    Klang::unlock();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mADC.process_frame(pInputLeft, pInputRight);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
