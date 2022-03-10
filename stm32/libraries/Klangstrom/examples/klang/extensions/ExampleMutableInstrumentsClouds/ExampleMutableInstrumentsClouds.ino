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

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mADC.process_frame(pInputLeft, pInputRight);
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void event_receive(const EVENT_TYPE event, const float* data) {
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