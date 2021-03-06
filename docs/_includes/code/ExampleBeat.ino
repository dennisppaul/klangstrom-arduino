//
//  ExampleBeat
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;
NodeADSR            mADSR;

uint8_t mInterval = 12;

void setup()  {
    Klang::connect(mOsc,    Node::CH_OUT_SIGNAL,  mADSR,   Node::CH_IN_SIGNAL);
    Klang::connect(mADSR,   Node::CH_OUT_SIGNAL,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);

    mADSR.set_attack(0.01);
    mADSR.set_decay(0.01);
    mADSR.set_sustain(0.75);
    mADSR.set_release(0.01);

    beats_per_minute(150 * 24);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    if (event == EVENT_MOUSE_MOVED) {
        mInterval = floor(data[X] * 3 + 1) * 6;
    }
}

void beat(uint32_t pBeat)  {
    if (mInterval > 0) {
        if (!(pBeat % mInterval)) {
            uint8_t mNote = ( pBeat / mInterval ) % 5;
            mOsc.set_frequency(DEFAULT_FREQUENCY * (1 + mNote));
            mADSR.start();
        } else if (!(pBeat % (mInterval / 2))) {
            mADSR.stop();
        }
    }
}
