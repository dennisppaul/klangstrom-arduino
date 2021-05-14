//
//  ExampleEnvelope.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Nodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC             mDAC;
NodeEnvelope        mEnvelopeFreq;
NodeEnvelope        mEnvelopeAmp;
NodeEnvelope        mEnvelopeCutoff;
NodeVCOWavetable    mOsc;
NodeVCFMoogLHBP     mFilter;
NodeVCA             mMultiply;

void setup()  {
    Klang::connect(mEnvelopeFreq,     Node::CH_OUT_SIGNAL,    mMultiply,  Node::CH_IN_SIGNAL);
    Klang::connect(mMultiply,         Node::CH_OUT_SIGNAL,    mOsc,       NodeVCO::CH_IN_FREQ);
    Klang::connect(mEnvelopeAmp,      Node::CH_OUT_SIGNAL,    mOsc,       NodeVCO::CH_IN_AMP);
    Klang::connect(mEnvelopeCutoff,   Node::CH_OUT_SIGNAL,    mFilter,    NodeVCF::CH_IN_CUTOFF);
    Klang::connect(mOsc,                                      mFilter);
    Klang::connect(mFilter,           Node::CH_OUT_SIGNAL,    mDAC,       NodeDAC::CH_IN_SIGNAL_LEFT);

    mEnvelopeFreq.add_stage(4.0, 0.1);
    mEnvelopeFreq.add_stage(0.5, 0.2);
    mEnvelopeFreq.add_stage(1.0, 0.01);
    mEnvelopeFreq.add_stage(2.0, 0.1);
    mEnvelopeFreq.add_stage(1.0);

    mEnvelopeCutoff.add_stage(0.5, 0.05);
    mEnvelopeCutoff.add_stage(0.1, 0.15);
    mEnvelopeCutoff.add_stage(0.2, 0.1);
    mEnvelopeCutoff.add_stage(0.7, 0.1);
    mEnvelopeCutoff.add_stage(0.0);

    mEnvelopeAmp.stages().push_back(EnvelopeStage(0.0, 0.1));
    mEnvelopeAmp.stages().push_back(EnvelopeStage(0.5, 0.3));
    mEnvelopeAmp.stages().push_back(EnvelopeStage(0.5, 0.1));
    mEnvelopeAmp.stages().push_back(EnvelopeStage(0.0));

    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
    mOsc.set_frequency(55);
    mOsc.set_amplitude(0.33);

    mFilter.set_cutoff(0.1); // 0.1 – 0.3
    mFilter.set_resonance(0.5);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight)  {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data)  {
    if (event == EVENT_KEY_PRESSED) {
        char key = (char)data[KEY];
        switch (key) {
            case '1':
                mMultiply.set_amplification(55);
                mEnvelopeFreq.start();
                mEnvelopeAmp.start();
                mEnvelopeCutoff.start();
                break;
            case '2':
                mMultiply.set_amplification(110);
                mEnvelopeFreq.start();
                mEnvelopeAmp.start();
                mEnvelopeCutoff.start();
                break;
            case '3':
                mMultiply.set_amplification(220);
                mEnvelopeFreq.start();
                mEnvelopeAmp.start();
                mEnvelopeCutoff.start();
                break;
            case '4':
                mMultiply.set_amplification(440);
                mEnvelopeFreq.start();
                mEnvelopeAmp.start();
                mEnvelopeCutoff.start();
                break;
            case ' ':
                mEnvelopeFreq.stop();
                mEnvelopeAmp.stop();
                mEnvelopeCutoff.stop();
                break;
        }
    }
    if (event == EVENT_MOUSE_MOVED) {
        mEnvelopeFreq.set_time_scale(data[X] * 0.9 + 0.1);
        mEnvelopeAmp.set_time_scale(mEnvelopeFreq.get_time_scale());
        mEnvelopeCutoff.set_time_scale(mEnvelopeFreq.get_time_scale());
    }
}
