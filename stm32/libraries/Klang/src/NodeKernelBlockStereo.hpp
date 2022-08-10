/*
 * Klang – a node+text-based synthesizer library
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *       [ NODE_KERNEL_BLOCK_STEREO ]
 *       +--------------------------+
 *       |                          |
 * IN00--| SIGNAL_A        SIGNAL_A |--OUT00
 * IN00--| SIGNAL_B        SIGNAL_B |--OUT01
 *       |                          |
 *       +--------------------------+
 *
 *       @description(
 *          abstract node that allows the easy creation of new signal processing nodes by
 *          overriding the `kernel` method. this kernel node allows to process two input
 *          and output signals.
 *       )
 */

#ifndef NodeKernelBlockStereo_hpp
#define NodeKernelBlockStereo_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeKernelBlockStereo : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL_LEFT   = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_RIGHT  = 1;
        static const CHANNEL_ID NUM_CH_IN           = 2;
        static const CHANNEL_ID CH_OUT_SIGNAL_LEFT  = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_OUT          = 2;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_LEFT) {
                mConnection_CH_IN_SIGNAL_A = pConnection;
                return true;
            } else if (pInChannel == CH_IN_SIGNAL_RIGHT) {
                mConnection_CH_IN_SIGNAL_B = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_LEFT) {
                mConnection_CH_IN_SIGNAL_A = nullptr;
                return true;
            } else if (pInChannel == CH_IN_SIGNAL_RIGHT) {
                mConnection_CH_IN_SIGNAL_B = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (mConnection_CH_IN_SIGNAL_A == nullptr && mConnection_CH_IN_SIGNAL_B == nullptr) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = 0;
                }
            } else {
                if (is_not_updated()) {
                    SIGNAL_TYPE*   mBlockData_OUT_SIGNAL_A = nullptr;
                    SIGNAL_TYPE*   mBlockData_IN_SIGNAL_A  = nullptr;
                    AUDIO_BLOCK_ID mBlock_SIGNAL_IN_A      = AudioBlockPool::NO_ID;
                    if (mConnection_CH_IN_SIGNAL_A != nullptr) {
                        mBlock_SIGNAL_OUT_A     = AudioBlockPool::instance().request();
                        mBlock_SIGNAL_IN_A      = AudioBlockPool::instance().request();
                        mBlockData_OUT_SIGNAL_A = AudioBlockPool::instance().data(mBlock_SIGNAL_OUT_A);
                        mBlockData_IN_SIGNAL_A  = AudioBlockPool::instance().data(mBlock_SIGNAL_IN_A);
                        mConnection_CH_IN_SIGNAL_A->update(mBlock_SIGNAL_IN_A);
                    }

                    SIGNAL_TYPE*   mBlockData_OUT_SIGNAL_B = nullptr;
                    SIGNAL_TYPE*   mBlockData_IN_SIGNAL_B  = nullptr;
                    AUDIO_BLOCK_ID mBlock_SIGNAL_IN_B      = AudioBlockPool::NO_ID;
                    if (mConnection_CH_IN_SIGNAL_B != nullptr) {
                        mBlock_SIGNAL_OUT_B     = AudioBlockPool::instance().request();
                        mBlock_SIGNAL_IN_B      = AudioBlockPool::instance().request();
                        mBlockData_OUT_SIGNAL_B = AudioBlockPool::instance().data(mBlock_SIGNAL_OUT_B);
                        mBlockData_IN_SIGNAL_B  = AudioBlockPool::instance().data(mBlock_SIGNAL_IN_B);
                        mConnection_CH_IN_SIGNAL_B->update(mBlock_SIGNAL_IN_B);
                    }

                    kernel(mBlockData_OUT_SIGNAL_A,
                           mBlockData_OUT_SIGNAL_B,
                           mBlockData_IN_SIGNAL_A,
                           mBlockData_IN_SIGNAL_B);

                    AudioBlockPool::instance().release(mBlock_SIGNAL_IN_A);
                    AudioBlockPool::instance().release(mBlock_SIGNAL_IN_B);

                    flag_updated();
                }
                if (pChannel == CH_OUT_SIGNAL_LEFT && mConnection_CH_IN_SIGNAL_A != nullptr) {
                    SIGNAL_TYPE* mBlockData_OUT_SIGNAL_A = AudioBlockPool::instance().data(mBlock_SIGNAL_OUT_A);
                    KLANG_COPY_AUDIO_BUFFER(pAudioBlock, mBlockData_OUT_SIGNAL_A);
                } else if (pChannel == CH_OUT_SIGNAL_RIGHT && mConnection_CH_IN_SIGNAL_B != nullptr) {
                    SIGNAL_TYPE* mBlockData_OUT_SIGNAL_B = AudioBlockPool::instance().data(mBlock_SIGNAL_OUT_B);
                    KLANG_COPY_AUDIO_BUFFER(pAudioBlock, mBlockData_OUT_SIGNAL_B);
                }
            }
        }

        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad){};

    protected:
        /* override kernel method to implement custom kernels. */
        virtual void kernel(SIGNAL_TYPE* pOutputSignal_LEFT,
                            SIGNAL_TYPE* pOutputSignal_RIGHT,
                            SIGNAL_TYPE* pInputSignal_A,
                            SIGNAL_TYPE* pInputSignal_B) = 0;

    private:
        Connection* mConnection_CH_IN_SIGNAL_A = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_B = nullptr;

        AUDIO_BLOCK_ID mBlock_SIGNAL_OUT_A = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_SIGNAL_OUT_B = AudioBlockPool::NO_ID;
    };
}  // namespace klang

#endif /* NodeKernelBlockStereo_hpp */
