#include "Klangstrom.h"
#include "KlangNodes.hpp"
#include "NodeMutableInstrumentsBraids.hpp"

using namespace klang;
using namespace klangstrom;

NodeMutableInstrumentsBraids mBraids;
NodeADSR                     mADSR;
NodeDAC                      mDAC;

void setup() {
    Klang::lock();

    Klang::connect(mBraids, mADSR);
    Klang::connect(mADSR, mDAC);

    mADSR.set_attack(0.01);
    mADSR.set_decay(0.05);
    mADSR.set_sustain(0.5);
    mADSR.set_release(0.25);

    Klang::unlock();
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
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
