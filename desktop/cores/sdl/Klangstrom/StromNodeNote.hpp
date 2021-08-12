//
//  StromNodeNote.hpp
//  Strom – a node+text-based library
//
//
//

/*
 *
 *       [ NODE_NOTE           ]
 *       +---------------------+
 *       |                     |
 * IN00--| TRIGGER             |
 * IN01--| PITCH               |
 * IN02--| VELOCITY            |
 *       |                     |
 *       +---------------------+
 *
 *       @description (
 *           receives a trigger and creates a note event with pitch and velocity values.
 *       )
 *       @todo ( how are notes turned off? )
 */

#ifndef StromNodeNote_hpp
#define StromNodeNote_hpp

#include "StromEventDistributor.hpp"
#include "StromNode.hpp"

#define DEBUG_STROM_NODE_NOTE

namespace strom {
    class StromNodeNote : public StromNode, public StromEventDistributor {
    public:
        static const STROM_CHANNEL_ID CH_IN_TRIGGER  = 0;
        static const STROM_CHANNEL_ID CH_IN_PITCH    = 1;
        static const STROM_CHANNEL_ID CH_IN_VELOCITY = 2;

        StromNodeNote() : StromNode(3, 0) {}

        void in(const STROM_CHANNEL_ID pChannel, const float pData) override {
            switch (pChannel) {
                case CH_IN_TRIGGER:
                    mTrigger = pData;
#ifdef DEBUG_STROM_NODE_NOTE
                    STROM_LOG("+++ @StromNodeNote  Node(%02d) receives trigger : %f", ID(), mTrigger);
                    STROM_LOG("+++ @StromNodeNote  playing note              : pitch(%f) velocity(%f)", mPitch, mVelocity);
#endif
                    fire_event(STROM_EVENT::NOTE_ON, std::vector<float>{mPitch, mVelocity});
                    break;
                case CH_IN_PITCH:
                    mPitch = pData;
#ifdef DEBUG_STROM_NODE_NOTE
                    STROM_LOG("+++ @StromNodeNote  Node(%02d) receives pitch   : %f", ID(), mPitch);
#endif
                    break;
                case CH_IN_VELOCITY:
                    mVelocity = pData;
#ifdef DEBUG_STROM_NODE_NOTE
                    STROM_LOG("+++ @StromNodeNote  Node(%02d) receives velocity: %f", ID(), mVelocity);
#endif
                    break;
            }
        }

        float out(const STROM_CHANNEL_ID pChannel) override {
            return STROM_NO_VALUE;
        }

    private:
        float mTrigger  = 0.0;
        float mPitch    = 0.0;
        float mVelocity = 0.0;
    };
}  // namespace strom

#endif /* StromNodeNote_hpp */
