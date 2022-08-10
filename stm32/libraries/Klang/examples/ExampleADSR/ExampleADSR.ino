/*
 * this example demonstrates how to use an ADSR ( attack, decay, sustain + release ) envelope to control the volume of
 * a signal. the signal is produced by an oscillator (VCO) which is fed into the the envelope.
 *
 * press the encoder to start and release the encoder to stop the ADSR envelope.
 *
 * this example also demonstrates how to handle events received from encoder, computer mouse or keyboard ( the latter
 * two are only available in the emulator ).
 *
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeADSR        mADSR;
NodeDAC         mDAC;

void setup() {
    Klang::lock();

    Klang::connect(mVCO, mADSR);
    Klang::connect(mADSR, mDAC);

    mVCO.set_frequency(DEFAULT_FREQUENCY * 2);
    mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
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
        case EVENT_ENCODER_BUTTON_PRESSED:
        case EVENT_KEY_PRESSED:
        case EVENT_MOUSE_PRESSED:
            mADSR.start();
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
        case EVENT_KEY_RELEASED:
        case EVENT_MOUSE_RELEASED:
            mADSR.stop();
            break;
    }
}
