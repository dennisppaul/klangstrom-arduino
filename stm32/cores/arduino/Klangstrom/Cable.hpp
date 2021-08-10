//
//  Cabel.hpp
//  Strom – a node+text-based library
//
//
//

#ifndef Cabel_hpp
#define Cabel_hpp

#include "StromNode.hpp"

namespace strom {
    class Cable {
    public:
        Cable(StromNode* pOutNode, STROM_CHANNEL_ID pOutChannel, 
              StromNode* pInNode,  STROM_CHANNEL_ID pInChannel);
        virtual ~Cable();
        void patch();
        
        StromNode*              out_node;
        const STROM_CHANNEL_ID  out_channel;
        StromNode*              in_node;
        const STROM_CHANNEL_ID  in_channel;
    };
}

#endif /* Cabel_hpp */
