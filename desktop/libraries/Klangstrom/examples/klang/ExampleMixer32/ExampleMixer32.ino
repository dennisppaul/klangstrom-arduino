//
//  ExampleMixer32
//

// NodeMixer32
// duration of audioblock (μs) ........... : 2026.7 ( with -Os ) (20210602 22:38)
// duration of audioblock (μs) ........... : 2004.8 ( with -Os ) (20210601 10:00)
// duration of audioblock (μs) ........... : 1930.3 ( with -O3 ) (20210601 10:01)

// NodeMixer16
// duration of audioblock (μs) ........... : 1019.7 ( with -Os ) (20210602 23:02)
// duration of audioblock (μs) ........... :  930.6 ( with -O3 ) (20210602 23:01)
// ... 12 OSCs
// duration of audioblock (μs) ........... :  821.4

#include "CycleCounter.h"
#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t  NUM_OF_OSC = NodeMixer32::NUM_CH_IN;

NodeDAC             mDAC;
NodeVCOWavetable    mOSC[NUM_OF_OSC];
NodeMixer32         mMixer;

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
