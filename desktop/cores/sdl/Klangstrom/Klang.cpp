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

#include "Klang.hpp"

#include "AudioBlockPool.hpp"
#include "Connection.hpp"
#include "ConnectionPool.hpp"
#include "KlangNode.hpp"

using namespace klang;

CONNECTION_ID Klang::connect(Node &     pOutputNode,
                             CHANNEL_ID pOutputChannel,
                             Node &     pInputNode,
                             CHANNEL_ID pInputChannel) {
    return ConnectionPool::instance().connect(pOutputNode,
                                              pOutputChannel,
                                              pInputNode,
                                              pInputChannel);
}

CONNECTION_ID Klang::connect(Node &pOutputNode, Node &pInputNode) {
    return ConnectionPool::instance().connect(pOutputNode,
                                              Node::CH_OUT_SIGNAL,
                                              pInputNode,
                                              Node::CH_IN_SIGNAL);
}

bool Klang::disconnect(Node &     pOutputNode,
                       CHANNEL_ID pOutputChannel,
                       Node &     pInputNode,
                       CHANNEL_ID pInputChannel) {
    return ConnectionPool::instance().disconnect(pOutputNode,
                                                 Node::CH_OUT_SIGNAL,
                                                 pInputNode,
                                                 Node::CH_IN_SIGNAL);
}

bool Klang::disconnect(CONNECTION_ID pConnectionID) {
    return ConnectionPool::instance().disconnect(pConnectionID);
}

bool Klang::frame_begin() {
#ifdef DEBUG_SYNTHESIZER
    if (mFirstFrame) {
        KLANG_LOG("++++++++++++++++++++++++++++++++++++++++++++++++++");
        KLANG_LOG("+++ @Klang                                     +++");
        KLANG_LOG("+++ rendering first frame                      +++");
    }
#endif
    AudioBlockPool::instance().reset();
    mFrameCounter++;
    return !mLock;
}

void Klang::frame_end() {
    //            if (AudioBlockPool::instance().blocks_available() != KLANG_AUDIO_BLOCKS) {
    //                KLANG_LOG_ERR("### @Klang not all audioblocks were released");
    //            }
#ifdef DEBUG_SYNTHESIZER
    if (mFirstFrame) {
        KLANG_LOG("+++ blocks used per frame: %03i                 +++", AudioBlockPool::instance().blocks_used_max());
        KLANG_LOG("++++++++++++++++++++++++++++++++++++++++++++++++++");
        mFirstFrame = false;
    }
#endif
    //            if (mLock) {
    //                return;
    //            }
}

FRAME_TYPE Klang::frame_index() {
    return mFrameCounter;
}

void Klang::reset_frame_index() {
    mFrameCounter = 0;
#ifdef DEBUG_SYNTHESIZER
    mFirstFrame = true;
#endif
}
