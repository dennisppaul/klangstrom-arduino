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
 * IN00--| SIGNAL_LEFT  SIGNAL |--OUT00
 * IN01--| SIGNAL_RIGHT        |
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
        static const CHANNEL_ID CH_IN_SIGNAL_LEFT   = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_RIGHT  = 1;
        static const CHANNEL_ID CH_IN_MIX           = 2;
        static const CHANNEL_ID NUM_CH_IN           = 3;
        static const CHANNEL_ID NUM_CH_OUT          = 1;
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_LEFT) {
                mConnection_CH_IN_SIGNAL_LEFT = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_RIGHT) {
                mConnection_CH_IN_SIGNAL_RIGHT = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_MIX) {
                mConnection_CH_IN_MIX = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_LEFT) {
                mConnection_CH_IN_SIGNAL_LEFT = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_RIGHT) {
                mConnection_CH_IN_SIGNAL_RIGHT = nullptr;
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
            // @TODO("this needs to be optimized. only audio block ( i.e `mBlock_MIX` ) is necessary …")
            if (is_not_updated()) {
                mBlock_SIGNAL_LEFT = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_SIGNAL_LEFT != nullptr) {
                    mBlock_SIGNAL_LEFT = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL_LEFT->update(mBlock_SIGNAL_LEFT);
                }
                mBlock_SIGNAL_RIGHT = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_SIGNAL_RIGHT != nullptr) {
                    mBlock_SIGNAL_RIGHT = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL_RIGHT->update(mBlock_SIGNAL_RIGHT);
                }
                mBlock_MIX = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_MIX != nullptr) {
                    mBlock_MIX = AudioBlockPool::instance().request();
                    mConnection_CH_IN_MIX->update(mBlock_MIX);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                if (mBlock_SIGNAL_LEFT == AudioBlockPool::NO_ID || mBlock_SIGNAL_RIGHT != AudioBlockPool::NO_ID) {
                    for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        pAudioBlock[i] = 0.0;
                    }
                }
                SIGNAL_TYPE* mBlockData_LEFT = AudioBlockPool::instance().data(mBlock_SIGNAL_LEFT);
                SIGNAL_TYPE* mBlockData_RIGHT = AudioBlockPool::instance().data(mBlock_SIGNAL_RIGHT);
                if (mBlock_MIX == AudioBlockPool::NO_ID) {
                    const float mLeftRatio = ( 1.0 - mMix ) * 0.5;
                    const float mRightRatio = mMix * 0.5;
                    for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        pAudioBlock[i] = mBlockData_LEFT[i] * mLeftRatio + mBlockData_RIGHT[i] * mRightRatio;
                    }
                } else {
                    SIGNAL_TYPE* mBlockData_MIX = AudioBlockPool::instance().data(mBlock_MIX);
                    for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        const float mLeftRatio = ( 1.0 - mBlockData_MIX[i] ) * 0.5;
                        const float mRightRatio = mBlockData_MIX[i] * 0.5;
                        pAudioBlock[i] = mBlockData_LEFT[i] * mLeftRatio + mBlockData_RIGHT[i] * mRightRatio;
                    }
                }
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
        AUDIO_BLOCK_ID mBlock_SIGNAL_LEFT   = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_SIGNAL_RIGHT  = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_MIX           = AudioBlockPool::NO_ID;
        
        Connection* mConnection_CH_IN_SIGNAL_LEFT   = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_RIGHT  = nullptr;
        Connection* mConnection_CH_IN_MIX           = nullptr;
        
        SIGNAL_TYPE mMix = 0.5;
    };
}

#endif /* NodeMixer2_hpp */
