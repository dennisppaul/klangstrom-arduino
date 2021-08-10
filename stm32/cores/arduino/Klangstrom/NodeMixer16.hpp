//
//  NodeMixer16.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_MIXER_16       ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL_00    SIGNAL |--OUT00
 *   ... | ...                 |
 * IN16--| SIGNAL_16           |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeMixer16_hpp
#define NodeMixer16_hpp

#include "Node.hpp"

namespace klang {
    class NodeMixer16 : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 16;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeMixer16() {
            for (uint8_t i = 0; i < NUM_CH_IN; ++i) {
                mConnection_CH_IN_SIGNAL[i] = nullptr;
                mMix[i]                     = 1.0;
            }
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel < NUM_CH_IN) {
                mConnection_CH_IN_SIGNAL[pInChannel] = pConnection;
                return true;
            } else {
                return false;
            }
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel < NUM_CH_IN) {
                mConnection_CH_IN_SIGNAL[pInChannel] = nullptr;
                return true;
            } else {
                return false;
            }
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            bool    m_has_SIGNAL[NUM_CH_IN];
            uint8_t mSignalInputCounter = 0;
            for (uint8_t i = 0; i < NUM_CH_IN; ++i) {
                const bool mHasSignal = (mConnection_CH_IN_SIGNAL[i] != nullptr);
                m_has_SIGNAL[i]       = mHasSignal;
                mSignalInputCounter += mHasSignal;
            }
            if (is_not_updated() && pChannel == CH_OUT_SIGNAL && mSignalInputCounter > 0) {
                AUDIO_BLOCK_ID mBlock_SIGNAL[NUM_CH_IN];
                SIGNAL_TYPE*   mBlockData_SIGNAL[NUM_CH_IN];

                for (uint8_t i = 0; i < NUM_CH_IN; ++i) {
                    if (m_has_SIGNAL[i]) {
                        mBlock_SIGNAL[i] = AudioBlockPool::instance().request();
                        if (mBlock_SIGNAL[i] == AudioBlockPool::NO_ID) {
                            // @note(probably ran out of memory blocks @maybe(implement some better error handling))
                            memset(pAudioBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
                            return;
                        }
                        mBlockData_SIGNAL[i] = AudioBlockPool::instance().data(mBlock_SIGNAL[i]);
                        mConnection_CH_IN_SIGNAL[i]->update(mBlock_SIGNAL[i]);
                    }
                }

                const float mInverseSigCounter = 1.0 / mSignalInputCounter;
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; ++i) {
                    float sum = 0.0;
                    for (uint8_t j = 0; j < NUM_CH_IN; ++j) {
                        const float s = m_has_SIGNAL[j] ? (mBlockData_SIGNAL[j][i] * mMix[j]) : 0.0;
                        sum += s;
                    }
                    pAudioBlock[i] = sum * mInverseSigCounter;
                }

                for (uint8_t i = 0; i < NUM_CH_IN; ++i) {
                    AudioBlockPool::instance().release(mBlock_SIGNAL[i]);
                }

                flag_updated();
            } else {
                memset(pAudioBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
            }
        }

        void set_mix(uint8_t pChannel, float pValue) {
            mMix[pChannel] = pValue;
        }

        SIGNAL_TYPE get_mix(uint8_t pChannel) {
            return mMix[pChannel];
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIX_F32:
                    set_mix(static_cast<uint8_t>(pPayLoad[0]), KlangMath::FLOAT_32(pPayLoad, 1));
                    break;
            }
        }

    private:
        Connection* mConnection_CH_IN_SIGNAL[NUM_CH_IN];
        float       mMix[NUM_CH_IN];
    };
}  // namespace klang

#endif /* NodeMixer16_hpp */
