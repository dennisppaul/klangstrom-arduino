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

// @TODO(this node should go into `NodeClip`)

/**
 *       [ NODE_SOFT_CLIP      ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeSoftClip_hpp
#define NodeSoftClip_hpp

#include "KlangMath.hpp"
#include "NodeKernelBlock.hpp"

namespace klang {
    class NodeSoftClip : public NodeKernelBlock {
    public:
        void set_level_in(SIGNAL_TYPE pValue) {
            mLevel_IN = pValue;
        }

        SIGNAL_TYPE get_level_in() {
            return mLevel_IN;
        }

        void set_level_out(SIGNAL_TYPE pValue) {
            mLevel_OUT = pValue;
        }

        SIGNAL_TYPE get_level_out() {
            return mLevel_OUT;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            // @todo( add `KLANG_SET_LEVEL_IN_F32` + `KLANG_SET_LEVEL_OUT_F32` )
            // switch (pCommand) {
            //     case KLANG_SET_LEVEL_IN_F32:
            //         set_level_in(KlangMath::FLOAT_32(pPayLoad));
            //         break;
            //     case KLANG_SET_LEVEL_OUT_F32:
            //         set_level_out(KlangMath::FLOAT_32(pPayLoad));
            //         break;
            // }
        }

    protected:
        void kernel(SIGNAL_TYPE* s) {
            for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                s[i] = klang_math_tan(s[i] * mLevel_IN) * mLevel_OUT;
            }
        }

    private:
        SIGNAL_TYPE mLevel_IN  = 1.0;
        SIGNAL_TYPE mLevel_OUT = 1.0;
    };
}  // namespace klang

#endif /* NodeSoftClip_hpp */
