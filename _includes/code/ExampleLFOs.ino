/*
 * this example demonstrates how to control amplitude and frequency of an oscillator with two other oscillators
 * configured to oscillate at lower frequencies ( LFO ).
 *
 * move the mouse to change the frequency of the LFOs. use keys 1–6 to change the LFOs’ waveforms ( depending on amplitude and frequency the differences can be quite subtle ).
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeVCOWavetable mLFOAmplitude;
NodeVCOWavetable mLFOFrequency;

void setup() {
    Klang::connect(mLFOFrequency, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_FREQ);
    Klang::connect(mLFOAmplitude, Node::CH_OUT_SIGNAL, mOsc, NodeVCOWavetable::CH_IN_AMP);
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL);

    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);

    mLFOFrequency.set_frequency(10);
    mLFOFrequency.set_amplitude(5);
    mLFOFrequency.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mLFOFrequency.set_offset(DEFAULT_FREQUENCY * 2);

    mLFOAmplitude.set_frequency(1);
    mLFOAmplitude.set_amplitude(0.1);
    mLFOAmplitude.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
    mLFOAmplitude.set_offset(0.5);
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
            mLFOFrequency.set_offset(DEFAULT_FREQUENCY + DEFAULT_FREQUENCY * floor(mouse_event(data).x * 8));
            mLFOAmplitude.set_amplitude(mouse_event(data).y * 0.5);
            break;
        case EVENT_MOUSE_DRAGGED:
            mLFOAmplitude.set_frequency(mouse_event(data).x * 20);
            mLFOFrequency.set_frequency(mouse_event(data).y * 20);
            break;
    }
}

void handle_key_pressed(int key) {
    switch (key) {
        case '1':
            mLFOFrequency.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
            break;
        case '2':
            mLFOFrequency.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
            break;
        case '3':
            mLFOFrequency.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
            break;
        case '4':
            mLFOAmplitude.set_waveform(NodeVCOWavetable::WAVEFORM::SINE);
            break;
        case '5':
            mLFOAmplitude.set_waveform(NodeVCOWavetable::WAVEFORM::SAWTOOTH);
            break;
        case '6':
            mLFOAmplitude.set_waveform(NodeVCOWavetable::WAVEFORM::SQUARE);
            break;
    }
}
