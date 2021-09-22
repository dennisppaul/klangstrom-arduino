//
//  ExampleADSR
//

#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeADSR        mADSR;
NodeDAC         mDAC;

void setup() {
    Klang::lock();

    Klang::connect(mVCO, Node::CH_OUT_SIGNAL, mADSR, Node::CH_IN_SIGNAL);
    Klang::connect(mADSR, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);

    mVCO.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO.set_amplitude(0.5);

    mADSR.set_attack(0.01);
    mADSR.set_decay(0.05);
    mADSR.set_sustain(0.5);
    mADSR.set_release(0.25);

    Klang::unlock();
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
    switch (event) {
        case EVENT_KEY_PRESSED:
            if (data[KEY] == '1') {
                mVCO.set_waveform(NodeVCOFunction::WAVEFORM::TRIANGLE);
            }
            if (data[KEY] == '2') {
                mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
            }
            if (data[KEY] == '3') {
                mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SAWTOOTH);
            }
            if (data[KEY] == '4') {
                mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SQUARE);
            }
            break;
        case EVENT_MOUSE_PRESSED:
        case EVENT_ENCODER_BUTTON_PRESSED:
            mADSR.start();
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_ENCODER_BUTTON_RELEASED:
            mADSR.stop();
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            mVCO.set_frequency(DEFAULT_FREQUENCY * (2 + data[X]));
            mVCO.set_amplitude(0.5 * data[Y]);
            break;
        case EVENT_ENCODER_ROTATE:
            const float mDelta = data[TICK] - data[PREVIOUS_TICK];
            if (data[INDEX] == ENCODER_00) {
                mVCO.set_frequency(mVCO.get_frequency() + mDelta);
            } else if (data[INDEX] == ENCODER_01) {
                const float mAmp = fmin(1.0, fmax(0.0, mVCO.get_amplitude() + mDelta * 0.05));
                mVCO.set_amplitude(mAmp);
            }
            break;
    }
}
