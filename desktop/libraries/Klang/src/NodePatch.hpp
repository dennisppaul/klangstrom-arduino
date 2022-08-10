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
 *       [ NODE_PATCH          ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

// @TODO implement `NodePatchStereo` and `NodePatchMulti` for multiple in- and outputs

/**
 *       [ NODE_PATCH_STEREO   ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| SIGNAL       SIGNAL |--OUT01
 *       |                     |
 *       +---------------------+
 */

/**
 *       [ NODE_PATCH_MULTI    ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |  ...          ...   |
 * INFF--| SIGNAL       SIGNAL |--OUTFF
 *       |                     |
 *       +---------------------+
 */

#ifndef NodePatch_hpp
#define NodePatch_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodePatch : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                return input().connect(pConnection, pInChannel);
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                return input().disconnect(pInChannel);
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                output().update(Node::CH_IN_SIGNAL, pAudioBlock);
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
            }
        }

        NodePassthrough& output() {
            return mOutput;
        }

        NodePassthrough& input() {
            return mInput;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        NodePassthrough mInput;
        NodePassthrough mOutput;
    };
}  // namespace klang

#endif /* NodePatch_hpp */
