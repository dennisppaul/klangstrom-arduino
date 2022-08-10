//
// ExampleStromEventsInKlang
//

/**
 * this example demonstrate how to use Strom events to interact with Klang nodes. 
 * 
 * in this example the Strom node `strom::StromNodeNote` emits an event to all its listeners. the 
 * two Klang nodes `MNodeADSR` and `MNodeVCOFunction` receive the event and use an envelope and an
 * oscillator to play an audible note. both classes inherit properties from existing Klang nodes 
 * and add `strom::StromEventListener` to enable them to listen to events.
 */

#include "Klangstrom.h"
#include "KlangMath.hpp"
#include "KlangNodes.hpp"
#include "StromNodes.hpp"

using namespace klangstrom;

class MNodeADSR : public strom::StromEventListener, public klang::NodeADSR {
public:
    void event(strom::STROM_EVENT pEventType, std::vector<float> pData) {
        if (mToggle) {
            start();
        } else {
            stop();
        }
        mToggle = !mToggle;
    }

private:
    bool mToggle = false;
};

class MNodeVCOFunction : public strom::StromEventListener, public klang::NodeVCOFunction {
public:
    void event(strom::STROM_EVENT pEventType, std::vector<float> pData) {
        set_frequency(klang::KlangMath::note_to_frequency(pData[0]));
        set_amplitude(pData[0] / 127.0);
    }
};

MNodeADSR               mADSR;
MNodeVCOFunction        mVCO;
klang::NodeDAC          mDAC;
strom::StromNodeTrigger mTrigger;
strom::StromNodeNote    mNote;

void setup() {
    klang::Klang::lock();
    klang::Klang::connect(mVCO, klang::Node::CH_OUT_SIGNAL, mADSR, klang::Node::CH_IN_SIGNAL);
    klang::Klang::connect(mADSR, klang::Node::CH_OUT_SIGNAL, mDAC, klang::NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_amplitude(0.25);
    mVCO.set_frequency(110);
    mVCO.set_waveform(klang::NodeVCOFunction::WAVEFORM::SINE);
    klang::Klang::unlock();

    mNote.in(strom::StromNodeNote::CH_IN_VELOCITY, 40);
    mNote.in(strom::StromNodeNote::CH_IN_PITCH, 48 * 2);
    mNote.add_event_listener(&mVCO);
    mNote.add_event_listener(&mADSR);
    strom::Strom::connect(mTrigger, strom::StromNodeTrigger::CH_OUT_TRIGGER_00,
                          mNote, strom::StromNodeNote::CH_IN_TRIGGER);
}

void beat(uint32_t pBeat) {
    strom::Strom::update(mTrigger);
    if (pBeat == 8) {
        mNote.clear_listeners();
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
