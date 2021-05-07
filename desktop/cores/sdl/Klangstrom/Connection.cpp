//
//  Connection.cpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "Klang.hpp"
#include "Connection.hpp"

using namespace klang;

CONNECTION_ID Connection::oID = 0;

Connection::Connection(Node& pOutputNode, CHANNEL_ID pOutputChannel,
                       Node& pInputNode, CHANNEL_ID pInputChannel,
                       bool& pStatus) :
output_node(pOutputNode),
output_channel_id(pOutputChannel),
input_node(pInputNode),
input_channel_id(pInputChannel),
mID(oID++) {
    pStatus = pInputNode.connect(this, pInputChannel);
}

Connection::~Connection() {
    input_node.disconnect(input_channel_id);
}

void Connection::update(SIGNAL_TYPE* pAudioBlock) {
    output_node.update(output_channel_id, pAudioBlock);
}

void Connection::update(AUDIO_BLOCK_ID pAudioBlockID) {
    output_node.update(output_channel_id, AudioBlockPool::instance().data(pAudioBlockID));
}

void  Connection::reset() {
    oID = 0;
}
