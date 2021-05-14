//
//  ExampleWavetables.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;

void setup()  {
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.35);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    switch (event) {
        case EVENT_MOUSE_MOVED:
            mOsc.set_frequency(DEFAULT_FREQUENCY + DEFAULT_FREQUENCY * floor(data[X] * 8));
            mOsc.set_waveform((uint8_t)(6 * data[Y]));
            break;
        case EVENT_KEY_PRESSED:
            handle_key_pressed(data[KEY]);
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
