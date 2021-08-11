//
//  ExampleOscillators
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction     mVCOFunction;
NodeVCOWavetable    mVCOWavetable;
NodeDAC             mDAC;

void setup() {
    Klang::connect(mVCOFunction,    Node::CH_OUT_SIGNAL,    mDAC,   NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mVCOWavetable,   Node::CH_OUT_SIGNAL,    mDAC,   NodeDAC::CH_IN_SIGNAL_RIGHT);

    mVCOFunction.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCOFunction.set_amplitude(0.5);
    mVCOWavetable.set_frequency(mVCOFunction.get_frequency());
    mVCOWavetable.set_amplitude(mVCOFunction.get_amplitude());

    mDAC.set_stereo(true);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            if (data[KEY] == '1') {
                mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
            }
            if (data[KEY] == '2') {
                mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);
            }
            if (data[KEY] == '3') {
                mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);
            }
            if (data[KEY] == '4') {
                mVCOFunction.set_waveform(NodeVCOFunction::WAVEFORM::SQUARE);
            }
            if (data[KEY] == '5') {
                mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
            }
            if (data[KEY] == '6') {
                mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);
            }
            if (data[KEY] == '7') {
                mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
            }
            if (data[KEY] == '8') {
                mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
            }
            if (data[KEY] == '9') {
                mVCOWavetable.set_waveform(NodeVCOWavetable::WAVEFORM::EXPONENT);
            }
            break;
        case EVENT_MOUSE_MOVED:
            mVCOFunction.set_frequency(DEFAULT_FREQUENCY / 2 * floor(32 * data[X]));
            mVCOFunction.set_amplitude(0.5 * data[Y]);
            mVCOWavetable.set_frequency(mVCOFunction.get_frequency());
            mVCOWavetable.set_amplitude(mVCOFunction.get_amplitude());
            break;
    }
}
