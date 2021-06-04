//
//  ExampleMixer4Stereo
//

#include "CycleCounter.h"
#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOscA;
NodeVCOWavetable    mOscB;
NodeVCOWavetable    mOscC;
NodeVCOWavetable    mOscD;
NodeMixer4Stereo    mMixer;

float mAudioblockDuration = 0;
uint8_t mBlocksUsed       = 0;

void setup()  {
    Serial.begin(115200);
#if KLST_BOARD_TYPE==KLST_TINY
    while (!Serial);
#endif
    klst_enable_cycle_counter();


    Klang::connect(mOscA,     Node::CH_OUT_SIGNAL,                    mMixer,     NodeMixer4::CH_IN_SIGNAL_0);
    Klang::connect(mOscB,     Node::CH_OUT_SIGNAL,                    mMixer,     NodeMixer4::CH_IN_SIGNAL_1);
    Klang::connect(mOscC,     Node::CH_OUT_SIGNAL,                    mMixer,     NodeMixer4::CH_IN_SIGNAL_2);
    Klang::connect(mOscD,     Node::CH_OUT_SIGNAL,                    mMixer,     NodeMixer4::CH_IN_SIGNAL_3);
    Klang::connect(mMixer,    NodeMixer4Stereo::CH_OUT_SIGNAL_LEFT,   mDAC,       NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mMixer,    NodeMixer4Stereo::CH_OUT_SIGNAL_RIGHT,  mDAC,       NodeDAC::CH_IN_SIGNAL_RIGHT);

    const float mOffset = 0.04;
    mOscA.set_frequency(DEFAULT_FREQUENCY * (4.0 + mOffset));
    mOscA.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscA.set_amplitude(0.2);
    mOscB.set_frequency(DEFAULT_FREQUENCY * (2.0 + mOffset));
    mOscB.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscB.set_amplitude(0.4);
    mOscC.set_frequency(DEFAULT_FREQUENCY * (1.0 + mOffset));
    mOscC.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscC.set_amplitude(0.6);
    mOscD.set_frequency(DEFAULT_FREQUENCY * (0.5 + mOffset));
    mOscD.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mOscD.set_amplitude(0.8);

    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_0, 0.4);
    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_1, 0.55);
    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_2, 0.8);
    mMixer.set_mix(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_3, 1.0);

    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_0, -1.0);
    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_1, -0.5);
    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_2, 0.0);
    mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_3, 1.0);

    mDAC.set_stereo(true);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_0, data[X] * 2 - 1);
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_1, data[Y] * 2 - 1);
            break;
        case EVENT_MOUSE_DRAGGED:
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_2, data[X] * 2 - 1);
            mMixer.set_pan(NodeMixer4Stereo::SIGNAL_CHANNEL::SIGNAL_3, data[Y] * 2 - 1);
            break;
    }
}

void beat(uint32_t pBeat) {
    Serial.print("duration of audioblock (μs) ........... : ");
    Serial.println(mAudioblockDuration);
    Serial.print("number of audioblock used ............. : ");
    Serial.println(mBlocksUsed);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    const uint32_t start = klst_get_cycles();
    mDAC.process_frame(pOutputLeft, pOutputRight);
    const uint32_t delta = klst_get_cycles() - start;
    mAudioblockDuration = klst_cyclesToMicros(delta);
    mBlocksUsed = AudioBlockPool::instance().blocks_used_max();
}
