/*
 * this example demonstrates how to mix 8 ( theoretically up to 256 ) signals into a two output signals ( i.e stereo ).
 * it also shows how to use the pan parameter on each incoming signal.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

static const uint8_t NUM_OF_OSC = 8;

struct Traveller {
    float            frequency;
    float            pan;
    float            speed;
    float            direction;
    NodeVCOWavetable oscillator;
};

NodeDAC              mDAC;
Traveller            fTraveller[NUM_OF_OSC];
NodeMixerMultiStereo mMixer;

void setup() {
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        Klang::connect(fTraveller[i].oscillator, Node::CH_OUT_SIGNAL, mMixer, i);
    }
    Klang::connect(mMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_LEFT, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mMixer, NodeMixerMultiStereo::CH_OUT_SIGNAL_RIGHT, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);

    mDAC.set_stereo(true);

    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        fTraveller[i].frequency = DEFAULT_FREQUENCY * 0.25 * (1.0 + 7.9 * i / NUM_OF_OSC);
        fTraveller[i].pan       = (float)i / (NUM_OF_OSC - 1) * 2 - 1;
        fTraveller[i].speed     = 0.01 * i / NUM_OF_OSC + 0.03;
        fTraveller[i].direction = (i - (i / 2) * 2) ? 1 : -1;
        fTraveller[i].oscillator.set_frequency(fTraveller[i].frequency);
        fTraveller[i].oscillator.set_waveform(NodeVCOWavetable::WAVEFORM::TRIANGLE);
        fTraveller[i].oscillator.set_amplitude(0.5 / NUM_OF_OSC);
        mMixer.set_pan(i, fTraveller[i].pan);
    }

    beats_per_minute(480);
}

void loop() {}

void beat(uint32_t pBeat) {
    for (uint8_t i = 0; i < NUM_OF_OSC; ++i) {
        fTraveller[i].pan += fTraveller[i].speed * fTraveller[i].direction;
        if (fTraveller[i].pan > 0.9) {
            fTraveller[i].direction = -1;
        } else if (fTraveller[i].pan < -0.9) {
            fTraveller[i].direction = 1;
        }
        mMixer.set_pan(i, fTraveller[i].pan);
    }
}

void audioblock(float* output_signal[LEFT], float* output_signal[RIGHT],
                float* input_signal[LEFT], float* input_signal[RIGHT]) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}
