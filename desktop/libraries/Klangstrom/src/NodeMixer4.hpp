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
// @TODO(implement `MIX`)

#ifndef NodeMixer4_hpp
#define NodeMixer4_hpp

#include "Node.hpp"

namespace klang {
    class NodeMixer4 : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL_0      = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_1      = 1;
        static const CHANNEL_ID CH_IN_SIGNAL_2      = 2;
        static const CHANNEL_ID CH_IN_SIGNAL_3      = 3;
        static const CHANNEL_ID NUM_CH_IN           = 4;
        static const CHANNEL_ID NUM_CH_OUT          = 1;
        
        enum SIGNAL_CHANNEL { SIGNAL_0, SIGNAL_1, SIGNAL_2, SIGNAL_3 };
        
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
            AUDIO_BLOCK_ID mBlock_SIGNAL_A  = AudioBlockPool::NO_ID;
            AUDIO_BLOCK_ID mBlock_SIGNAL_B  = AudioBlockPool::NO_ID;
            AUDIO_BLOCK_ID mBlock_SIGNAL_C  = AudioBlockPool::NO_ID;
            AUDIO_BLOCK_ID mBlock_SIGNAL_D  = AudioBlockPool::NO_ID;
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL_0 != nullptr) {
                    mBlock_SIGNAL_A = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL_0->update(mBlock_SIGNAL_A);
                }
                if (mConnection_CH_IN_SIGNAL_1 != nullptr) {
                    mBlock_SIGNAL_B = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL_1->update(mBlock_SIGNAL_B);
                }
                if (mConnection_CH_IN_SIGNAL_2 != nullptr) {
                    mBlock_SIGNAL_C = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL_2->update(mBlock_SIGNAL_C);
                }
                if (mConnection_CH_IN_SIGNAL_3 != nullptr) {
                    mBlock_SIGNAL_D = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL_3->update(mBlock_SIGNAL_D);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                SIGNAL_TYPE* mBlockData_SIGNAL_A = AudioBlockPool::instance().data(mBlock_SIGNAL_A);
                SIGNAL_TYPE* mBlockData_SIGNAL_B = AudioBlockPool::instance().data(mBlock_SIGNAL_B);
                SIGNAL_TYPE* mBlockData_SIGNAL_C = AudioBlockPool::instance().data(mBlock_SIGNAL_C);
                SIGNAL_TYPE* mBlockData_SIGNAL_D = AudioBlockPool::instance().data(mBlock_SIGNAL_D);
                const uint8_t mSignalInputCounter =
                (mBlockData_SIGNAL_A==nullptr ? 0 : 1) +
                (mBlockData_SIGNAL_B==nullptr ? 0 : 1) +
                (mBlockData_SIGNAL_C==nullptr ? 0 : 1) +
                (mBlockData_SIGNAL_D==nullptr ? 0 : 1);
                const float mRatio = 1.0 / mSignalInputCounter;
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] =
                    ((mBlockData_SIGNAL_A!=nullptr ? mBlockData_SIGNAL_A[i] : 0.0) * mMix[SIGNAL_CHANNEL::SIGNAL_0] +
                     (mBlockData_SIGNAL_B!=nullptr ? mBlockData_SIGNAL_B[i] : 0.0) * mMix[SIGNAL_CHANNEL::SIGNAL_1] +
                     (mBlockData_SIGNAL_C!=nullptr ? mBlockData_SIGNAL_C[i] : 0.0) * mMix[SIGNAL_CHANNEL::SIGNAL_2] +
                     (mBlockData_SIGNAL_D!=nullptr ? mBlockData_SIGNAL_D[i] : 0.0) * mMix[SIGNAL_CHANNEL::SIGNAL_3]
                     ) * mRatio;
                }
            }
        }
        
        void set_mix(SIGNAL_CHANNEL pChannel, float pValue) {
            mMix[pChannel] = pValue;
        }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIX_F32:
                    set_mix(static_cast<SIGNAL_CHANNEL>(pPayLoad[0]), KlangMath::FLOAT_32(pPayLoad, 1));
                    break;
            }
        }
        
    private:
        Connection* mConnection_CH_IN_SIGNAL_0  = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_1  = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_2  = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_3  = nullptr;
        
        float mMix[4] = {0.5, 0.5, 0.5, 0.5};
    };
}

#endif /* NodeMixer4_hpp */
