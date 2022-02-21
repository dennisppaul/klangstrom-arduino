/*
 * Klang – a node+text-based synthesizer library
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

#include "Connection.hpp"

#include "Klang.hpp"

using namespace klang;

CONNECTION_ID Connection::oID = 0;

Connection::Connection(Node& pOutputNode, CHANNEL_ID pOutputChannel,
                       Node& pInputNode, CHANNEL_ID pInputChannel,
                       bool& pStatus) : output_node(pOutputNode),
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

void Connection::reset() {
    oID = 0;
}
