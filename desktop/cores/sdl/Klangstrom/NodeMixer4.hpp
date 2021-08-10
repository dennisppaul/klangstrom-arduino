//
//  NodeMixer4.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_MIXER4         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL_0     SIGNAL |--OUT00
 * IN01--| SIGNAL_1            |
 * IN02--| SIGNAL_2            |
 * IN03--| SIGNAL_3            |
 *       |                     |
 *       +---------------------+
 */
// @TODO(implement `MIX` as INPUT)

#ifndef NodeMixer4_hpp
#define NodeMixer4_hpp

#include "Node.hpp"

namespace klang {
    class NodeMixer4 : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL_0 = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_1 = 1;
        static const CHANNEL_ID CH_IN_SIGNAL_2 = 2;
        static const CHANNEL_ID CH_IN_SIGNAL_3 = 3;
        static const CHANNEL_ID NUM_CH_IN      = 4;
        static const CHANNEL_ID NUM_CH_OUT     = 1;

        enum SIGNAL_CHANNEL { SIGNAL_0 = 0,
                              SIGNAL_1,
                              SIGNAL_2,
                              SIGNAL_3 };

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_0) {
                mConnection_CH_IN_SIGNAL_0 = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_1) {
                mConnection_CH_IN_SIGNAL_1 = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_2) {
                mConnection_CH_IN_SIGNAL_2 = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_3) {
                mConnection_CH_IN_SIGNAL_3 = pConnection;
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
            return false;
            if (pInChannel == CH_IN_SIGNAL_2) {
                mConnection_CH_IN_SIGNAL_2 = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_3) {
                mConnection_CH_IN_SIGNAL_3 = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            const bool    m_has_SIGNAL_0      = (mConnection_CH_IN_SIGNAL_0 != nullptr);
            const bool    m_has_SIGNAL_1      = (mConnection_CH_IN_SIGNAL_1 != nullptr);
            const bool    m_has_SIGNAL_2      = (mConnection_CH_IN_SIGNAL_2 != nullptr);
            const bool    m_has_SIGNAL_3      = (mConnection_CH_IN_SIGNAL_3 != nullptr);
            const uint8_t mSignalInputCounter = m_has_SIGNAL_0 + m_has_SIGNAL_1 + m_has_SIGNAL_2 + m_has_SIGNAL_3;
            if (is_not_updated() && pChannel == CH_OUT_SIGNAL && mSignalInputCounter > 0) {
                AUDIO_BLOCK_ID mBlock_SIGNAL_0     = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_SIGNAL_1     = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_SIGNAL_2     = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_SIGNAL_3     = AudioBlockPool::NO_ID;
                SIGNAL_TYPE*   mBlockData_SIGNAL_0 = nullptr;
                SIGNAL_TYPE*   mBlockData_SIGNAL_1 = nullptr;
                SIGNAL_TYPE*   mBlockData_SIGNAL_2 = nullptr;
                SIGNAL_TYPE*   mBlockData_SIGNAL_3 = nullptr;

                if (m_has_SIGNAL_0) {
                    mBlock_SIGNAL_0     = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_0 = AudioBlockPool::instance().data(mBlock_SIGNAL_0);
                    mConnection_CH_IN_SIGNAL_0->update(mBlock_SIGNAL_0);
                }
                if (m_has_SIGNAL_1) {
                    mBlock_SIGNAL_1     = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_1 = AudioBlockPool::instance().data(mBlock_SIGNAL_1);
                    mConnection_CH_IN_SIGNAL_1->update(mBlock_SIGNAL_1);
                }
                if (m_has_SIGNAL_2) {
                    mBlock_SIGNAL_2     = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_2 = AudioBlockPool::instance().data(mBlock_SIGNAL_2);
                    mConnection_CH_IN_SIGNAL_2->update(mBlock_SIGNAL_2);
                }
                if (m_has_SIGNAL_3) {
                    mBlock_SIGNAL_3     = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_3 = AudioBlockPool::instance().data(mBlock_SIGNAL_3);
                    mConnection_CH_IN_SIGNAL_3->update(mBlock_SIGNAL_3);
                }

                const float mInverseSigCounter = 1.0 / mSignalInputCounter;
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; ++i) {
                    const float s0  = m_has_SIGNAL_0 ? (mBlockData_SIGNAL_0[i] * mMix[SIGNAL_CHANNEL::SIGNAL_0]) : 0.0;
                    const float s1  = m_has_SIGNAL_1 ? (mBlockData_SIGNAL_1[i] * mMix[SIGNAL_CHANNEL::SIGNAL_1]) : 0.0;
                    const float s2  = m_has_SIGNAL_2 ? (mBlockData_SIGNAL_2[i] * mMix[SIGNAL_CHANNEL::SIGNAL_2]) : 0.0;
                    const float s3  = m_has_SIGNAL_3 ? (mBlockData_SIGNAL_3[i] * mMix[SIGNAL_CHANNEL::SIGNAL_3]) : 0.0;
                    const float sum = s0 + s1 + s2 + s3;
                    pAudioBlock[i]  = sum * mInverseSigCounter;
                }
                AudioBlockPool::instance().release(mBlock_SIGNAL_0);
                AudioBlockPool::instance().release(mBlock_SIGNAL_1);
                AudioBlockPool::instance().release(mBlock_SIGNAL_2);
                AudioBlockPool::instance().release(mBlock_SIGNAL_3);

                flag_updated();
            } else {
                memset(pAudioBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
            }
        }

        void set_mix(SIGNAL_CHANNEL pChannel, float pValue) {
            mMix[pChannel] = pValue;
        }

        SIGNAL_TYPE get_mix(uint8_t pChannel) {
            return mMix[pChannel];
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIX_F32:
                    set_mix(static_cast<SIGNAL_CHANNEL>(pPayLoad[0]), KlangMath::FLOAT_32(pPayLoad, 1));
                    break;
            }
        }

    private:
        Connection* mConnection_CH_IN_SIGNAL_0 = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_1 = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_2 = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_3 = nullptr;

        float mMix[4] = {1, 1, 1, 1};
    };
}  // namespace klang

#endif /* NodeMixer4_hpp */
