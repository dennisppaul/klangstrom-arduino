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
 * IN02--| NOTE                |
 * IN03--| VELOCITY            |
 *       |                     |
 *       +---------------------+
 *
 */

#ifndef StromNodeNote_hpp
#define StromNodeNote_hpp

#include "StromNode.hpp"

namespace strom {
    class StromNodeNoteCallback {
    public:
        virtual void trigger_note(float pPitch, float pVelocity) = 0;
    };

    class StromNodeNote : public StromNode {
    public:
        static const STROM_CHANNEL_ID CH_IN_TRIGGER = 0;
        static const STROM_CHANNEL_ID CH_IN_PITCH = 1;
        static const STROM_CHANNEL_ID CH_IN_VELOCITY = 2;

        StromNodeNote() : StromNode(3, 0) {}

        void set_callback(StromNodeNoteCallback* pCallback) {
            mCallback = pCallback;
        }

        void in(const STROM_CHANNEL_ID pChannel, const float pData) override {
            switch (pChannel) {
                case CH_IN_TRIGGER:
                    mTrigger = pData;
                    STROM_LOG("+++ @StromNodeNote  Node(%02d) receives trigger : %f", ID(), mTrigger);
                    STROM_LOG("+++ @StromNodeNote      playing note          : pitch(%f) velocity(%f)", mPitch, mVelocity);
                    if (mCallback != nullptr) {
                        mCallback->trigger_note(mPitch, mVelocity);
                    }
                    break;
                case CH_IN_PITCH:
                    mPitch = pData;
                    STROM_LOG("+++ @StromNodeNote  Node(%02d) receives pitch   : %f", ID(), mPitch);
                    break;
                case CH_IN_VELOCITY:
                    mVelocity = pData;
                    STROM_LOG("+++ @StromNodeNote  Node(%02d) receives velocity: %f", ID(), mVelocity);
                    break;
            }
        }

        float out(const STROM_CHANNEL_ID pChannel) override {
            return STROM_NO_VALUE;
        }

    private:
        float mTrigger = 0.0;
        float mPitch = 0.0;
        float mVelocity = 0.0;
        StromNodeNoteCallback* mCallback = nullptr;
    };
}  // namespace strom

#endif /* StromNodeNote_hpp */
