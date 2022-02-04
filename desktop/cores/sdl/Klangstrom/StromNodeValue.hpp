/*
 * Strom – a node+text-based composition library
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

/*
 *
 *       [ NODE_VALUE          ]
 *       +---------------------+
 *       |                     |
 * IN00--| TRIGGER       VALUE |--OUT00
 *       |                     |
 *       +---------------------+
 *
 *       @description (
 *           receives a trigger and passes on a single value.
 *       )
 */

#ifndef StromNodeValue_hpp
#define StromNodeValue_hpp

#include "StromNode.hpp"

namespace strom {
    class StromNodeValue : public StromNode {
    public:
        static const STROM_CHANNEL_ID CH_IN_TRIGGER = 0;
        static const STROM_CHANNEL_ID CH_OUT_VALUE  = 0;

        StromNodeValue() : StromNode(1, 1) {}

        void in(const STROM_CHANNEL_ID pChannel, const float pData) override {
            if (pChannel == CH_IN_TRIGGER) {
                STROM_LOG("+++ @StromNodeValue Node(%02d) receives trigger : %f", ID(), pData);
                if (pData != STROM_NO_VALUE) {
                    // @TODO(@research, "check if this is desired behavior … maybe the value should not be changed here?!?")
                    mValue = pData;
                }
            }
        }

        float out(const STROM_CHANNEL_ID pChannel) override {
            if (pChannel == CH_OUT_VALUE) {
                return mValue;
            } else {
                return STROM_NO_VALUE;
            }
        }

        void set_value(float pValue) {
            mValue = pValue;
        }

        float get_value() {
            return mValue;
        }

    private:
        float mValue = 0.0;
    };
}  // namespace strom

#endif /* StromNodeValue_hpp */
