/*
 * this example demonstrates how to apply a chorus effect to a signal. note, that the effect transforms a mono input
 * into a stereo signal.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOWavetable mOsc;
NodeChorus       mChorus;
NodeDAC          mDAC;

void setup() {
    Klang::connect(mOsc, mChorus);
    Klang::connect(mChorus, NodeChorus::CH_OUT_SIGNAL_LEFT, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mChorus, NodeChorus::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mChorus.set_stereo(true);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);

    mDAC.set_stereo(true);
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
            mChorus.set_feedback(mouse_event(data).x);
            mChorus.set_sweep(mouse_event(data).y);
            break;
        case EVENT_MOUSE_DRAGGED:
            mChorus.set_rate(mouse_event(data).x);
            mChorus.set_delay(mouse_event(data).y);
            break;
    }
}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            mChorus.set_mode(true);
            break;
        case '2':
            mChorus.set_mode(false);
            break;
        case '3':
            mChorus.set_feedback_sign(true);
            break;
        case '4':
            mChorus.set_feedback_sign(false);
            break;
    }
}
