//
//  ExampleLFOs
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;
NodeVCOWavetable    mLFOAmplitude;
NodeVCOWavetable    mLFOFrequency;

void setup()  {
    Klang::connect(mLFOFrequency, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_FREQ);
    Klang::connect(mLFOAmplitude, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_AMP);
    Klang::connect(mOsc,          Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);

    mLFOFrequency.set_frequency(10);
    mLFOFrequency.set_amplitude(5);
    mLFOFrequency.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mLFOFrequency.set_offset(DEFAULT_FREQUENCY * 2);

    mLFOAmplitude.set_frequency(1);
    mLFOAmplitude.set_amplitude(0.1);
    mLFOAmplitude.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mLFOAmplitude.set_offset(0.5);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_KEY_PRESSED:
            handle_key_pressed(data[KEY]);
            break;
        case EVENT_MOUSE_MOVED:
            mLFOFrequency.set_offset(DEFAULT_FREQUENCY + DEFAULT_FREQUENCY * floor(data[X] * 8));
            mLFOAmplitude.set_amplitude(data[Y] * 0.5);
            break;
        case EVENT_MOUSE_DRAGGED:
            mLFOAmplitude.set_frequency(data[X] * 20);
            mLFOFrequency.set_frequency(data[Y] * 20);
            break;
    }
}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);
            break;
        case '2':
            mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
            break;
        case '3':
            mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
            break;
        case '4':
            mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
            break;
        case '5':
            mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::EXPONENT);
            break;
    }
}
