/*
 * this example demonstrates how to use the sampler node. in this example a buffer with 32-bit floats (F32) is used,
 * however other types are implemented as well:
 *
 * - `NodeSamplerUI8`
 * - `NodeSamplerI8`
 * - `NodeSamplerUI16`
 * - `NodeSamplerI16`
 * - `NodeSamplerF32` ( alias to `NodeSampler` )
 *
 * the sampler can be set to loop and the speed of the playback can be specified; including negative speed values which
 * will cause the playback to be reversed. note, that a sampler must be started with `start()`.
 *
 * this example fills the sample buffer with a modified sine wave. press mouse, keyboard or potentiomter to play the
 * sample and move the mouse or potentiometer to change the speed ( which does not have an *impressive* effect in this
 * very example ).
 */

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC     mDAC;
NodeSampler mSampler;
NodeADSR    mADSR;

const uint16_t SAMPLER_BUFFER_SIZE = 512;
float          mSamplerBuffer[SAMPLER_BUFFER_SIZE];

void setup() {
    Klang::connect(mSampler, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mSampler.set_buffer(mSamplerBuffer);
    mSampler.set_buffer_size(SAMPLER_BUFFER_SIZE);
    mSampler.set_speed(1.0);
    mSampler.loop(true);
    mSampler.start();

    for (uint16_t i = 0; i < SAMPLER_BUFFER_SIZE; i++) {
        const float r     = (float)i / SAMPLER_BUFFER_SIZE * TWO_PI * 4.0;
        mSamplerBuffer[i] = (sin(r) * 0.5 + sin(r * TWO_PI) * 0.1) * 0.5;
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
            mSampler.set_speed(-2.0 + mouse_event(data).x * 4.0);
            break;
        case EVENT_ENCODER_ROTATED:
            mSampler.set_speed(mSampler.get_speed() + encoder_event(data).delta * 0.1);
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
