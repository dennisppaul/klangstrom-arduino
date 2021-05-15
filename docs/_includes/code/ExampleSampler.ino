//
//  ExampleSampler.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC         mDAC;
NodeSampler     mSampler;
NodeADSR        mADSR;

float mSamplerBuffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];

void setup()  {
    Klang::connect(mSampler,    Node::CH_OUT_SIGNAL,  mADSR,   Node::CH_IN_SIGNAL);
    Klang::connect(mADSR,       Node::CH_OUT_SIGNAL,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

    mSampler.set_buffer(mSamplerBuffer);
    mSampler.set_buffer_size(KLANG_SAMPLES_PER_AUDIO_BLOCK);
    mSampler.loop(true);
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        const float r = (float)i / KLANG_SAMPLES_PER_AUDIO_BLOCK * TWO_PI * 4.0;
        mSamplerBuffer[i] = sin(r) + sin(r * TWO_PI) * 0.1;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mSampler.set_speed(1.0 + data[X] * 3.0);
            break;
        case EVENT_KEY_PRESSED:
            mADSR.start();
            break;
        case EVENT_KEY_RELEASED:
            mADSR.stop();
            break;
    }
}
