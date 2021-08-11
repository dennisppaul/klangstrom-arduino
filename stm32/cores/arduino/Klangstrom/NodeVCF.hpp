//
//  NodeVCF.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_VCF            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| CUTOFF              |
 * IN02--| RESONANCE           |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeVCF_hpp
#define NodeVCF_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeVCF : public Node {
    public:
        static const CHANNEL_ID CH_IN_CUTOFF    = 1;
        static const CHANNEL_ID CH_IN_RESONANCE = 2;
        static const CHANNEL_ID NUM_CH_IN       = 3;
        static const CHANNEL_ID NUM_CH_OUT      = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_CUTOFF) {
                mConnection_CH_IN_CUTOFF = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_RESONANCE) {
                mConnection_CH_IN_RESONANCE = pConnection;
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
                    // @TODO(@feature, "instead of updating directly into the incoming block, an extra block could be created here then after updating all other input blocks, the output will be processed into that block. in step 3 data will only be copied …")
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                if (mConnection_CH_IN_CUTOFF != nullptr) {
                    mBlock_CUTOFF = AudioBlockPool::instance().request();
                    mConnection_CH_IN_CUTOFF->update(AudioBlockPool::instance().data(mBlock_CUTOFF));
                }
                if (mConnection_CH_IN_RESONANCE != nullptr) {
                    mBlock_RESONANCE = AudioBlockPool::instance().request();
                    mConnection_CH_IN_RESONANCE->update(AudioBlockPool::instance().data(mBlock_RESONANCE));
                }
                flag_updated();
            }

            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    if (mBlock_CUTOFF != AudioBlockPool::NO_ID) {
                        set_cutoff(AudioBlockPool::instance().data(mBlock_CUTOFF)[i]);
                    }
                    if (mBlock_RESONANCE != AudioBlockPool::NO_ID) {
                        set_resonance(AudioBlockPool::instance().data(mBlock_RESONANCE)[i]);
                    }
                    pAudioBlock[i] = mCutoff > 0.0 ? process(pAudioBlock[i]) : 0.0;  // @NOTE("returning 0.0 instead of `mSignal` yields more consistent results")
                }
            }
        }

        void set_cutoff_Hz(const float pCutoffHz) {
            /* cutoff frequency in Hz [0, sampling_rate/2] */
            const float a = KLANG_AUDIO_RATE_UINT16 * 0.5;
            mCutoff       = pCutoffHz / a;
            mCutoff       = mCutoff > 1.0f ? 1.0f : (mCutoff < 0.0f ? 0.0f : mCutoff);
        }

        inline void set_cutoff(const float pCutoff) {
            mCutoff = pCutoff;
        }

        inline void set_resonance(const float pResonance) {
            mResonance = pResonance;
        }

        float get_cutoff_Hz() {
            const float a = KLANG_AUDIO_RATE_UINT16 * 0.5;
            return mCutoff * a;
        }

        float get_cutoff() {
            return mCutoff;
        }

        float get_resonance() {
            return mResonance;
        }

    protected:
        float               mCutoff                    = 0.5f;
        float               mResonance                 = 0.5f;
        virtual SIGNAL_TYPE process(SIGNAL_TYPE input) = 0;

    private:
        AUDIO_BLOCK_ID mBlock_CUTOFF    = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_RESONANCE = AudioBlockPool::NO_ID;

        Connection* mConnection_CH_IN_SIGNAL    = nullptr;
        Connection* mConnection_CH_IN_CUTOFF    = nullptr;
        Connection* mConnection_CH_IN_RESONANCE = nullptr;
    };
}  // namespace klang

#endif /* NodeVCF_hpp */
