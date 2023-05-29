#include "Klangstrom.h"
#include "KlangNodes.hpp"
#include "NodeMutableInstrumentsClouds.hpp"

using namespace klang;
using namespace klangstrom;

NodeDAC                      mDAC;
NodeADC                      mADC;
NodeMutableInstrumentsClouds mClouds;

void setup() {
    Klang::lock();

    Klang::connect(mADC,    NodeADC::CH_OUT_SIGNAL_LEFT,                       mClouds, NodeMutableInstrumentsClouds::CH_IN_SIGNAL_LEFT);
    Klang::connect(mADC,    NodeADC::CH_OUT_SIGNAL_RIGHT,                      mClouds, NodeMutableInstrumentsClouds::CH_IN_SIGNAL_RIGHT);
    Klang::connect(mClouds, NodeMutableInstrumentsClouds::CH_OUT_SIGNAL_LEFT,  mDAC,    NodeDAC::CH_IN_SIGNAL_LEFT);
    Klang::connect(mClouds, NodeMutableInstrumentsClouds::CH_OUT_SIGNAL_RIGHT, mDAC,    NodeDAC::CH_IN_SIGNAL_RIGHT);

    Klang::unlock();
}

void loop() {}

void audioblock(float** input_signal, float** output_signal) {
    mADC.process_frame(input_signal[LEFT], input_signal[RIGHT]);
    mDAC.process_frame(output_signal[LEFT], output_signal[RIGHT]);
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_MOUSE_PRESSED:
        case EVENT_KEY_PRESSED:
            mClouds.get_parameters()->freeze       = true;
            break;
        case EVENT_MOUSE_RELEASED:
        case EVENT_KEY_RELEASED:
            mClouds.get_parameters()->freeze       = false;
            break;
        case EVENT_MOUSE_MOVED:
            mClouds.get_parameters()->reverb = mouse_event(data).x;
            mClouds.get_parameters()->dry_wet = mouse_event(data).y;
            break;
        case EVENT_MOUSE_DRAGGED:
            break;
    }
}