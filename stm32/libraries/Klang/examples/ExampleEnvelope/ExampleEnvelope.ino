/*
 * this example how to control the oscillator parameters frequency, cutoff, and amplitude with an envelope over time.
 * an envelope can have multiple stages. a start value is defined via `set_start_value(value)` ( default = 0.0 ) and
 * stages are add with `add_stage(duration, value)` or `add_stage_ms(duration_in_milli_seconds, value)`.
 *
 * once an envelope is started with `start()` it will interpolate from its current value to the value of the next
 * stage. e.g the following code will generate values from `0.0` to `2.0` over 1.0 sec then from `2.0` to `1.0` over
 * 0.5 sec:
 *
 * ```
 * mEnvelope.set_start_value(0.0);
 * mEnvelope.add_stage(1.0, 2.0);
 * mEnvelope.add_stage(0.5, 1.0);
 * ```
 *
 * envelopes can be stopped at any point with `stop()`. envelopes can be set to loop with `set_loop(true)`. the
 * envelope duration can be scaled with `set_time_scale(scale)` where a value of `1.0` is the normal duration. the 
 * envelopes values can be scaled with `set_value_scale(scale)` where a value of `1.0` is the normal value.
 *
 * envelopes can also be triggered from an incoming signal where a `start()` event is triggered whenever it passes a
 * threshold ( 0.0 ) on a rising edge and a `stop()` event on a falling edge.
 *
 * in this example pressing `1` on the keyboard will trigger the envelope, `2` will set the envelope to loop and `3`
 * will stop the envelope. the mouse will change the oscillator frequency on the y-axis and the envelope’s time scaling
 * on the x-axis.
 *
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeEnvelope     mEnvelopeFreq;
NodeEnvelope     mEnvelopeAmp;
NodeEnvelope     mEnvelopeCutoff;
NodeVCOWavetable mOsc;
NodeVCFMoogLHBP  mFilter;
NodeVCA          mMultiply;
bool             fLoopEnvelopes = false;

void setup() {
    Klang::connect(mEnvelopeFreq, Node::CH_OUT_SIGNAL, mMultiply, Node::CH_IN_SIGNAL);
    Klang::connect(mMultiply, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_FREQ);
    Klang::connect(mEnvelopeAmp, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_AMP);
    Klang::connect(mEnvelopeCutoff, Node::CH_OUT_SIGNAL, mFilter, NodeVCF::CH_IN_CUTOFF);
    Klang::connect(mOsc, mFilter);
    Klang::connect(mFilter, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mEnvelopeFreq.add_stage(0.1, 4.0);
    mEnvelopeFreq.add_stage(0.2, 0.5);
    mEnvelopeFreq.add_stage(0.01, 1.0);
    mEnvelopeFreq.add_stage(0.1, 2.0);
    mEnvelopeFreq.add_stage(0.5, 0.0);

    mEnvelopeCutoff.set_start_value(0.4);
    mEnvelopeCutoff.add_stage(0.05, 0.5);
    mEnvelopeCutoff.add_stage(0.15, 0.1);
    mEnvelopeCutoff.add_stage(0.1, 0.2);
    mEnvelopeCutoff.add_stage(0.1, 0.7);
    mEnvelopeCutoff.add_stage(0.4, 0.0);

    mEnvelopeAmp.add_stage_ms(100, 0.5);
    mEnvelopeAmp.add_stage_ms(300, 0.3);
    mEnvelopeAmp.add_stage_ms(450, 0.0);

    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
    mOsc.set_frequency(55);
    mOsc.set_amplitude(0.33);

    mFilter.set_cutoff(0.1);
    mFilter.set_resonance(0.5);
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void start_envelopes() {
    mEnvelopeFreq.start();
    mEnvelopeAmp.start();
    mEnvelopeCutoff.start();
}

void event_receive(const EVENT_TYPE event, const float* data) {
    if (event == EVENT_KEY_PRESSED) {
        const char key = (char)keyboard_event(data).key;
        switch (key) {
            case '1':
                start_envelopes();
                break;
            case '2':
                fLoopEnvelopes = !fLoopEnvelopes;
                mEnvelopeFreq.set_loop(fLoopEnvelopes);
                mEnvelopeCutoff.set_loop(fLoopEnvelopes);
                mEnvelopeAmp.set_loop(fLoopEnvelopes);
                break;
            case '3':
                mEnvelopeAmp.stop();
                mEnvelopeFreq.stop();
                mEnvelopeCutoff.stop();
                break;
        }
    }
    if (event == EVENT_MOUSE_MOVED) {
        mMultiply.set_amplification(55 * floor(1.0 + mouse_event(data).y * 8.0));
        float mTimeScale = mouse_event(data).x * 0.9 + 2.1;
        mEnvelopeFreq.set_time_scale(mTimeScale);
        mEnvelopeAmp.set_time_scale(mTimeScale);
        mEnvelopeCutoff.set_time_scale(mTimeScale);
    }
}
