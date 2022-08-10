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

#ifndef Strom_hpp
#define Strom_hpp

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#if __has_include("StromConfiguration.h")
#include "StromConfiguration.h"
#endif

#include "StromDefines.hpp"
#include "StromNode.hpp"

namespace strom {
    class StromNode;
    class Strom {
    public:
        static void update(StromNode& pRoot);
        static bool connect(StromNode* pOutNode, STROM_CHANNEL_ID pOutChannel,
                            StromNode* pInNode, STROM_CHANNEL_ID pInChannel);
        static bool connect(StromNode& pOutNode, STROM_CHANNEL_ID pOutChannel,
                            StromNode& pInNode, STROM_CHANNEL_ID pInChannel);
        static bool connect(StromNode& pOutNode, StromNode& pInNode);

        static void patch(StromNode* pNodeOut, STROM_CHANNEL_ID pChanneOut,
                          StromNode* pNodeIn, STROM_CHANNEL_ID pChannelIn);
    };
}  // namespace strom

#endif /* Strom_hpp */
