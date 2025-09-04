/*
 * this example demonstrate how to apply different kinds of distortion to a signal.
 * keys 1–3 select the distortion type. key 4–6 select the signal type. the mouse position controls the distortion parameters clipping and amplification.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOWavetable mOsc;
NodeDistortion   mDistortion;
NodeDAC          mDAC;

void setup() {
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mDistortion, Node::CH_IN_SIGNAL);
    Klang::connect(mDistortion, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.25);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            handle_key_pressed(keyboard_event(data).keys[0]);
            break;
        case EVENT_MOUSE_MOVED:
            mDistortion.set_clip(mouse_event(data).x * 0.25);
            mDistortion.set_amplification(0.5 + mouse_event(data).y * 9.0);
            break;
    }
}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            mDistortion.set_type(NodeDistortion::TYPE::CLIP);
            break;
        case '2':
            mDistortion.set_type(NodeDistortion::TYPE::FOLDBACK);
            break;
        case '3':
            mDistortion.set_type(NodeDistortion::TYPE::FOLDBACK_SINGLE);
            break;
        case '4':
            mOsc.set_frequency(DEFAULT_FREQUENCY * 1);
            break;
        case '5':
            mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
            break;
        case '6':
            mOsc.set_frequency(DEFAULT_FREQUENCY * 4);
            break;
    }
}
