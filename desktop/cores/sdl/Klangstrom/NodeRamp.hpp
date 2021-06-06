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
 *       |              SIGNAL |--OUT00
 *       |             TRIGGER |--OUT01
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

//@todo(consider a trigger via input signal)
//@todo(look into semantics `trigger` vs `start/stop` also look at `NodeADSR`)

#ifndef NodeRamp_hpp
#define NodeRamp_hpp

#include "Node.hpp"
#include "KlangMath.hpp"

namespace klang {
    class NodeRamp : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN       = 0;

        static const CHANNEL_ID CH_OUT_SIGNAL   = 0;
        static const CHANNEL_ID CH_OUT_TRIGGER  = 1;
        static const CHANNEL_ID NUM_CH_OUT      = 2;
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            return false;
        }
        
        void start() {
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
            mDuration = pDuration * M_TIME_SCALE;
            mStart = pStart; 
            mDestination = pDestination;
            recompute_delta();
        }

        void set_duration(float pDuration) { 
            mDuration = pDuration * M_TIME_SCALE;
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

        float get_duration() { return mDuration * M_TIME_SCALE; }
        float get_start() { return mCurrentValue; }
        float get_destination() { return mDestination; }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_START:
                    start();
                    break;
                    // @todo(add set start + desitination)
            }
        }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                mStateSwitchID     = KLANG_SAMPLES_PER_AUDIO_BLOCK;
                flag_updated();
            }
            
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    const bool mSwitchedState = step_float();
                    if (mSwitchedState) {
                        mStateSwitchID = i;
                    }
                    pAudioBlock[i] = mCurrentValue;
                }
            } else if (pChannel == CH_OUT_TRIGGER) {
                // @todo(use remembered `i` state switch) to populate buffer.
                //       problem: what happens if trigger channels is querried before signal channel?!?!)
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = (i > mStateSwitchID) ? 0 : 1;
                }
            }
        }
        
    private:
        
        const float M_TIME_SCALE = 1000000;
        enum class ENVELOPE_STATE {
            IDLE, RAMPING
        };
        ENVELOPE_STATE mState = ENVELOPE_STATE::IDLE;
        
        SIGNAL_TYPE mCurrentValue   = 0.0f;
        SIGNAL_TYPE mDeltaFraction  = 0.0f;
        SIGNAL_TYPE mStart          = 0.0f;
        SIGNAL_TYPE mDestination    = 0.0f;
        SIGNAL_TYPE mDuration       = 0.0f;
        uint16_t    mStateSwitchID  = KLANG_SAMPLES_PER_AUDIO_BLOCK;

        bool step_float() {
            if (mState == ENVELOPE_STATE::RAMPING) {
                mCurrentValue += mDeltaFraction;
                const bool mDirection = mDeltaFraction > 0;
                const bool mEvalCondition = mDirection ? (mCurrentValue >= mDestination) : (mCurrentValue <= mDestination);
                if (mEvalCondition) {
                        mCurrentValue = mDestination;
                        mState = ENVELOPE_STATE::IDLE;
                        return true;
                }
            }
            return false;
        }
        
        float compute_delta_fraction(const float pDelta, const float pDuration) {
            const float c = pDelta * M_TIME_SCALE;
            const float a = c / KLANG_AUDIO_RATE_UINT16;
            const float b = a / pDuration;
            return (pDuration > 0) ? b : pDelta;
        }
        
        void recompute_delta() {
            const float mDeltaTotal = mDestination - mStart;
            const float mDelta = mDestination - mCurrentValue;
            const float mDurationFraction = mDuration * (mDelta / mDeltaTotal);
            mDeltaFraction = compute_delta_fraction(mDelta, mDurationFraction);
        }
    };
}

#endif /* NodeRamp_hpp */
