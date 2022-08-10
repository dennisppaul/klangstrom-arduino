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
 *       [ NODE_KERNEL_BLOCK_2 ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL_A     SIGNAL |--OUT00
 * IN00--| SIGNAL_B            |
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          abstract node that allows the easy creation of new signal processing nodes by
 *          overriding the `kernel` method. this kernel node allows to process two input
 *          signals.
 *       )
 */

#ifndef NodeKernelBlock2_hpp
#define NodeKernelBlock2_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeKernelBlock2 : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL_A = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_B = 1;
        static const CHANNEL_ID NUM_CH_IN      = 2;
        static const CHANNEL_ID NUM_CH_OUT     = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_A) {
                mConnection_CH_IN_SIGNAL_A = pConnection;
                return true;
            } else if (pInChannel == CH_IN_SIGNAL_B) {
                mConnection_CH_IN_SIGNAL_B = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_A) {
                mConnection_CH_IN_SIGNAL_A = nullptr;
                return true;
            } else if (pInChannel == CH_IN_SIGNAL_B) {
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
                if (is_not_updated() && pChannel == CH_OUT_SIGNAL) {
                    SIGNAL_TYPE*   mBlockData_IN_SIGNAL_A = nullptr;
                    AUDIO_BLOCK_ID mBlock_SIGNAL_IN_A     = AudioBlockPool::NO_ID;
                    if (mConnection_CH_IN_SIGNAL_A != nullptr) {
                        mBlock_SIGNAL_IN_A     = AudioBlockPool::instance().request();
                        mBlockData_IN_SIGNAL_A = AudioBlockPool::instance().data(mBlock_SIGNAL_IN_A);
                        mConnection_CH_IN_SIGNAL_A->update(mBlock_SIGNAL_IN_A);
                    }

                    SIGNAL_TYPE*   mBlockData_IN_SIGNAL_B = nullptr;
                    AUDIO_BLOCK_ID mBlock_SIGNAL_IN_B     = AudioBlockPool::NO_ID;
                    if (mConnection_CH_IN_SIGNAL_B != nullptr) {
                        mBlock_SIGNAL_IN_B     = AudioBlockPool::instance().request();
                        mBlockData_IN_SIGNAL_B = AudioBlockPool::instance().data(mBlock_SIGNAL_IN_B);
                        mConnection_CH_IN_SIGNAL_B->update(mBlock_SIGNAL_IN_B);
                    }

                    kernel(pAudioBlock,
                           mBlockData_IN_SIGNAL_A,
                           mBlockData_IN_SIGNAL_B);

                    AudioBlockPool::instance().release(mBlock_SIGNAL_IN_A);
                    AudioBlockPool::instance().release(mBlock_SIGNAL_IN_B);

                    flag_updated();
                }
            }
        }

        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad){};

    protected:
        /* override kernel method to implement custom kernels. */
        virtual void kernel(SIGNAL_TYPE* pOutputSignal,
                            SIGNAL_TYPE* pInputSignal_A,
                            SIGNAL_TYPE* pInputSignal_B) = 0;

    private:
        Connection* mConnection_CH_IN_SIGNAL_A = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_B = nullptr;
    };
}  // namespace klang

#endif /* NodeKernelBlock2_hpp */
