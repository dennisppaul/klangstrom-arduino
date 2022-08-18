/*
 * this example demonstrates how to use the splitter node to store a signal and allow multiple nodes to connect to the
 * splitter’s output channel.
 *
 * in contrast to other node-based systems it is not allowed ( or at least not recommended ) in *Klang* to connect
 * multiple nodes to an output channel of an upstream node. if this functionality is needed, the splitter node is
 * a good choice. its only purpose is to store a signal from its intput and deliver it to multiple node connected
 * to its output.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeVCFMoogLHBP  mFilter;
NodeSplitter     mSplitter;

void setup() {
    Klang::connect(mOsc, mSplitter);
    Klang::connect(mSplitter, mFilter);
    Klang::connect(mFilter, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mSplitter, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.35);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);

    mDAC.set_stereo(true);
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    if (event == EVENT_MOUSE_MOVED) {
        mFilter.set_cutoff_Hz(mouse_event(data).x * 22050.0);
        mFilter.set_resonance(mouse_event(data).y * 1.0);
        mOsc.set_frequency(DEFAULT_FREQUENCY * 2 + DEFAULT_FREQUENCY * mouse_event(data).x);
    }
}
