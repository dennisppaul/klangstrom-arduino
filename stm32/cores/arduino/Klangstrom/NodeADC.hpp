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
 *       [ NODE_ADC            ]
 *       +---------------------+
 *       |                     |
 *       |       SIGNAL(_LEFT) |--OUT00
 *       |       SIGNAL_RIGHT  |--OUT01
 *       |                     |
 *       +---------------------+
 *
 */

#ifndef NodeADC_hpp
#define NodeADC_hpp

#include <algorithm>

#include "KlangNode.hpp"

namespace klang {
    class NodeADC : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN           = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL       = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_LEFT  = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_OUT          = 2;

        bool connect(Connection *pConnection, CHANNEL_ID pInChannel) {
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            return false;
        }

        void process_frame(SIGNAL_TYPE *pLeft, SIGNAL_TYPE *pRight) {
            std::copy(pLeft, pLeft + KLANG_SAMPLES_PER_AUDIO_BLOCK, mBufferLeft);
            std::copy(pRight, pRight + KLANG_SAMPLES_PER_AUDIO_BLOCK, mBufferRight);
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE *pAudioBlock) {
            // @TODO use `std::copy` function
            if (pChannel == CH_OUT_SIGNAL_LEFT) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBufferLeft[i];
                }
            }
            if (pChannel == CH_OUT_SIGNAL_RIGHT) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBufferRight[i];
                }
            }
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE *pPayLoad) {}

    private:
        SIGNAL_TYPE mBufferLeft[KLANG_SAMPLES_PER_AUDIO_BLOCK];
        SIGNAL_TYPE mBufferRight[KLANG_SAMPLES_PER_AUDIO_BLOCK];
    };
}  // namespace klang

#endif /* NodeADC_hpp */
