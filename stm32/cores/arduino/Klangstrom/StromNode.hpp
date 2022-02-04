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

#ifndef StromNode_hpp
#define StromNode_hpp

#include <vector>

#include "Strom.hpp"

using namespace std;

namespace strom {
    class Cable;
    class StromNode {
    public:
        virtual void              in(const STROM_CHANNEL_ID pChannel, const STROM_SIGNAL_TYPE pData) = 0;
        virtual STROM_SIGNAL_TYPE out(const STROM_CHANNEL_ID pChannel)                               = 0;
        STROM_NODE_ID             ID();
        bool                      connect(const STROM_CHANNEL_ID pOutChannel, StromNode* pInNode, const STROM_CHANNEL_ID pInChannel);
        void                      traverse(vector<StromNode*>& pNodeList);
        void                      patch_cables();
        StromNode(const STROM_CHANNEL_ID pNumberOfChannelsIn, const STROM_CHANNEL_ID pNumberOfChannelsOut);
        virtual ~StromNode();

    private:
        const STROM_NODE_ID  mID;
        static STROM_NODE_ID oID;

    protected:
        const STROM_CHANNEL_ID mNumberOfChannelsIn;
        const STROM_CHANNEL_ID mNumberOfChannelsOut;
        Cable**                mCables;
    };
}  // namespace strom

#endif /* StromNode_hpp */
