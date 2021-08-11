//
//  ExampleWaveshaper
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

class NodeWaveshaper : public NodeKernel {
    public:
        void set_amount(float pAmount) {
            mAmount = pAmount;
        }

        SIGNAL_TYPE kernel(SIGNAL_TYPE s) {
            /*
                from http://www.musicdsp.org/showArchiveComment.php?ArchiveID=46

                amount should be in [-1..1]

                x = input in [-1..1]
                y = output
                k = 2*amount/(1-amount);

                f(x) = (1+k)*x/(1+k*abs(x))
            */

            const float k = 2 * mAmount / (1 - mAmount);
            return (1 + k) * s / ( 1 + k * abs(s) );
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
    Klang::connect(mVCO01,      Node::CH_OUT_SIGNAL,    mMixer, NodeMixer2::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mVCO02,      Node::CH_OUT_SIGNAL,    mMixer, NodeMixer2::CH_IN_SIGNAL_LEFT);
    Klang::connect(mMixer,      mWaveshaper);
    Klang::connect(mWaveshaper, mDAC);

    set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO01.set_amplitude(0.85);
    mVCO02.set_amplitude(0.85);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            if (data[KEY] == '1') {
                set_waveform(NodeVCOFunction::WAVEFORM::SINE);
            }
            if (data[KEY] == '2') {
                set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);
            }
            if (data[KEY] == '3') {
                set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);
            }
            if (data[KEY] == '4') {
                set_waveform(NodeVCOFunction::WAVEFORM::SQUARE);
            }
            break;
        case EVENT_MOUSE_MOVED:
            set_frequency(DEFAULT_FREQUENCY * floor(15 * data[X] + 1));
            mWaveshaper.set_amount(2.0 * data[Y] - 1);
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
