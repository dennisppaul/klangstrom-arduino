//
//  ExampleEvents
//

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC          mDAC;
NodeVCOWavetable mOsc;
NodeADSR         mADSR;

void setup() {
    Klang::connect(mOsc, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mOsc.set_frequency(DEFAULT_FREQUENCY * 2);
    mOsc.set_amplitude(0.5);
    mOsc.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);

    mADSR.set_attack(0.01);
    mADSR.set_decay(0.05);
    mADSR.set_sustain(0.5);
    mADSR.set_release(0.25);
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_KEY_PRESSED:
            mADSR.start();
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_KEY_RELEASED:
            mADSR.stop();
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mOsc.set_frequency(DEFAULT_FREQUENCY * (2 + mouse_event(data).x));
            break;
        case EVENT_MIDI_IN_NOTE_ON:
            midi_note_on(data);
            break;
        case EVENT_MIDI_IN_NOTE_OFF:
            mADSR.stop();
            break;
        case EVENT_MIDI_IN_CC:
            KLANG_LOG("EVENT_MIDI_IN_CC: %f, %f, %f", data[CHANNEL], data[NUMBER], data[VALUE]);
            break;
        case EVENT_MIDI_IN_PROGRAM_CHANGE:
            KLANG_LOG("EVENT_MIDI_IN_CC: %f, %f", data[CHANNEL], data[PROG_NUM]);
            break;
        case EVENT_MIDI_IN_SYSEX:
            KLANG_LOG("EVENT_MIDI_IN_SYSEX");
            break;
    }
}

void midi_note_on(const void* data) {
    const float mFrequency = KlangMath::note_to_frequency(data[NOTE]);
    const float mAmplitude = data[VELOCITY] / 127.0;
    mOsc.set_frequency(mFrequency);
    mOsc.set_amplitude(mAmplitude);
    mADSR.start();
}
