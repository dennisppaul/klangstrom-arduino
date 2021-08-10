//
//  Node.cpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Node.hpp"

using namespace klang;

NODE_ID Node::oID = 0;

Node::Node() : mID(oID++) {
    //    KLANG_LOG("+++ CREATING NODE_%02i %s", mID, typeid(*this).name());
}

void Node::reset() {
    oID = 0;
}
