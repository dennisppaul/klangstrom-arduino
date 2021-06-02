//
//  ExampleMixer32
//

#include "CycleCounter.h"
#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t  NUM_OF_OSC = 4;//NodeMixer32::NUM_CH_IN;

NodeDAC             mDAC;
NodeVCOWavetable    mOSC[NUM_OF_OSC];
NodeMixer4          mMixer;

float mAudioblockDuration = 0;
uint8_t mBlocksUsed       = 0;

void setup()  {
    Serial.begin(115200);
#if KLST_BOARD_TYPE==KLST_TINY
    while (!Serial);
#endif

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        Klang::connect(mOSC[i], Node::CH_OUT_SIGNAL, mMixer, i);
    }
    Klang::connect(mMixer, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        const float mOffset = 0.02;
        mOSC[i].set_frequency(DEFAULT_FREQUENCY * ((NUM_OF_OSC - i) + mOffset));
        mOSC[i].set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    }
    klst_enable_cycle_counter();
}

void beat(uint32_t pBeat) {
    Serial.print("duration of audioblock (μs) ........... : ");
    Serial.println(mAudioblockDuration);
    Serial.print("number of audioblock used ............. : ");
    Serial.println(mBlocksUsed);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    const uint32_t start = klst_get_cycles();
    mDAC.process_frame(pOutputLeft, pOutputRight);
    const uint32_t delta = klst_get_cycles() - start;
    mAudioblockDuration = klst_cyclesToMicros(delta);
    mBlocksUsed = AudioBlockPool::instance().blocks_used_max();
}
