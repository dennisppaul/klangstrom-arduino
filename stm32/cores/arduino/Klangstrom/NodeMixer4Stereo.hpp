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
// @TODO(implement `PAN`, `MIX` as INPUTs)

#ifndef NodeMixer4Stereo_hpp
#define NodeMixer4Stereo_hpp

#if KLST_BOARD_TYPE==KLST_TINY
// #define KLST_USE_DSP
#endif

#ifdef KLST_USE_DSP
// see https://arm-software.github.io/CMSIS_5/DSP/html/index.html
#include <CMSIS_DSP.h>
#endif

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
        
        enum SIGNAL_CHANNEL { SIGNAL_0 = 0, SIGNAL_1, SIGNAL_2, SIGNAL_3 };

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
            const bool b0 = (mConnection_CH_IN_SIGNAL_0!=nullptr);
            const bool b1 = (mConnection_CH_IN_SIGNAL_1!=nullptr);
            const bool b2 = (mConnection_CH_IN_SIGNAL_2!=nullptr);
            const bool b3 = (mConnection_CH_IN_SIGNAL_3!=nullptr);
            const uint8_t mSignalInputCounter = b0 + b1 + b2 + b3;
            if (is_not_updated()
                && pChannel == CH_OUT_SIGNAL
                && mSignalInputCounter > 0) {
                AUDIO_BLOCK_ID mBlock_SIGNAL_0  = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_SIGNAL_1  = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_SIGNAL_2  = AudioBlockPool::NO_ID;
                AUDIO_BLOCK_ID mBlock_SIGNAL_3  = AudioBlockPool::NO_ID;
                SIGNAL_TYPE* mBlockData_SIGNAL_0 = nullptr;
                SIGNAL_TYPE* mBlockData_SIGNAL_1 = nullptr;
                SIGNAL_TYPE* mBlockData_SIGNAL_2 = nullptr;
                SIGNAL_TYPE* mBlockData_SIGNAL_3 = nullptr;
                
                if (b0) {
                    mBlock_SIGNAL_0 = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_0 = AudioBlockPool::instance().data(mBlock_SIGNAL_0);
                    mConnection_CH_IN_SIGNAL_0->update(mBlock_SIGNAL_0);
                }
                if (b1) {
                    mBlock_SIGNAL_1 = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_1 = AudioBlockPool::instance().data(mBlock_SIGNAL_1);
                    mConnection_CH_IN_SIGNAL_1->update(mBlock_SIGNAL_1);
                }
                if (b2) {
                    mBlock_SIGNAL_2 = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_2 = AudioBlockPool::instance().data(mBlock_SIGNAL_2);
                    mConnection_CH_IN_SIGNAL_2->update(mBlock_SIGNAL_2);
                }
                if (b3) {
                    mBlock_SIGNAL_3 = AudioBlockPool::instance().request();
                    mBlockData_SIGNAL_3 = AudioBlockPool::instance().data(mBlock_SIGNAL_3);
                    mConnection_CH_IN_SIGNAL_3->update(mBlock_SIGNAL_3);
                }

                mBlock_CH_OUT_SIGNAL_LEFT = AudioBlockPool::instance().request();
                mBlock_CH_OUT_SIGNAL_RIGHT = AudioBlockPool::instance().request();
                SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_LEFT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_LEFT);
                SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_RIGHT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_RIGHT);

                // @todo(this should range from [-1.0...1.0] and not from [0.0...1.0])
                const float mPan0 = mPan[SIGNAL_CHANNEL::SIGNAL_0] * 0.5 + 0.5;
                const float mPan1 = mPan[SIGNAL_CHANNEL::SIGNAL_1] * 0.5 + 0.5;
                const float mPan2 = mPan[SIGNAL_CHANNEL::SIGNAL_2] * 0.5 + 0.5;
                const float mPan3 = mPan[SIGNAL_CHANNEL::SIGNAL_3] * 0.5 + 0.5;
                const float pR0 = mPan0;
                const float pL0 = 1.0 - mPan0;
                const float pR1 = mPan1;
                const float pL1 = 1.0 - mPan1;
                const float pR2 = mPan2;
                const float pL2 = 1.0 - mPan2;
                const float pR3 = mPan3;
                const float pL3 = 1.0 - mPan3;

                const float mRatio = 1.0 / mSignalInputCounter;
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; ++i) {
                    float sL = 0.0;
                    float sR = 0.0;
                    if (b0) {
                        const float s0 = mBlockData_SIGNAL_0[i] * mMix[SIGNAL_CHANNEL::SIGNAL_0];
                        sL   += s0 * pL0;
                        sR   += s0 * pR0;
                    }
                    if (b1) {
                        const float s1 = mBlockData_SIGNAL_1[i] * mMix[SIGNAL_CHANNEL::SIGNAL_1];
                        sL   += s1 * pL1;
                        sR   += s1 * pR1;
                    }
                    if (b2) {
                        const float s2 = mBlockData_SIGNAL_2[i] * mMix[SIGNAL_CHANNEL::SIGNAL_2];
                        sL   += s2 * pL2;
                        sR   += s2 * pR2;
                    }
                    if (b3) {
                        const float s3 = mBlockData_SIGNAL_3[i] * mMix[SIGNAL_CHANNEL::SIGNAL_3];
                        sL   += s3 * pL3;
                        sR   += s3 * pR3;
                    }
                    mBlockData_CH_OUT_SIGNAL_LEFT[i]    = sL;
                    mBlockData_CH_OUT_SIGNAL_RIGHT[i]   = sR;

#ifndef KLST_USE_DSP
                    mBlockData_CH_OUT_SIGNAL_LEFT[i]   *= mRatio;
                    mBlockData_CH_OUT_SIGNAL_RIGHT[i]  *= mRatio;
#endif
                }
                
#ifdef KLST_USE_DSP
//                 arm_offset_f32	(mBlockData_CH_OUT_SIGNAL_LEFT, [[add value]], mBlockData_CH_OUT_SIGNAL_LEFT, KLANG_SAMPLES_PER_AUDIO_BLOCK);
                arm_scale_f32(mBlockData_CH_OUT_SIGNAL_LEFT, mRatio, mBlockData_CH_OUT_SIGNAL_LEFT, KLANG_SAMPLES_PER_AUDIO_BLOCK);	
                arm_scale_f32(mBlockData_CH_OUT_SIGNAL_RIGHT, mRatio, mBlockData_CH_OUT_SIGNAL_RIGHT, KLANG_SAMPLES_PER_AUDIO_BLOCK);	
#endif                
                AudioBlockPool::instance().release(mBlock_SIGNAL_0);
                AudioBlockPool::instance().release(mBlock_SIGNAL_1);
                AudioBlockPool::instance().release(mBlock_SIGNAL_2);
                AudioBlockPool::instance().release(mBlock_SIGNAL_3);

                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL_LEFT) {
                if (mBlock_CH_OUT_SIGNAL_LEFT != AudioBlockPool::NO_ID) {
                    SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_LEFT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_LEFT);
#ifdef KLST_USE_DSP
                    arm_copy_f32(mBlockData_CH_OUT_SIGNAL_LEFT,pAudioBlock,KLANG_SAMPLES_PER_AUDIO_BLOCK);
#else
                    memcpy( pAudioBlock,
                            mBlockData_CH_OUT_SIGNAL_LEFT,
                            sizeof(SIGNAL_TYPE) * KLANG_SAMPLES_PER_AUDIO_BLOCK);
#endif
                }
            } else if (pChannel == CH_OUT_SIGNAL_RIGHT) {
                if (mBlock_CH_OUT_SIGNAL_RIGHT != AudioBlockPool::NO_ID) {
                    SIGNAL_TYPE* mBlockData_CH_OUT_SIGNAL_RIGHT = AudioBlockPool::instance().data(mBlock_CH_OUT_SIGNAL_RIGHT);
#ifdef KLST_USE_DSP
                    arm_copy_f32(mBlockData_CH_OUT_SIGNAL_LEFT,pAudioBlock,KLANG_SAMPLES_PER_AUDIO_BLOCK);
#else
                    memcpy( pAudioBlock,
                            mBlockData_CH_OUT_SIGNAL_RIGHT,
                            sizeof(SIGNAL_TYPE) * KLANG_SAMPLES_PER_AUDIO_BLOCK);
#endif
                }
            }
        }
        
        void set_mix(SIGNAL_CHANNEL pChannel, SIGNAL_TYPE pValue) {
            mMix[pChannel] = pValue;
        }

        SIGNAL_TYPE get_mix(uint8_t pChannel) {
            return mMix[pChannel];
        }
                
        void set_pan(SIGNAL_CHANNEL pChannel, SIGNAL_TYPE pValue) {
            mPan[pChannel] = pValue;
        }
        
        SIGNAL_TYPE get_pan(uint8_t pChannel) {
            return mPan[pChannel];
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
        
        Connection* mConnection_CH_IN_SIGNAL_0  = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_1  = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_2  = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_3  = nullptr;
        static const uint8_t NUM_SIGNAL_INPUT_CHANNELS = 4;
        SIGNAL_TYPE mMix[NUM_SIGNAL_INPUT_CHANNELS] = {1.0, 1.0, 1.0, 1.0};
        SIGNAL_TYPE mPan[NUM_SIGNAL_INPUT_CHANNELS] = {0.0, 0.0, 0.0, 0.0};
    };
}

#endif /* NodeMixer4Stereo_hpp */
