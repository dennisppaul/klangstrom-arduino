//
//  Strom.cpp
//  Strom – a node+text-based library
//
//
//

#include "Strom.hpp"

using namespace std;
using namespace strom;

// #define DEBUG_STROM

void Strom::update_structure(StromNode& pRoot) {
    /* breadth-first search (BFS) */
    vector<StromNode*> mNodeStructure;
    mNodeStructure.push_back(&pRoot);
    pRoot.traverse(mNodeStructure);
    for (StromNode* mNode : mNodeStructure) {
#ifdef DEBUG_STROM
        STROM_LOG("+++ @Strom::structure NODE_%02i", mNode->ID());
#endif
    }
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
