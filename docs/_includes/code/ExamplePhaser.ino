//
//  ExamplePhaser.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeSampler     mSampler;
NodePhaser      mPhaser;
NodeClip        mClip;
NodeDAC         mDAC;

void setup()  {
    Klang::connect(mSampler,  mPhaser);
    Klang::connect(mPhaser,   mClip);
    Klang::connect(mClip,     mDAC);

    mSampler.set_buffer_size(KLANG_SAMPLES_PER_AUDIO_BLOCK);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mSampler.set_buffer(pInputLeft);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    if (event == EVENT_MOUSE_MOVED) {
        mPhaser.set_wet(0.5);
        mPhaser.set_feedback(data[X] * 2.0);
        mPhaser.set_sweeprate(data[Y] * 2.0);
    }
}
