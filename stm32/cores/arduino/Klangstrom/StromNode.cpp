//
//  StromNode.cpp
//  Strom – a node+text-based library
//
//
//

#include "StromNode.hpp"

#include <algorithm>

#include "Cable.hpp"

using namespace strom;

STROM_NODE_ID StromNode::oID = 0;

StromNode::StromNode(const STROM_CHANNEL_ID pNumberOfChannelsIn,
                     const STROM_CHANNEL_ID pNumberOfChannelsOut) : mID(oID++),
                                                                    mNumberOfChannelsIn(pNumberOfChannelsIn),
                                                                    mNumberOfChannelsOut(pNumberOfChannelsOut) {
    // STROM_LOG("+++ CREATING NODE_%02i %s", mID, typeid(*this).name());
    mCables = new Cable*[mNumberOfChannelsOut];
    for (uint8_t i = 0; i < mNumberOfChannelsOut; i++) {
        mCables[i] = nullptr;
    }
}

StromNode::~StromNode() {
    for (uint8_t i = 0; i < mNumberOfChannelsOut; i++) {
        delete mCables[i];
    }
    delete[] mCables;
}

STROM_NODE_ID StromNode::ID() {
    return mID;
}

bool StromNode::connect(const STROM_CHANNEL_ID pOutChannel,
                        StromNode*             pInNode,
                        const STROM_CHANNEL_ID pInChannel) {
    if (pOutChannel >= 0 && pOutChannel < mNumberOfChannelsOut) {
        mCables[pOutChannel] = new Cable(this, pOutChannel, pInNode, pInChannel);
        return true;
    } else {
        STROM_LOG_ERR("+++ @StromNode      Node(%02d) connect failed to connect to output channel %02i", ID(), pOutChannel);
        return false;
    }
}

void StromNode::traverse(vector<StromNode*>& pNodeList) {
    for (uint8_t i = mNumberOfChannelsOut; i > 0; i--) {
        const uint8_t j = i - 1;
#define _STROM_REJECT_DUPLICATES_
#ifdef _STROM_REJECT_DUPLICATES_
        if (mCables[j] != nullptr) {
            if (std::find(pNodeList.begin(), pNodeList.end(), mCables[j]->in_node) == pNodeList.end()) {
                pNodeList.push_back(mCables[j]->in_node);
            }
#else
        pNodeList.push_back(mCables[j]->in_node);
#endif
        }
    }
    for (uint8_t i = mNumberOfChannelsOut; i > 0; i--) {
        const uint8_t j = i - 1;
        if (mCables[j] != nullptr) {
            mCables[j]->in_node->traverse(pNodeList);
        }
    }
}

void StromNode::patch_cables() {
    for (uint8_t i = mNumberOfChannelsOut; i > 0; i--) {
        if (mCables[i - 1] != nullptr) {
            mCables[i - 1]->patch();
        }
    }
}
