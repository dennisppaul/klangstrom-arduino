/*
 * this examples demonstrates how to generate noise. there are two different kinds of noise. `RANDOM` is generated from
 * a pseudo random number generator and sounds a lot like white noise. `SIMPLEX` is generated from a perlin noise
 * generator. note that the latter has a stride parameter. also note that the algorithm is not perfectly suitable for
 * direct audio signals as it features an audible repetitive pattern.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC   mDAC;
NodeNoise mNoise;
NodeADSR  mADSR;

void setup() {
    Klang::connect(mNoise, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void loop() {}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            mNoise.set_type(NodeNoise::TYPE::RANDOM);
            break;
        case '2':
            mNoise.set_type(NodeNoise::TYPE::SIMPLEX);
            break;
    }
    mADSR.start();
}

void handle_encoder_pressed(int index) {
    switch (index) {
        case ENCODER_00:
            mNoise.set_type(NodeNoise::TYPE::RANDOM);
            break;
        case ENCODER_01:
            mNoise.set_type(NodeNoise::TYPE::SIMPLEX);
            break;
    }
    mADSR.start();
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            handle_encoder_pressed(encoder_event(data).index);
            break;
        case EVENT_KEY_PRESSED:
            handle_key_pressed(keyboard_event(data).key);
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
        case EVENT_KEY_RELEASED:
            mADSR.stop();
            break;
        case EVENT_MOUSE_MOVED:
            mNoise.set_stride(mouse_event(data).x * 0.5);
            break;
        case EVENT_ENCODER_ROTATED:
            mNoise.set_stride(mNoise.get_stride() + encoder_event(data).delta * 0.01);
            break;
    }
}
