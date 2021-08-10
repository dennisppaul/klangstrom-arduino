//
//  NodeMixer2.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_MIXER2         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL_0     SIGNAL |--OUT00
 * IN01--| SIGNAL_1            |
 * IN02--| MIX                 |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeMixer2_hpp
#define NodeMixer2_hpp

#include "Node.hpp"

namespace klang {
    class NodeMixer2 : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL_0 = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_1 = 1;
        static const CHANNEL_ID CH_IN_MIX      = 2;
        static const CHANNEL_ID NUM_CH_IN      = 3;
        static const CHANNEL_ID NUM_CH_OUT     = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_0) {
                mConnection_CH_IN_SIGNAL_0 = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_1) {
                mConnection_CH_IN_SIGNAL_1 = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_MIX) {
                mConnection_CH_IN_MIX = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_0) {
                mConnection_CH_IN_SIGNAL_0 = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_1) {
                mConnection_CH_IN_SIGNAL_1 = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_MIX) {
                mConnection_CH_IN_MIX = nullptr;
                return true;
            }
            return false;
        }

        void set_mix(SIGNAL_TYPE pMix) {
            mMix = pMix;
        }

        SIGNAL_TYPE get_mix() {
            return mMix;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated() && pChannel == CH_OUT_SIGNAL && (mConnection_CH_IN_SIGNAL_0 != nullptr && mConnection_CH_IN_SIGNAL_1 != nullptr)) {
                AUDIO_BLOCK_ID mBlock_SIGNAL_0     = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_SIGNAL_1     = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_MIX          = AudioBlockPool::NO_ID;
                SIGNAL_TYPE*   mBlockData_SIGNAL_0 = nullptr;
                SIGNAL_TYPE*   mBlockData_SIGNAL_1 = nullptr;
                SIGNAL_TYPE*   mBlockData_MIX      = nullptr;

                if (mConnection_CH_IN_SIGNAL_0 != nullptr) {
                    mBlock_SIGNAL_0     = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_0 = AudioBlockPool::instance().data(mBlock_SIGNAL_0);
                    mConnection_CH_IN_SIGNAL_0->update(mBlock_SIGNAL_0);
                }
                if (mConnection_CH_IN_SIGNAL_1 != nullptr) {
                    mBlock_SIGNAL_1     = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_1 = AudioBlockPool::instance().data(mBlock_SIGNAL_1);
                    mConnection_CH_IN_SIGNAL_1->update(mBlock_SIGNAL_1);
                }
                if (mConnection_CH_IN_MIX != nullptr) {
                    mBlock_MIX     = AudioBlockPool::instance().request();
                    mBlockData_MIX = AudioBlockPool::instance().data(mBlock_MIX);
                    mConnection_CH_IN_MIX->update(mBlock_MIX);
                }

                const uint8_t mSignalInputCounter =
                    (mBlockData_SIGNAL_0 == nullptr ? 0 : 1) +
                    (mBlockData_SIGNAL_1 == nullptr ? 0 : 1);
                if (mSignalInputCounter == 0) {
                    memset(pAudioBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
                } else {
                    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; ++i) {
                        const float m0 = (mBlockData_MIX != nullptr) ? (mBlockData_MIX[i]) : mMix;
                        const float m1 = 1.0 - m0;
                        const float s0 = (mBlockData_SIGNAL_0 != nullptr) ? (mBlockData_SIGNAL_0[i] * m0) : 0.0;
                        const float s1 = (mBlockData_SIGNAL_1 != nullptr) ? (mBlockData_SIGNAL_1[i] * m1) : 0.0;
                        const float s  = s0 + s1;
                        pAudioBlock[i] = s / (float)mSignalInputCounter;
                    }
                }
                AudioBlockPool::instance().release(mBlock_SIGNAL_0);
                AudioBlockPool::instance().release(mBlock_SIGNAL_1);
                AudioBlockPool::instance().release(mBlock_MIX);

                flag_updated();
            } else {
                memset(pAudioBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
            }
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIX_F32:
                    set_mix(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    private:
        Connection* mConnection_CH_IN_SIGNAL_0 = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_1 = nullptr;
        Connection* mConnection_CH_IN_MIX      = nullptr;

        SIGNAL_TYPE mMix = 0.5;
    };
}  // namespace klang

#endif /* NodeMixer2_hpp */
