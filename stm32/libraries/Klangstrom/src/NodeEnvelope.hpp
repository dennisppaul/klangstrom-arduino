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
        EnvelopeStage(float pValue, float pDuration) : duration(pDuration), value(pValue) {}
        EnvelopeStage(float pValue) : duration(0.0), value(pValue) {}
        EnvelopeStage() {}
        float duration = 0.0;
        float value = 0.0;
    };
    
    class NodeEnvelope : public Node {
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
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = kernel(pAudioBlock[i]);
                }
            }
        }

        std::vector<EnvelopeStage>& stages() {
            return mEnvelopeStages;
        }
        
        void add_stage(float pValue, float pDuration) {
            mEnvelopeStages.push_back(EnvelopeStage(pValue, pDuration));
        }
        
        void add_stage(float pValue) {
            mEnvelopeStages.push_back(EnvelopeStage(pValue, 0.0));
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
        
    protected:
        SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            if (!mEnvelopeDone) {
                if (mEnvStage < mEnvelopeStages.size()) {
                    mValue += mTimeScale * mDelta ;
                    mStageDuration += mTimeScale * 1.0 / (float) KLANG_AUDIO_RATE_UINT16;
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
            return (mConnection_CH_IN_SIGNAL==nullptr) ? mValue : s * mValue;
        }
        
    private:
        Connection* mConnection_CH_IN_SIGNAL   = nullptr;

        std::vector<EnvelopeStage> mEnvelopeStages;
        
        uint16_t mEnvStage = 0;
        float mValue = 0.0;
        float mDelta = 0.0;
        float mTimeScale = 1.0;
        float mStageDuration = 0.0;
        bool mEnvelopeDone = true;
        
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

        float compute_delta_fraction(float pDelta, float pDuration) {
            return pDuration > 0 ? (pDelta / KLANG_AUDIO_RATE_UINT16) / pDuration : pDelta;
        }
    };
}

#endif /* NodeEnvelope_hpp */
