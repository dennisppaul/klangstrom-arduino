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
 *       [ NODE_TRIGGER        ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL              |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeTrigger_hpp
#define NodeTrigger_hpp

#include "KlangEventDistributor.hpp"
#include "KlangNode.hpp"

namespace klang {
    class NodeTrigger : public NodeKernel, public KlangEventDistributor {
    public:
        static constexpr float TRIGGER_FALLING_EDGE = -1.0;
        static constexpr float TRIGGER_RISING_EDGE  = 1.0;

        SIGNAL_TYPE kernel(SIGNAL_TYPE s) {
            if (mPreviousSample > mThreshold && s < mThreshold) {
                fire_event(KLANG_EVENT::TRIGGER, std::vector<float>{TRIGGER_FALLING_EDGE});
            } else if (mPreviousSample < mThreshold && s > mThreshold) {
                fire_event(KLANG_EVENT::TRIGGER, std::vector<float>{TRIGGER_RISING_EDGE});
            }
            mPreviousSample = s;
            return s;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        SIGNAL_TYPE            mPreviousSample = 0.0;
        static constexpr float mThreshold      = 0.0;
    };
}  // namespace klang

#endif /* NodeTrigger_hpp */
