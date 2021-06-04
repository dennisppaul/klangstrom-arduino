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

#include "Node.hpp"
#include "KlangMath.hpp"

namespace klang {
    class NodeADSR : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN         = 1;
        static const CHANNEL_ID NUM_CH_OUT        = 1;
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            }
            return false;
        }
        
        void trigger(SIGNAL_TYPE pTrigger) {
            mTrigger = pTrigger;
        }
        
        void start() {
            trigger(1);
            trigger_attack();
        }
        
        void stop() {
            trigger(0);
            trigger_release();
        }
        
        void set_attack(float pAttack) { mAttack = pAttack; }
        void set_decay(float pDecay) { mDecay = pDecay; }
        void set_sustain(float pSustain) { mSustain = pSustain; }
        void set_release(float pRelease) { mRelease = pRelease; }
        float get_attack() { return mAttack; }
        float get_decay() { return mDecay; }
        float get_sustain() { return mSustain; }
        float get_release() { return mRelease; }
        
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
                flag_updated();
            }
            
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
#if (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_INT16)
                    pAudioBlock[i] = pAudioBlock[i];
                    //@TODO("multiply with amplitude")
#elif (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_FLOAT)
                    step_float();
                    pAudioBlock[i] = mNoInputSignal ? mAmp : pAudioBlock[i] * mAmp;
#else
#error KLANG_SIGNAL_TYPE not defined
#endif
                }
            }
        }
        
        void step_float() {
            switch (mState) {
                case ENVELOPE_STATE::IDLE:
                    trigger_attack();
                    break;
                case ENVELOPE_STATE::ATTACK:
                    trigger_release();
                    // increase amp to sustain_level in ATTACK sec
                    mAmp += mDelta;
                    if (mAmp >= 1.0f) {
                        mAmp = 1.0f;
                        mDelta = compute_delta_fraction(-(mAmp - mSustain), mDecay);
                        //-((mAmp - mSustain) / (float) KLANG_AUDIO_RATE_UINT16) / mDecay;
                        mState = ENVELOPE_STATE::DECAY;
#ifdef DEBUG_ADSR
                        KLANG_LOG("### @NodeADSR(%i) DECAY\n", ID());
#endif
                    }
                    break;
                case ENVELOPE_STATE::DECAY:
                    trigger_release();
                    // decrease amp to sustain_level in DECAY sec
                    mAmp += mDelta;
                    if (mAmp <= mSustain) {
                        mAmp = mSustain;
                        mState = ENVELOPE_STATE::SUSTAIN;
#ifdef DEBUG_ADSR
                        KLANG_LOG("### @NodeADSR(%i) SUSTAIN\n", ID());
#endif
                    }
                    break;
                case ENVELOPE_STATE::SUSTAIN:
                    trigger_release();
                    break;
                case ENVELOPE_STATE::RELEASE:
                    trigger_attack();
                    // decrease amp to 0.0 in RELEASE sec
                    mAmp += mDelta;
                    if (mAmp <= 0.0f) {
                        mAmp = 0.0f;
                        mState = ENVELOPE_STATE::IDLE;
#ifdef DEBUG_ADSR
                        KLANG_LOG("### @NodeADSR(%i) IDLE\n", ID());
#endif
                    }
                    break;
                case ENVELOPE_STATE::FADE_TO_ZERO:
#define FADE_TO_ZERO_RATE_SEC 0.1
                    mAmp -= 1.0 / (KLANG_AUDIO_RATE_UINT16 * FADE_TO_ZERO_RATE_SEC);
                    if (mAmp <= 0.0) {
                        mState = ENVELOPE_STATE::ATTACK;
                    }
                    break;
            }
        }
        
    private:
        Connection* mConnection_CH_IN_SIGNAL   = nullptr;
        
        SIGNAL_TYPE mTrigger        = 0.0f;
        SIGNAL_TYPE mAttack         = 0.01f;
        SIGNAL_TYPE mDecay          = 0.05f;
        SIGNAL_TYPE mSustain        = 0.5f;
        SIGNAL_TYPE mRelease        = 0.25f;
        
        enum class ENVELOPE_STATE {
            IDLE, ATTACK, DECAY, SUSTAIN, RELEASE, FADE_TO_ZERO
        };
        ENVELOPE_STATE mState = ENVELOPE_STATE::IDLE;
        
        float mAmp = 0.0f;
        float mDelta = 0.0f;
        bool mNoInputSignal = true;
        
        float compute_delta_fraction(float pDelta, float pDuration) {
            return pDuration > 0 ? (pDelta / KLANG_AUDIO_RATE_UINT16) / pDuration : pDelta;
        }
        
        void trigger_attack() {
            if (mTrigger > 0.0f) {
                mDelta = compute_delta_fraction(1.0 - mAmp, mAttack);//(1.0f / (float) KLANG_AUDIO_RATE_UINT16) / mAttack;
                if (mAmp > 0.0f) {
                    mState = ENVELOPE_STATE::FADE_TO_ZERO;
                } else {
                    mState = ENVELOPE_STATE::ATTACK;
                }
            }
        }
        
        void trigger_release() {
            if (mTrigger <= 0.0f) {
                mDelta = compute_delta_fraction(-mAmp, mRelease);//-(mAmp / (float) KLANG_AUDIO_RATE_UINT16) / mRelease;
                mState = ENVELOPE_STATE::RELEASE;
            }
        }
    };
}

#endif /* NodeADSR_hpp */
