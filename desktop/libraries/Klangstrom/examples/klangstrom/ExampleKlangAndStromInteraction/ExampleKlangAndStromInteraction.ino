//
// ExampleKlangAndStromInteraction
//

#include "KlangNodes.hpp"
#include "StromNodes.hpp"

using namespace klangstrom;

klang::NodeVCOFunction  mVCO;
klang::NodeDAC          mDAC;
klang::NodeADSR         mADSR;
strom::StromNodeTrigger mTrigger;
strom::StromNodeNote    mNote;

class MEventListener : public strom::StromEventListener {
    bool mToggle = false;

public:
    void event(strom::STROM_EVENT pEventType, std::vector<float> pData) {
        if (mToggle) {
            mADSR.start();
        } else {
            mADSR.stop();
        }
        mToggle = !mToggle;
    }
};
MEventListener mEventListener;

void setup() {
    klang::Klang::lock();
    klang::Klang::connect(mVCO, klang::Node::CH_OUT_SIGNAL, mADSR, klang::Node::CH_IN_SIGNAL);
    klang::Klang::connect(mADSR, klang::Node::CH_OUT_SIGNAL, mDAC, klang::NodeDAC::CH_IN_SIGNAL_LEFT);
    mVCO.set_amplitude(0.25);
    mVCO.set_frequency(110);
    mVCO.set_waveform(klang::NodeVCOFunction::WAVEFORM::SINE);
    klang::Klang::unlock();

    mNote.in(strom::StromNodeNote::CH_IN_VELOCITY, 100);
    mNote.in(strom::StromNodeNote::CH_IN_PITCH, 48);
    mNote.set_event_listener(&mEventListener);
    strom::Strom::connect(mTrigger, strom::StromNodeTrigger::CH_OUT_TRIGGER_00,
                          mNote, strom::StromNodeNote::CH_IN_TRIGGER);
}

void beat(uint32_t pBeat) {
    strom::Strom::update(mTrigger);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
