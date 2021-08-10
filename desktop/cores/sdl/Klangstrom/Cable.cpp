//
//  Cabel.cpp
//  Strom – a node+text-based library
//
//
//

#include "Cable.hpp"

using namespace strom;

Cable::Cable(StromNode*       pOutNode,
             STROM_CHANNEL_ID pOutChannel,
             StromNode*       pInNode,
             STROM_CHANNEL_ID pInChannel) : out_node(pOutNode),
                                            out_channel(pOutChannel),
                                            in_node(pInNode),
                                            in_channel(pInChannel) {}

Cable::~Cable() {}

void Cable::patch() {
    // STROM_LOG("+++ @Cable Node(%02d#%02d) > Node(%02d#%02d)          : %f", out_node->ID(), out_channel, in_node->ID(), in_channel, out_node->out(out_channel));
    in_node->in(in_channel, out_node->out(out_channel));
}
