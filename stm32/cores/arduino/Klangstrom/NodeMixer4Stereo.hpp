//
//  NodeMixer4Stereo.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_MIXER4_STEREO  ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL_0   SIGNAL_R |--OUT00
 * IN01--| SIGNAL_1   SIGNAL_L |--OUT01
 * IN02--| SIGNAL_2            |
 * IN03--| SIGNAL_3            |
 *       |                     |
 *       +---------------------+
 */
// @TODO(implement `PAN`, `MIX`)

#ifndef NodeMixer4Stereo_hpp
#define NodeMixer4Stereo_hpp

#include "Node.hpp"

namespace klang {
    class NodeMixer4Stereo : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL_0      = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_1      = 1;
        static const CHANNEL_ID CH_IN_SIGNAL_2      = 2;
        static const CHANNEL_ID CH_IN_SIGNAL_3      = 3;
        static const CHANNEL_ID NUM_CH_IN           = 4;
        static const CHANNEL_ID CH_OUT_SIGNAL_LEFT  = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_OUT          = 2;
        
        enum SIGNAL_CHANNEL { SIGNAL_0, SIGNAL_1, SIGNAL_2, SIGNAL_3 };

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel >= CH_IN_SIGNAL_0 && pInChannel <= CH_IN_SIGNAL_3) {
                mConnection_CH_IN_SIGNAL[pInChannel] = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel >= CH_IN_SIGNAL_0 && pInChannel <= CH_IN_SIGNAL_3) {
                mConnection_CH_IN_SIGNAL[pInChannel] = nullptr;
                return true;
            }
            return false;
        }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                AUDIO_BLOCK_ID mBlock_SIGNAL[4] = { AudioBlockPool::NO_ID, AudioBlockPool::NO_ID, AudioBlockPool::NO_ID, AudioBlockPool::NO_ID };
                SIGNAL_TYPE* mBlockData_SIGNAL[4];
                uint8_t mSignalInputCounter = 0;
                for (uint8_t i=0; i < 4; i++) {
                    if (mConnection_CH_IN_SIGNAL[i] != nullptr) {
                        mBlock_SIGNAL[i] = AudioBlockPool::instance().request();
                        mConnection_CH_IN_SIGNAL[i]->update(mBlock_SIGNAL[i]);
                    }
                    mBlockData_SIGNAL[i] = AudioBlockPool::instance().data(mBlock_SIGNAL[i]);
                    mSignalInputCounter += mBlockData_SIGNAL[i]==nullptr ? 0 : 1;
                }
                const float mRatio = 1.0 / mSignalInputCounter;
                
                mBlock_CH_OUT_SIGNAL_LEFT = AudioBlockPool::instance().request();
                mBlock_CH_OUT_SIGNAL_RIGHT = AudioBlockPool::instance().request();
                SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_LEFT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_LEFT);
                SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_RIGHT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_RIGHT);
                for (uint16_t k=0; k < KLANG_SAMPLES_PER_AUDIO_BLOCK; k++) {
                    float sL = 0.0;
                    float sR = 0.0;
                    for (uint8_t i=0; i < 4; i++) {
                        const float s = (mBlockData_SIGNAL[i]!=nullptr) ? (mBlockData_SIGNAL[i][k] * mMix[i]) : 0.0;
                        sL += s * mPan[i];
                        sR += s * (1.0 - mPan[i]);
                    }
                    mBlockData_CH_OUT_SIGNAL_LEFT[k] = sL * mRatio;
                    mBlockData_CH_OUT_SIGNAL_RIGHT[k] = sR * mRatio;
                }
                for (uint8_t i=0; i < 4; i++) {
                    if (mConnection_CH_IN_SIGNAL[i] != nullptr) {
                        AudioBlockPool::instance().release(mBlock_SIGNAL[i]);
                    }
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL_LEFT) {
                SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_LEFT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_LEFT);
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBlockData_CH_OUT_SIGNAL_LEFT[i];
                }
            } else if (pChannel == CH_OUT_SIGNAL_RIGHT) {
                SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_RIGHT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_RIGHT);
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBlockData_CH_OUT_SIGNAL_RIGHT[i];
                }
            }
        }
        
        void set_mix(SIGNAL_CHANNEL pChannel, SIGNAL_TYPE pValue) {
            mMix[pChannel] = pValue;
        }
        
        void set_pan(SIGNAL_CHANNEL pChannel, SIGNAL_TYPE pValue) {
            mPan[pChannel] = pValue;
        }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIX_F32:
                    set_mix(static_cast<SIGNAL_CHANNEL>(pPayLoad[0]), KlangMath::FLOAT_32(pPayLoad, 1));
                    break;
                case KLANG_SET_PAN_I8_F32:
                    set_pan(static_cast<SIGNAL_CHANNEL>(pPayLoad[0]), KlangMath::FLOAT_32(pPayLoad, 1));
                    break;
            }
        }
        
    private:
        AUDIO_BLOCK_ID mBlock_CH_OUT_SIGNAL_LEFT    = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_CH_OUT_SIGNAL_RIGHT   = AudioBlockPool::NO_ID;
        
        Connection* mConnection_CH_IN_SIGNAL[4] = {nullptr, nullptr, nullptr, nullptr};
        
        SIGNAL_TYPE mMix[4] = {1.0, 1.0, 1.0, 1.0};
        SIGNAL_TYPE mPan[4] = {0.5, 0.5, 0.5, 0.5};
    };
}

#endif /* NodeMixer4Stereo_hpp */