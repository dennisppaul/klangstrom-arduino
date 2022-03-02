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
 *       [ NODE_TRIGGER        ]
 *       +---------------------+
 *       |                     |
 *       |             TRIGGER |--OUTNN
 *       |                     |
 *       +---------------------+
 *
 *       @description (
 *           triggers up to 255 connected nodes
 *       )
 *
 */

#ifndef StromNodeTrigger_hpp
#define StromNodeTrigger_hpp

#include "StromNode.hpp"

namespace strom {
    class StromNodeTrigger : public StromNode {
    public:
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_00 = 0x00;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_01 = 0x01;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_02 = 0x02;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_03 = 0x03;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_04 = 0x04;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_05 = 0x05;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_06 = 0x06;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_07 = 0x07;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_08 = 0x08;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_09 = 0x09;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_10 = 0x0A;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_11 = 0x0B;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_12 = 0x0C;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_13 = 0x0D;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_14 = 0x0E;
        static const STROM_CHANNEL_ID CH_OUT_TRIGGER_15 = 0x0F;

        StromNodeTrigger() : StromNode(0, 255) {}

        void in(const STROM_CHANNEL_ID pChannel, const float pData) override {}

        float out(const STROM_CHANNEL_ID pChannel) override {
            return STROM_NO_VALUE;
        }
    };
}  // namespace strom

#endif /* StromNodeTrigger_hpp */
