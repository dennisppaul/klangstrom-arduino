//
//  ExampleMixerMultiStereo
//

#include "CycleCounter.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t  NUM_OF_OSC = 12;

NodeDAC                 mDAC;
NodeVCOWavetable        mOSC[NUM_OF_OSC];
NodeMixerMultiStereo    mMixer;

float mAudioblockDuration = 0;
uint8_t mBlocksUsed       = 0;

void setup()  {
    begin_serial_debug(true);

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        Klang::connect(mOSC[i], Node::CH_OUT_SIGNAL, mMixer, i);
    }
    Klang::connect(mMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_LEFT,    mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_RIGHT,   mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        const float mOffset = 0.1;
        mOSC[i].set_frequency(DEFAULT_FREQUENCY * 1.5 * ((NUM_OF_OSC - i) + mOffset));
        mOSC[i].set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
        mOSC[i].set_amplitude(0.15);
        float mPan = (float)i / (NUM_OF_OSC - 1) * 2 - 1;
        mMixer.set_pan(i, mPan);
    }

    mDAC.set_stereo(true);

    beats_per_minute(120);
    klst_enable_cycle_counter();
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mMixer.set_pan(0, data[X] * 2 - 1);
            mMixer.set_pan(1, data[Y] * 2 - 1);
            break;
        case EVENT_MOUSE_DRAGGED:
            mMixer.set_pan(2, data[X] * 2 - 1);
            mMixer.set_pan(3, data[Y] * 2 - 1);
            break;
    }
}

void beat(uint32_t pBeat) {
    Serial.print("duration of audioblock (μs) ........... : ");
    Serial.println(mAudioblockDuration);
    Serial.print("number of audioblock used ............. : ");
    Serial.println(mBlocksUsed);

    uint8_t i = pBeat % NUM_OF_OSC;
    float mPan = mMixer.get_pan(i);
    mPan += 0.05 + i * 0.1;
    mPan = ( mPan > 0.9 ) ? -0.9 : mPan;
    mMixer.set_pan(i, mPan);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    const uint32_t start = klst_get_cycles();
    mDAC.process_frame(pOutputLeft, pOutputRight);
    const uint32_t delta = klst_get_cycles() - start;
    mAudioblockDuration = klst_cyclesToMicros(delta);
    mBlocksUsed = AudioBlockPool::instance().blocks_used_max();
}
