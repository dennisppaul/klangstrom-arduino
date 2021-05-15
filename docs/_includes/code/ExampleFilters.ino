//
//  ExampleFilters.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;
NodeVCF*            mFilter;

void setup()  {
    connect_filter(new NodeVCFMoogLHBP);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
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
            mFilter->set_cutoff_Hz(data[X] * 22050.0);
            mFilter->set_resonance(data[Y] * 1.0);
            mOsc.set_frequency(DEFAULT_FREQUENCY + DEFAULT_FREQUENCY * floor(data[X] * 8));
            break;
    }
}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            connect_filter(new NodeVCFMoogLHBP());
            break;
        case '2':
            connect_filter(new NodeVCFMoogLP());
            break;
        case '3':
            connect_filter(new NodeVCFLowPass());
            break;
    }
}

void connect_filter(NodeVCF* pFilter) {
    Klang::lock();
    Klang::disconnect(mOsc,     Node::CH_OUT_SIGNAL, *mFilter,  Node::CH_IN_SIGNAL);
    Klang::disconnect(*mFilter, Node::CH_OUT_SIGNAL, mDAC,      NodeDAC::CH_IN_SIGNAL_LEFT);

    delete (mFilter);
    mFilter = pFilter;
    Klang::connect(mOsc,     Node::CH_OUT_SIGNAL, *mFilter,  Node::CH_IN_SIGNAL);
    Klang::connect(*mFilter, Node::CH_OUT_SIGNAL, mDAC,      NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::unlock();
}
