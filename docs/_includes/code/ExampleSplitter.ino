//
//  ExampleSplitter
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeVCOWavetable    mOsc;
NodeVCFMoogLHBP     mFilter;
NodeSplitter        mSplitter;

void setup()  {
    Klang::connect(mOsc,                              mSplitter);
    Klang::connect(mSplitter,                         mFilter);
    Klang::connect(mFilter,   Node::CH_OUT_SIGNAL,    mDAC,       NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mSplitter, Node::CH_OUT_SIGNAL,    mDAC,       NodeDAC::CH_IN_SIGNAL_RIGHT);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.35);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);

    mDAC.set_stereo(true);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    if (event == EVENT_MOUSE_MOVED) {
        mFilter.set_cutoff_Hz(data[X] * 22050.0);
        mFilter.set_resonance(data[Y] * 1.0);
        mOsc.set_frequency(DEFAULT_FREQUENCY * 2 + DEFAULT_FREQUENCY * data[X]);
    }
}
