//
//  ExampleNoise
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC         mDAC;
NodeNoise       mNoise;
NodeADSR        mADSR;

void setup()  {
    Klang::connect(mNoise,  Node::CH_OUT_SIGNAL,  mADSR,   Node::CH_IN_SIGNAL);
    Klang::connect(mADSR,   Node::CH_OUT_SIGNAL,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mNoise.set_stride(data[X] * 0.5);
            break;
        case EVENT_KEY_PRESSED:
            handle_key_pressed(data[KEY]);
            mADSR.start();
            break;
        case EVENT_KEY_RELEASED:
            mADSR.stop();
            break;
    }
}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            mNoise.set_type(NodeNoise::TYPE::RANDOM);
            break;
        case '2':
            mNoise.set_type(NodeNoise::TYPE::SIMPLEX);
            break;
    }
}
