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

#include "Strom.hpp"

using namespace std;
using namespace strom;

// #define DEBUG_STROM

void Strom::update(StromNode& pRoot) {
    /* breadth-first search (BFS) */
    vector<StromNode*> mNodeStructure;
    mNodeStructure.push_back(&pRoot);
    pRoot.traverse(mNodeStructure);
#ifdef DEBUG_STROM
    for (StromNode* mNode : mNodeStructure) {
        STROM_LOG("+++ @Strom::structure NODE_%02i", mNode->ID());
    }
#endif
#define _STROM_REVERSE_STRCUTURE_TRAVERSAL_
#ifndef _STROM_REVERSE_STRCUTURE_TRAVERSAL_
    for (StromNode* mNode : mNodeStructure) {
        STROM_LOG("+++ @Strom::patch NODE_%02i", mNode->ID());
        mNode->patch_cables();
    }
#else
    for (uint32_t i = mNodeStructure.size(); i > 0; i--) {
        StromNode* mNode = mNodeStructure[i - 1];
#ifdef DEBUG_STROM
        STROM_LOG("+++ @Strom::patch(REV) NODE_%02i", mNode->ID());
#endif
        mNode->patch_cables();
    }
#endif
}

bool Strom::connect(StromNode* pOutNode, STROM_CHANNEL_ID pOutChannel,
                    StromNode* pInNode, STROM_CHANNEL_ID pInChannel) {
#ifdef DEBUG_STROM
    STROM_LOG("+++ @Strom::connect NODE_%02i(OUT%02i) > NODE_%02i(IN%02i)", pOutNode->ID(), pOutChannel, pInNode->ID(), pInChannel);
#endif
    return pOutNode->connect(pOutChannel, pInNode, pInChannel);
}

bool Strom::connect(StromNode& pOutNode, STROM_CHANNEL_ID pOutChannel,
                    StromNode& pInNode, STROM_CHANNEL_ID pInChannel) {
#ifdef DEBUG_STROM
    STROM_LOG("+++ @Strom::connect NODE_%02i(OUT%02i) > NODE_%02i(IN%02i)", pOutNode.ID(), pOutChannel, pInNode.ID(), pInChannel);
#endif
    return pOutNode.connect(pOutChannel, &pInNode, pInChannel);
}

bool Strom::connect(StromNode& pOutNode, StromNode& pInNode) {
#ifdef DEBUG_STROM
    STROM_LOG("+++ @Strom::connect NODE_%02i(OUT%02i) > NODE_%02i(IN%02i)", pOutNode.ID(), 0, pInNode.ID(), 0);
#endif
    return pOutNode.connect(0, &pInNode, 0);
}

void Strom::patch(StromNode* pNodeOut, STROM_CHANNEL_ID pChanneOut, StromNode* pNodeIn, STROM_CHANNEL_ID pChannelIn) {
    pNodeIn->in(pChannelIn, pNodeOut->out(pChanneOut));
}
