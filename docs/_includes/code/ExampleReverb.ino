//
//  ExampleReverb.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#define KLANG_EXT_NODE_REVERB

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC         mDAC;
NodeVCOWavetable   mOsc;
NodeADSR        mADSR;
NodeReverb      mReverb;

uint8_t mFreq = 0;

void setup() {
    Klang::connect(mOsc,    Node::CH_OUT_SIGNAL,  mADSR,   Node::CH_IN_SIGNAL);
    Klang::connect(mADSR,   Node::CH_OUT_SIGNAL,  mReverb, Node::CH_IN_SIGNAL);
    Klang::connect(mReverb, Node::CH_OUT_SIGNAL,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_frequency(DEFAULT_FREQUENCY);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
            mousePressed();
            break;
        case EVENT_MOUSE_RELEASED:
            mouseReleased();
            break;
        case EVENT_MOUSE_MOVED:
            mouseMoved(data[X], data[Y]);
            break;
    }
}

void mouseMoved(float x, float y) {
    mReverb.setroomsize(x);
    mReverb.setwet(y);
    mReverb.setdry(1.0 - y);
}

void mousePressed() {
    mOsc.set_frequency((mFreq + 1)*DEFAULT_FREQUENCY);
    mADSR.start();
    mFreq++;
    mFreq %= 8;
}

void mouseReleased() {
    mADSR.stop();
}
