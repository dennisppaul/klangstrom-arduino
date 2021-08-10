//
//  Strom.hpp
//  Strom – a node+text-based library
//
//
//

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

using namespace std;

namespace strom {
    class StromNode;
    class Strom {
    public:
        static void update_structure(StromNode& pRoot);
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
