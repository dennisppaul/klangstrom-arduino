//
// ExampleStromNodeStructure
//

#include "KlangNodes.hpp"
#include "StromNodes.hpp"

strom::StromNodeTrigger mTrigger;
strom::StromNodeNote    mNote;

void setup() {
    STROM_LOG("--- STRUCTURE ---");
    mNote.in(strom::StromNodeNote::CH_IN_VELOCITY, 100);
    mNote.in(strom::StromNodeNote::CH_IN_PITCH, 48);
    strom::Strom::connect(mTrigger, strom::StromNodeTrigger::CH_OUT_TRIGGER_00,
                          mNote, strom::StromNodeNote::CH_IN_TRIGGER);
}

void beat(uint32_t pBeat) {
    STROM_LOG("--- PROCESS   ---");
    strom::Strom::update(mTrigger);
}
