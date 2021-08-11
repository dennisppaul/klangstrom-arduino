//
// ExampleStromEvents
//

#include "StromNodes.hpp"

using namespace strom;

class MEventListener : public StromEventListener {
public:
    void event(STROM_EVENT pEventType, std::vector<float> pData) {
        STROM_LOG("    STROM_EVENT type       : 0x%02x", pEventType);
        for (uint8_t i = 0; i < pData.size(); i++) {
            STROM_LOG("                data[0x%02x] : %f", i, pData[i]);
        }
    }
};

StromNodeTrigger mTrigger;
StromNodeNote    mNote;
MEventListener   mEventListener;

void setup() {
    STROM_LOG("--- STRUCTURE ---");
    mNote.in(StromNodeNote::CH_IN_VELOCITY, 100);
    mNote.in(StromNodeNote::CH_IN_PITCH, 48);
    mNote.set_event_listener(&mEventListener);
    Strom::connect(mTrigger, StromNodeTrigger::CH_OUT_TRIGGER_00, mNote, StromNodeNote::CH_IN_TRIGGER);
}

void beat(uint32_t pBeat) {
    STROM_LOG("--- PROCESS   ---");
    Strom::update(mTrigger);
}
