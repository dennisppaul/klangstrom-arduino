//
//  ExampleSharedWavetable
//

#include "CycleCounter.h"
#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t    NUM_OF_OSC          = 4;
static const uint16_t   WAVETABLE_SIZE      = 256;
static const float      FREQ_OFFSET         = 0.02;
static const float      BASE_FREQ           = 110;

float                   mAudioblockDuration = 0;
uint8_t                 mBlocksUsed         = 0;
SIGNAL_TYPE*            mWavetable          = new SIGNAL_TYPE[WAVETABLE_SIZE];

NodeDAC                 mDAC;
NodeVCOWavetable        mOSC[NUM_OF_OSC];
NodeMixerMulti          mMixer;

void setup()  {
    begin_serial_debug(true);

    NodeVCOWavetable::fill_wavetable(NodeVCOWavetable::WAVEFORM::SINE, mWavetable, WAVETABLE_SIZE);
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        mOSC[i] = NodeVCOWavetable(mWavetable, WAVETABLE_SIZE);
        Klang::connect(mOSC[i], Node::CH_OUT_SIGNAL, mMixer, i);
    }
    Klang::connect(mMixer, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    set_frequency(1);

    klst_enable_cycle_counter();
    beats_per_minute(120 * 6);
}

void set_frequency(uint8_t pOffset) {
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        mOSC[i].set_frequency(BASE_FREQ * (i + pOffset + FREQ_OFFSET));
        mOSC[i].set_amplitude(0.7 - (float)i / (float)NUM_OF_OSC * 0.5);
    }
}

void beat(uint32_t pBeat) {
    led_toggle(LED_00);
    set_frequency(random(1, 12));
    if (!(pBeat % 4)) {
        Serial.print("duration of audioblock (μs) ........... : ");
        Serial.println(mAudioblockDuration);
        Serial.print("number of audioblock used ............. : ");
        Serial.println(mBlocksUsed);
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    const uint32_t start = klst_get_cycles();
    mDAC.process_frame(pOutputLeft, pOutputRight);
    const uint32_t delta = klst_get_cycles() - start;
    mAudioblockDuration = klst_cyclesToMicros(delta);
    mBlocksUsed = AudioBlockPool::instance().blocks_used_max();
}
