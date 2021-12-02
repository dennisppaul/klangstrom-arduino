//
//  ExampleSampler
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC        mDAC;
NodeSamplerUI8 mSampler;  // requires only 25% of memory compared to the F32 sampler
NodeADSR       mADSR;

const uint16_t SAMPLER_BUFFER_SIZE = 512;
uint8_t        mSamplerBuffer[SAMPLER_BUFFER_SIZE];

void setup() {
    Klang::connect(mSampler, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mSampler.set_buffer(mSamplerBuffer);
    mSampler.set_buffer_size(SAMPLER_BUFFER_SIZE);
    mSampler.loop(true);

    for (uint16_t i = 0; i < SAMPLER_BUFFER_SIZE; i++) {
        const float   r    = (float)i / SAMPLER_BUFFER_SIZE * TWO_PI * 4.0;
        const float   s    = (sin(r) * 0.5 + sin(r * TWO_PI) * 0.1) * 0.5;
        const uint8_t sUI8 = (uint8_t)((s * 0.5 + 0.5) * 255);
        mSamplerBuffer[i]  = sUI8;
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mSampler.set_speed(1.0 + data[X] * 3.0);
            break;
        case EVENT_KEY_PRESSED:
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            mADSR.start();
            break;
        case EVENT_KEY_RELEASED:
        case EVENT_MOUSE_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            mADSR.stop();
            break;
    }
}
