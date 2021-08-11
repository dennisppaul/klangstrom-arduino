//
//  NodeADSR.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_ADSR           ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| TRIGGER     TRIGGER |--OUT01
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          the ADSR node provides a envelope with four different stages: 
 *          (A)ttack (D)ecay (S)ustain (R)elease. it is usually used to 
 *          control the amplitude of an *oscillator*.
 *
 *          |----->|-->|   |-->|
 *          |---A--|-D-|-S-|-R-|
 *          |      /\
 *          |     /  \
 *          |    /    \_____
 *          |   /        ^  \
 *          |  /         |   \
 *          | /          |    \
 *          |/___________|_____\
 *          |
 *          |Press          |Release
 *       )
 *
 *       @set_value(ATTACK_F32,DECAY_F32,SUSTAIN_F32,RELEASE_F32,START,STOP)
 *
 */

#ifndef NodeADSR_hpp
#define NodeADSR_hpp

#include "KlangMath.hpp"
#include "KlangNode.hpp"

namespace klang {
    class NodeADSR : public Node {
    public:
        static const CHANNEL_ID CH_IN_TRIGGER = 1;
        static const CHANNEL_ID NUM_CH_IN     = 2;

        static const CHANNEL_ID CH_OUT_TRIGGER = 1;
        static const CHANNEL_ID NUM_CH_OUT     = 2;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            } else if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            } else if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = nullptr;
                return true;
            }
            return false;
        }

        void start() {
            check_scheduled_attack_state();
        }

        void stop() {
            check_scheduled_release_state();
        }

        void set_attack(float pAttack) { mAttack = pAttack * M_TIME_SCALE; }
        void set_decay(float pDecay) { mDecay = pDecay * M_TIME_SCALE; }
        void set_sustain(float pSustain) { mSustain = pSustain; }
        void set_release(float pRelease) { mRelease = pRelease * M_TIME_SCALE; }

        void set_attack_ms(float pAttack) { mAttack = pAttack; }
        void set_decay_ms(float pDecay) { mDecay = pDecay; }
        void set_release_ms(float pRelease) { mRelease = pRelease; }

        float get_attack() { return mAttack / M_TIME_SCALE; }
        float get_decay() { return mDecay / M_TIME_SCALE; }
        float get_sustain() { return mSustain; }
        float get_release() { return mRelease / M_TIME_SCALE; }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_ATTACK_F32:
                    set_attack(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_DECAY_F32:
                    set_decay(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_SUSTAIN_F32:
                    set_sustain(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_RELEASE_F32:
                    set_release(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_START:
                    start();
                    break;
                case KLANG_SET_STOP:
                    stop();
                    break;
            }
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            /*
             *
             * |----->|-->|   |-->|
             * |---A--|-D-|-S-|-R-|
             * |      /\
             * |     /  \
             * |    /    \_____
             * |   /        ^  \
             * |  /         |   \
             * | /          |    \
             * |/___________|_____\
             * |
             * |Press         |Release
             *
             */

            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                    mNoInputSignal = false;
                } else {
                    mNoInputSignal = true;
                }
                mBlock_TRIGGER = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_TRIGGER != nullptr) {
                    mBlock_TRIGGER = AudioBlockPool::instance().request();
                    mConnection_CH_IN_TRIGGER->update(mBlock_TRIGGER);
                    SIGNAL_TYPE* mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        const float mCurrentSample = mBlockData_TRIGGER[i];
                        mBlockData_TRIGGER[i]      = evaluateEdge(mPreviousSample, mCurrentSample);
                        mPreviousSample            = mCurrentSample;
                    }
                }
                flag_updated();
            }

            if (pChannel == CH_OUT_SIGNAL) {
                const bool   mHasTriggerSignal  = (mBlock_TRIGGER != AudioBlockPool::NO_ID);
                SIGNAL_TYPE* mBlockData_TRIGGER = nullptr;
                if (mHasTriggerSignal) {
                    mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                }
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    if (mHasTriggerSignal) {
                        const SIGNAL_TYPE mTriggerState = mBlockData_TRIGGER[i];
                        if (mTriggerState == RAMP_RISING_EDGE) {
                            start();
                        } else if (mTriggerState == RAMP_FALLING_EDGE) {
                            stop();
                        }
                    }
                    kernel();
                    pAudioBlock[i] = mNoInputSignal ? mAmp : pAudioBlock[i] * mAmp;
                }
            } else if (pChannel == CH_OUT_TRIGGER) {
                const bool mHasTriggerSignal = (mBlock_TRIGGER != AudioBlockPool::NO_ID);
                if (mHasTriggerSignal) {
                    SIGNAL_TYPE* mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                    KLANG_COPY_AUDIO_BUFFER(pAudioBlock, mBlockData_TRIGGER);
                } else {
                    KLANG_FILL_AUDIO_BUFFER(pAudioBlock, 0.0);
                }
            }
        }

    private:
        Connection* mConnection_CH_IN_SIGNAL  = nullptr;
        Connection* mConnection_CH_IN_TRIGGER = nullptr;

        AUDIO_BLOCK_ID mBlock_TRIGGER = AudioBlockPool::NO_ID;

        static constexpr SIGNAL_TYPE RAMP_NO_EDGE                = 0.0;
        static constexpr SIGNAL_TYPE RAMP_RISING_EDGE            = 1.0;
        static constexpr SIGNAL_TYPE RAMP_FALLING_EDGE           = -1.0;
        static constexpr float       mThreshold                  = 0.0;  // @todo(could be made configurable)
        static constexpr float       M_TIME_SCALE                = 1000;
        static constexpr float       KLANG_AUDIO_RATE_UINT16_INV = 1.0 / KLANG_AUDIO_RATE_UINT16;

        SIGNAL_TYPE mAttack         = 0.01f;
        SIGNAL_TYPE mDecay          = 0.05f;
        SIGNAL_TYPE mSustain        = 0.5f;
        SIGNAL_TYPE mRelease        = 0.25f;
        SIGNAL_TYPE mPreviousSample = mThreshold;

        enum class ENVELOPE_STATE {
            IDLE,
            ATTACK,
            DECAY,
            SUSTAIN,
            RELEASE,
            PRE_ATTACK_FADE_TO_ZERO
        };
        ENVELOPE_STATE mState = ENVELOPE_STATE::IDLE;

        float mAmp           = 0.0f;
        float mDelta         = 0.0f;
        bool  mNoInputSignal = true;

        const SIGNAL_TYPE evaluateEdge(const SIGNAL_TYPE pPreviousSample, const SIGNAL_TYPE pCurrentSample) {
            if ((pPreviousSample < mThreshold) && (pCurrentSample > mThreshold)) {
                return RAMP_RISING_EDGE;
            } else if ((pPreviousSample > mThreshold) && (pCurrentSample < mThreshold)) {
                return RAMP_FALLING_EDGE;
            } else {
                return RAMP_NO_EDGE;
            }
        }

        float compute_delta_fraction(const float pDelta, const float pDuration) {
            if (pDuration > 0) {
                const float c = pDelta * M_TIME_SCALE;
                const float a = c * KLANG_AUDIO_RATE_UINT16_INV;
                return a / pDuration;
            } else {
                return pDelta;
            }
        }

        inline void setState(ENVELOPE_STATE pState) {
            mState = pState;
        }

        void check_scheduled_attack_state() {
            mDelta = compute_delta_fraction(1.0f, mAttack);
            setState(ENVELOPE_STATE::ATTACK);
        }

        void check_scheduled_release_state() {
            if (mState != ENVELOPE_STATE::RELEASE) {
                mDelta = compute_delta_fraction(-mAmp, mRelease);
                setState(ENVELOPE_STATE::RELEASE);
            }
        }

        inline void kernel() {
            switch (mState) {
                case ENVELOPE_STATE::IDLE:
                case ENVELOPE_STATE::SUSTAIN:
                    break;
                case ENVELOPE_STATE::ATTACK:
                    // increase amp to sustain_level in ATTACK sec
                    mAmp += mDelta;
                    if (mAmp >= 1.0f) {
                        mAmp   = 1.0f;
                        mDelta = compute_delta_fraction(-(1.0f - mSustain), mDecay);
                        setState(ENVELOPE_STATE::DECAY);
                    }
                    break;
                case ENVELOPE_STATE::DECAY:
                    // decrease amp to sustain_level in DECAY sec
                    mAmp += mDelta;
                    if (mAmp <= mSustain) {
                        mAmp = mSustain;
                        setState(ENVELOPE_STATE::SUSTAIN);
                    }
                    break;
                case ENVELOPE_STATE::RELEASE:
                    // decrease amp to 0.0 in RELEASE sec
                    mAmp += mDelta;
                    if (mAmp <= 0.0f) {
                        mAmp = 0.0f;
                        setState(ENVELOPE_STATE::IDLE);
                    }
                    break;
                case ENVELOPE_STATE::PRE_ATTACK_FADE_TO_ZERO:
                    mAmp += mDelta;
                    if (mAmp <= 0.0f) {
                        mAmp   = 0.0f;
                        mDelta = compute_delta_fraction(1.0f, mAttack);
                        setState(ENVELOPE_STATE::ATTACK);
                    }
                    break;
            }
        }
    };
}  // namespace klang

#endif /* NodeADSR_hpp */
