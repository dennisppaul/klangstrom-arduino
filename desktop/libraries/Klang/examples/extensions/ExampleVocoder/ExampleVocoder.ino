/*
 * this example demonstrates how to apply a vocoder effect to a signal. the vocoder superimposes a modulator signal
 * ( e.g a human voice ) onto a carrier signal ( e.g sawtooth oscillator ).
 *
 * note that `#define KLANG_EXT_NODE_VOCODER` is required to use the effect.
 */

#define KLANG_EXT_NODE_VOCODER

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeADC          mADC;
NodeVocoder      mVocoder{24, 4, 256};
NodeVCOWavetable mVocoderCarrierOsc;
NodeNoise        mVocoderCarrierOscNoise;
SIGNAL_TYPE      mSamplerBuffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];

void setup() {
    Klang::lock();

    Klang::connect(mADC, Node::CH_OUT_SIGNAL, mVocoder, NodeVocoder::CH_IN_MODULATOR);
    Klang::connect(mVocoderCarrierOsc, Node::CH_OUT_SIGNAL, mVocoder, NodeVocoder::CH_IN_CARRIER);
    Klang::connect(mVocoder, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mVocoderCarrierOsc.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
    mVocoderCarrierOsc.set_frequency(55);
    mVocoderCarrierOsc.set_amplitude(1.0);
    mVocoder.set_output_rescale(50);

    Klang::unlock();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mADC.process_frame(pInputLeft, pInputRight);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            handle_key_pressed(keyboard_event(data).key);
            break;
        case EVENT_MOUSE_MOVED:
            mouseMoved(mouse_event(data).x, mouse_event(data).y);
            break;
    }
}

void mouseMoved(float x, float y) {
    mVocoder.set_formant_shift(0.25 + 2.0 * x);
    mVocoder.set_reaction_time(0.002 + (0.1) * y);
}

void handle_key_pressed(int key) {
    if (key == '1') {
        mVocoderCarrierOsc.set_frequency(22.5);
    }
    if (key == '2') {
        mVocoderCarrierOsc.set_frequency(55.0);
    }
    if (key == '3') {
        mVocoderCarrierOsc.set_frequency(110.0);
    }
    if (key == '4') {
        mVocoderCarrierOsc.set_frequency(220.0);
    }
    if (key == '5') {
        mVocoderCarrierOsc.set_frequency(440.0);
    }
    if (key == '8') {
        Klang::disconnect(mVocoderCarrierOsc, Node::CH_OUT_SIGNAL, mVocoder, NodeVocoder::CH_IN_CARRIER);
        Klang::connect(mVocoderCarrierOscNoise, Node::CH_OUT_SIGNAL, mVocoder, NodeVocoder::CH_IN_CARRIER);
    }
    if (key == '9') {
        Klang::disconnect(mVocoderCarrierOscNoise, Node::CH_OUT_SIGNAL, mVocoder, NodeVocoder::CH_IN_CARRIER);
        Klang::connect(mVocoderCarrierOsc, Node::CH_OUT_SIGNAL, mVocoder, NodeVocoder::CH_IN_CARRIER);
    }
    if (key == '0') {
        mVocoder.reset();
    }
}
