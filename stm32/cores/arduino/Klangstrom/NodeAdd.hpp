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
 *       [ NODE_ADD            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeAdd_hpp
#define NodeAdd_hpp

#include "KlangMath.hpp"
#include "NodeKernel.hpp"

namespace klang {
    class NodeAdd : public NodeKernel {
    public:
        void set_value(SIGNAL_TYPE pValue) {
            mValue = pValue;
        }

        SIGNAL_TYPE get_value() {
            return mValue;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_VALUE_F32:
                    set_value(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    protected:
        SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            return s + mValue;
        }

    private:
        SIGNAL_TYPE mValue = 0.0;
    };
}  // namespace klang

#endif /* NodeAdd_hpp */
