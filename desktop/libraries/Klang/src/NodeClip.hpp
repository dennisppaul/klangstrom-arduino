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
 *       [ NODE_CLIP           ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeClip_hpp
#define NodeClip_hpp

#include "NodeKernel.hpp"

namespace klang {
    class NodeClip : public NodeKernel {
    public:
        void  set_min(float pClipMin) { mClipMin = pClipMin; }
        void  set_max(float pClipMax) { mClipMax = pClipMax; }
        float get_min() { return mClipMin; }
        float get_max() { return mClipMax; }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIN_F32:
                    set_min(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_MAX_F32:
                    set_max(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    protected:
        SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            return KlangMath::clamp(s, mClipMin, mClipMax);
        }

    private:
        float mClipMin = SIGNAL_MIN;
        float mClipMax = SIGNAL_MAX;
    };
}  // namespace klang

#endif /* NodeClip_hpp */
