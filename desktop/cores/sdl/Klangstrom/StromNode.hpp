//
//  StromNode.hpp
//  Strom – a node+text-based library
//
//
//

#ifndef StromNode_hpp
#define StromNode_hpp

#include <vector>
#include "Strom.hpp"

using namespace std;

namespace strom {
    class Cable;
    class StromNode {
    public:
        StromNode(const STROM_CHANNEL_ID pNumberOfChannelsIn, const STROM_CHANNEL_ID pNumberOfChannelsOut);
        virtual ~StromNode();
        STROM_NODE_ID ID();
        bool connect(const STROM_CHANNEL_ID pOutChannel, StromNode* pInNode, const STROM_CHANNEL_ID pInChannel);
        void traverse(vector<StromNode*>& pNodeList);
        
        virtual void in(const STROM_CHANNEL_ID pChannel, const STROM_SIGNAL_TYPE pData) = 0;
        virtual STROM_SIGNAL_TYPE out(const STROM_CHANNEL_ID pChannel) = 0;
        
        void patch_cables();

    private:
        const  STROM_NODE_ID mID;
        static STROM_NODE_ID oID;

    protected:
        const STROM_CHANNEL_ID mNumberOfChannelsIn;
        const STROM_CHANNEL_ID mNumberOfChannelsOut;
        Cable** mCables;
    };
}

#endif /* StromNode_hpp */
