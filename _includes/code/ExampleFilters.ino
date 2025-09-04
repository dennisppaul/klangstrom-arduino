/*
 * this example demonstrates how to apply filters to a signal. it also demonstrates how to dynamically disconnect and
 * reconnect filters.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeVCF*         mFilter;

void setup() {
    connect_filter(new NodeVCFMoogLHBP);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
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
            mFilter->set_cutoff_Hz(mouse_event(data).x * 22050.0);
            mFilter->set_resonance(mouse_event(data).y * 1.0);
            mOsc.set_frequency(DEFAULT_FREQUENCY + DEFAULT_FREQUENCY * floor(mouse_event(data).x * 8));
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
    Klang::disconnect(mOsc, Node::CH_OUT_SIGNAL, *mFilter, Node::CH_IN_SIGNAL);
    Klang::disconnect(*mFilter, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    delete (mFilter);
    mFilter = pFilter;
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, *mFilter, Node::CH_IN_SIGNAL);
    Klang::connect(*mFilter, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);
    Klang::unlock();
}
