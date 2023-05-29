/*
 * this example demonstrates how to write a custom waveshaper node from a `NodeKernel` base class.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class NodeWaveshaper : public NodeKernel {
public:
    void set_amount(float pAmount) {
        mAmount = pAmount;
    }

    float kernel(float s) {
        /*
            from http://www.musicdsp.org/showArchiveComment.php?ArchiveID=46

            amount should be in [-1..1]

            x = input in [-1..1]
            y = output
            k = 2*amount/(1-amount);

            f(x) = (1+k)*x/(1+k*abs(x))
        */

        const float k = 2 * mAmount / (1 - mAmount);
        return (1 + k) * s / (1 + k * abs(s));
    }

private:
    float mAmount = 0.5;
};

NodeVCOFunction mVCO01;
NodeVCOFunction mVCO02;
NodeMixer2      mMixer;
NodeWaveshaper  mWaveshaper;
NodeDAC         mDAC;

void setup() {
    Klang::connect(mVCO01, Node::CH_OUT_SIGNAL, mMixer, NodeMixer2::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mVCO02, Node::CH_OUT_SIGNAL, mMixer, NodeMixer2::CH_IN_SIGNAL_LEFT);
    Klang::connect(mMixer, mWaveshaper);
    Klang::connect(mWaveshaper, mDAC);

    set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO01.set_amplitude(0.85);
    mVCO02.set_amplitude(0.85);
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            if (keyboard_event(data).keys[0] == '1') {
                set_waveform(NodeVCOFunction::WAVEFORM::SINE);
            }
            if (keyboard_event(data).keys[0] == '2') {
                set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);
            }
            if (keyboard_event(data).keys[0] == '3') {
                set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);
            }
            if (keyboard_event(data).keys[0] == '4') {
                set_waveform(NodeVCOFunction::WAVEFORM::SQUARE);
            }
            break;
        case EVENT_MOUSE_MOVED:
            set_frequency(DEFAULT_FREQUENCY * floor(15 * mouse_event(data).x + 1));
            mWaveshaper.set_amount(2.0 * mouse_event(data).y - 1);
            break;
    }
}

void set_waveform(NodeVCOFunction::WAVEFORM pWaveform) {
    mVCO01.set_waveform(pWaveform);
    mVCO02.set_waveform(pWaveform);
}

void set_frequency(float pFreq) {
    mVCO01.set_frequency(pFreq);
    mVCO02.set_frequency(mVCO01.get_frequency() * 1.5);
}
