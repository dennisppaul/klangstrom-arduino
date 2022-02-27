/*
 * this example demonstrates how to generate signals from oscillators. note that there are two different types of
 * oscillators:
 *
 * - `NodeVCOFunction` :: computes a signal from a function on the fly. it is more precise especially at very low
 *   frequencies, and uses less memory but requires more compuational resources.
 * - `NodeVCOWavetable` :: computes a signal from a wavetable. it requires more memory and might create artifacts
 *    especially at low frequencies. it however is much faster. note that the wavetable is not generated at
 *    instantiation time, and must always be popluated before use. the wavetable can also be populated with custom
 *    waveforms.
 *
 * press keys 1–4 to switch the waveform of the function oscillators. press keys 5–8 to switch the waveform of the
 * wavetable oscillators. use mouse to change the frequency and the amplitude of the osciallators.
 */
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction  mVCOFunction;
NodeVCOWavetable mVCOWavetable;
NodeDAC          mDAC;

void setup() {
    Klang::connect(mVCOFunction, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mVCOWavetable, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mVCOFunction.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCOFunction.set_amplitude(0.5);
    mVCOWavetable.set_frequency(mVCOFunction.get_frequency());
    mVCOWavetable.set_amplitude(mVCOFunction.get_amplitude());
    mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mDAC.set_stereo(true);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void handle_key_pressed(char key) {
    switch (key) {
        case '1':
            mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
            break;
        case '2':
            mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);
            break;
        case '3':
            mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);
            break;
        case '4':
            mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::SQUARE);
            break;
        case '5':
            mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
            break;
        case '6':
            mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);
            break;
        case '7':
            mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
            break;
        case '8':
            mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
            break;
        case '9':
            mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::EXPONENT);
            break;
    }
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            handle_key_pressed(keyboard_event(data).key);
            break;
        case EVENT_MOUSE_MOVED:
            mVCOFunction.set_frequency(DEFAULT_FREQUENCY / 2 * floor(32 * mouse_event(data).x));
            mVCOFunction.set_amplitude(0.5 * mouse_event(data).y);
            mVCOWavetable.set_frequency(mVCOFunction.get_frequency());
            mVCOWavetable.set_amplitude(mVCOFunction.get_amplitude());
            break;
    }
}
