//
//  Klang.cpp
//  Klang – a node+text-based synthesizer library
//
//
//

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
