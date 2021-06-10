//
//  NodeEnvelope.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_ENVELOPE       ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| TRIGGER     TRIGGER |--OUT01
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeEnvelope_hpp
#define NodeEnvelope_hpp

#include "NodeKernel.hpp"
#include <vector>

namespace klang {
    class EnvelopeStage {
    public:
        EnvelopeStage(float pDuration, float pValue) : duration(pDuration), value(pValue) {}
        EnvelopeStage(float pValue) : duration(0.0), value(pValue) {}
        EnvelopeStage() {}
        float duration = 0.0;
        float value = 0.0;
    };
    
    class NodeEnvelope : public Node {
    public:
        static const CHANNEL_ID CH_IN_TRIGGER   = 1;
        static const CHANNEL_ID NUM_CH_IN       = 2;

        static const CHANNEL_ID CH_OUT_TRIGGER  = 1;
        static const CHANNEL_ID NUM_CH_OUT      = 2;
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            }
            else if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            }
            else if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = nullptr;
                return true;
            }
            return false;
        }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
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
                    pAudioBlock[i] = kernel(pAudioBlock[i]);
                }
            }
            else if (pChannel == CH_OUT_TRIGGER) {
                const bool mHasTriggerSignal = ( mBlock_TRIGGER != AudioBlockPool::NO_ID);
                if (mHasTriggerSignal) {
                    SIGNAL_TYPE* mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                    KLANG_COPY_AUDIO_BUFFER(pAudioBlock, mBlockData_TRIGGER);
                } else {
                    KLANG_FILL_AUDIO_BUFFER(pAudioBlock, 0.0);
                }
            }
        }

        std::vector<EnvelopeStage>& stages() {
            return mEnvelopeStages;
        }
        
        void add_stage(float pDuration, float pValue) {
            add_stage_ms(pDuration * M_TIME_SCALE, pValue);
        }        
        
        void add_stage_ms(float pDuration, float pValue) {
            mEnvelopeStages.push_back(EnvelopeStage(pDuration, pValue));
        }
        
        void add_stage(float pValue) {
            add_stage_ms(0.0, pValue);
        }
        
        void clear_stages() {
            mEnvelopeStages.clear();
        }
        
        void start() {
            mEnvelopeDone = false;
            prepareNextStage(0);
        }
        
        void stop() {
            mEnvelopeDone = true;
        }
        
        void set_time_scale(SIGNAL_TYPE pTimeScale) {
            mTimeScale = pTimeScale;
        }
        
        SIGNAL_TYPE get_time_scale() {
            return mTimeScale;
        }  

        void set_value_scale(SIGNAL_TYPE pValueScale) {
            mValueScale = pValueScale;
        }
        
        SIGNAL_TYPE get_value_scale() {
            return mValueScale;
        }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_TIME_SCALE_F32:
                    set_time_scale(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_ADD_STAGE_F32_F32:
                    add_stage(KlangMath::FLOAT_32(pPayLoad, 0), KlangMath::FLOAT_32(pPayLoad, 4));
                    break;
                case KLANG_SET_CLEAR_STAGES:
                    clear_stages();
                    break;
                case KLANG_SET_START:
                    start();
                    break;
                case KLANG_SET_STOP:
                    stop();
                    break;
            }
        }
        
    private:
        Connection* mConnection_CH_IN_SIGNAL    = nullptr;
        Connection* mConnection_CH_IN_TRIGGER   = nullptr;

        AUDIO_BLOCK_ID mBlock_TRIGGER           = AudioBlockPool::NO_ID;

        static constexpr SIGNAL_TYPE RAMP_NO_EDGE      =  0.0;
        static constexpr SIGNAL_TYPE RAMP_RISING_EDGE  =  1.0;
        static constexpr SIGNAL_TYPE RAMP_FALLING_EDGE = -1.0;
        static constexpr float       mThreshold        =  0.0; // @todo(could be made configurable)
        static constexpr float       M_TIME_SCALE      =  1000;
        static constexpr float       KLANG_AUDIO_RATE_UINT16_FRAC_INV = M_TIME_SCALE / KLANG_AUDIO_RATE_UINT16;

        std::vector<EnvelopeStage> mEnvelopeStages;
        
        uint16_t    mEnvStage       = 0;
        float       mValue          = 0.0;
        float       mDelta          = 0.0;
        float       mTimeScale      = 1.0;
        float       mValueScale     = 1.0;
        float       mStageDuration  = 0.0;
        bool        mEnvelopeDone   = true;
        SIGNAL_TYPE mPreviousSample = mThreshold;
        
        inline SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            if (!mEnvelopeDone) {
                if (mEnvStage < mEnvelopeStages.size()) {
                    mValue += mTimeScale * mDelta ;
                    mStageDuration += mTimeScale * KLANG_AUDIO_RATE_UINT16_FRAC_INV;
                    if (mStageDuration > mEnvelopeStages[mEnvStage].duration) {
                        mEnvStage++;
                        if (mEnvStage < mEnvelopeStages.size() - 1) {
                            prepareNextStage(mEnvStage);
                        } else {
                            // @TODO(is it of interest to be able to loop envelopes?)
                            stop();
                        }
                    }
                }
            }
            const bool mHasInputSignal = (mConnection_CH_IN_SIGNAL!=nullptr);
            const SIGNAL_TYPE a = mHasInputSignal ? s * mValue : mValue;
            return a * mValueScale;
        }

        void prepareNextStage(uint16_t pEnvStage) {
            mEnvStage = pEnvStage;
            mStageDuration = 0.0;
            mValue = mEnvelopeStages[mEnvStage].value;
            if (mEnvelopeStages.size() > 1) {
                setDelta(mEnvStage);
            }
        }
        
        void setDelta(uint16_t pEnvStage) {
            const float mDeltaTMP = mEnvelopeStages[pEnvStage + 1].value - mEnvelopeStages[pEnvStage].value;
            mDelta = compute_delta_fraction(mDeltaTMP, mEnvelopeStages[mEnvStage].duration);
        }

        float compute_delta_fraction(const float pDelta, const float pDuration) {
            if (pDuration > 0) {
                const float a = pDelta * KLANG_AUDIO_RATE_UINT16_FRAC_INV;
                return a / pDuration;
            } else {
                return pDelta;
            }
        }

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

#endif /* NodeEnvelope_hpp */
