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
 *       [ NODE_KERNEL_BLOCK   ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          abstract node that allows the easy creation of new signal block processing nodesby
 *          overriding the `kernel` method.
 *       )
 */

#ifndef NodeKernelBlock_hpp
#define NodeKernelBlock_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeKernelBlock : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
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
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                kernel(pAudioBlock);
            }
        }

        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    protected:
        /* override kernel method to implement custom kernels. */
        virtual void kernel(SIGNAL_TYPE* s) = 0;

    private:
        Connection* mConnection_CH_IN_SIGNAL = nullptr;
    };
}  // namespace klang

#endif /* NodeKernelBlock_hpp */
