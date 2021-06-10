//
//  NodeRamp.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_RAMP           ]
 *       +---------------------+
 *       |                     |
 * IN00--| TRIGGER      SIGNAL |--OUT00
 *       |             TRIGGER |--OUT01
 *       |                     |
 *       +---------------------+
 * 
 *       @description(
 *          the ramp’s duration, start and end value can be specified. however, the ramping process
 *          only starts when the `start()` function is called. if duration, start and end values
 *          are changed during ramping the ramping behavior changes. if the same values are changed
 *          while the ramp is not ramping no behavior change occurs until the `start()` function is 
 *          called.
 *       )
 *
 */

//@todo(make edge behavior configurable. curerntly `RISING`==`start()` + `FALLING`==`stop()` )

#ifndef NodeRamp_hpp
#define NodeRamp_hpp

#include "Node.hpp"
#include "KlangMath.hpp"

namespace klang {
    class NodeRamp : public Node {
    public:
        static const CHANNEL_ID CH_IN_TRIGGER   = 0;
        static const CHANNEL_ID NUM_CH_IN       = 1;

        static const CHANNEL_ID CH_OUT_TRIGGER  = 1;
        static const CHANNEL_ID NUM_CH_OUT      = 1;
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = nullptr;
                return true;
            }
            return false;
        }
        
        void start() {
            mCurrentValue = mStart; 
            mState = ENVELOPE_STATE::RAMPING;
        }

        void resume() {
            mState = ENVELOPE_STATE::RAMPING;
        }

        void stop() {
            mState = ENVELOPE_STATE::IDLE;
        }

        void set_current_value(float pCurrent) { 
            mCurrentValue = pCurrent; 
            recompute_delta();
        }

        void set_ramp(float pDuration, float pStart, float pDestination) { 
            set_ramp_ms(pDuration * M_TIME_SCALE, pStart, pDestination);
        }

        void set_ramp_ms(float pDuration, float pStart, float pDestination) { 
            mDuration = pDuration;
            mStart = pStart; 
            mDestination = pDestination;
            recompute_delta();
        }

        void set_duration(float pDuration) { 
            set_duration_ms(pDuration * M_TIME_SCALE);
        }
        
        void set_duration_ms(float pDuration) { 
            mDuration = pDuration;
            recompute_delta();
        }

        void set_start(float pStart) { 
            mStart = pStart; 
            recompute_delta();
        }

        void set_destination(float pDestination) { 
            mDestination = pDestination; 
            recompute_delta();
        }

        float get_duration() { return mDuration / M_TIME_SCALE; }
        float get_start() { return mCurrentValue; }
        float get_destination() { return mDestination; }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_START:
                    start();
                    break;
                case KLANG_SET_STOP:
                    stop();
                    break;
                    // @todo(add set start + desitination + set_ramp)
            }
        }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                mBlock_TRIGGER = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_TRIGGER != nullptr) {
                    mBlock_TRIGGER = AudioBlockPool::instance().request();
                    mConnection_CH_IN_TRIGGER->update(mBlock_TRIGGER);
                    SIGNAL_TYPE* mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        const float mCurrentSample = mBlockData_TRIGGER[i];
                        mBlockData_TRIGGER[i] = evaluateEdge(mPreviousSample, mCurrentSample);
                        mPreviousSample = mCurrentSample;
                    }
                }
                flag_updated();
            }
            
            if (pChannel == CH_OUT_SIGNAL) {
                const bool mHasTriggerSignal = ( mBlock_TRIGGER != AudioBlockPool::NO_ID);
                SIGNAL_TYPE* mBlockData_TRIGGER = nullptr;
                if (mHasTriggerSignal) {
                    mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                }
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    if (mHasTriggerSignal) {
                        const SIGNAL_TYPE mTriggerState = mBlockData_TRIGGER[i];
                        if (mTriggerState == RAMP_RISING_EDGE) {
                            start();
                        } else if (mTriggerState == RAMP_FALLING_EDGE) {
                            stop();
                        }
                    }
                    kernel();
                    pAudioBlock[i] = mCurrentValue;
                }
            } else if (pChannel == CH_OUT_TRIGGER) {
                const bool mHasTriggerSignal = ( mBlock_TRIGGER != AudioBlockPool::NO_ID);
                if (mHasTriggerSignal) {
                    SIGNAL_TYPE* mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                    KLANG_COPY_AUDIO_BUFFER(pAudioBlock, mBlockData_TRIGGER);
                } else {
                    KLANG_FILL_AUDIO_BUFFER(pAudioBlock, 0.0);
                }
            }
        }

    private:
        Connection* mConnection_CH_IN_TRIGGER   = nullptr;

        AUDIO_BLOCK_ID mBlock_TRIGGER           = AudioBlockPool::NO_ID;

        static constexpr float M_TIME_SCALE = 1;
        static constexpr float KLANG_AUDIO_RATE_UINT16_INV = 1.0 / KLANG_AUDIO_RATE_UINT16;
        enum class ENVELOPE_STATE {
            IDLE, RAMPING
        };
        ENVELOPE_STATE mState = ENVELOPE_STATE::IDLE;
        
        SIGNAL_TYPE mCurrentValue   = 0.0;
        SIGNAL_TYPE mDeltaFraction  = 0.0;
        SIGNAL_TYPE mStart          = 0.0;
        SIGNAL_TYPE mDestination    = 0.0;
        SIGNAL_TYPE mDuration       = 0.0;
        static constexpr float mThreshold = 0.0; // @todo(could be made configurable)
        SIGNAL_TYPE mPreviousSample = mThreshold;

        void kernel() {
            if (mState == ENVELOPE_STATE::RAMPING) {
                mCurrentValue += mDeltaFraction;
                const bool mDirection = mDeltaFraction > 0;
                const bool mEvalCondition = mDirection ? (mCurrentValue >= mDestination) : (mCurrentValue <= mDestination);
                if (mEvalCondition) {
                        mCurrentValue = mDestination;
                        mState = ENVELOPE_STATE::IDLE;
                }
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
        
        void recompute_delta() {
            const float mDeltaTotal = mDestination - mStart;
            const float mDelta = mDestination - mCurrentValue;
            const float mDurationFraction = mDuration * (mDelta / mDeltaTotal);
            mDeltaFraction = compute_delta_fraction(mDelta, mDurationFraction);
        }

        static constexpr SIGNAL_TYPE RAMP_NO_EDGE      =  0.0;
        static constexpr SIGNAL_TYPE RAMP_RISING_EDGE  =  1.0;
        static constexpr SIGNAL_TYPE RAMP_FALLING_EDGE = -1.0;

        const SIGNAL_TYPE evaluateEdge(const SIGNAL_TYPE pPreviousSample, const SIGNAL_TYPE pCurrentSample) {
            if ((pPreviousSample < mThreshold) && (pCurrentSample > mThreshold)) {
                return RAMP_RISING_EDGE;
            } else if ((pPreviousSample > mThreshold) && (pCurrentSample < mThreshold)) {
                return RAMP_FALLING_EDGE;
            } else {
                return RAMP_NO_EDGE;
            }
         }
    };
}

#endif /* NodeRamp_hpp */
