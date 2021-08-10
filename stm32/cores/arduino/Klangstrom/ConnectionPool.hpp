//
//  ConnectionPool.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef ConnectionPool_hpp
#define ConnectionPool_hpp

#include <vector>

namespace klang {
    class ConnectionPool {
    public:
        static ConnectionPool& instance() {
            static ConnectionPool instance;
            return instance;
        }

        CONNECTION_ID connect(Node&      pOutputNode,
                              CHANNEL_ID pOutputChannel,
                              Node&      pInputNode,
                              CHANNEL_ID pInputChannel) {
            /* check existing connections */
            for (uint8_t i = 0; i < mConnections.size(); i++) {
                Connection* m              = mConnections[i];
                uint16_t    mNodeID        = m->output_node.ID();
                CHANNEL_ID  mOutputChannel = m->output_channel_id;
                if (mNodeID == pOutputNode.ID() && mOutputChannel == pOutputChannel && !pOutputNode.ouput_allow_multiple_connections()) {
                    KLANG_LOG_ERR("+++ NODE_%02i(OUT%02i) already connected           +++", pOutputNode.ID(), pOutputChannel);
                    return KLANG_CONNECTION_ERROR;
                }
            }
            //            if (pOutputChannel >= pOutputNode.max_out_channels()) {
            //                KLANG_LOG_ERR("+++ NODE_%02i(OUT%02i) is not valid … %i", pOutputNode.ID(), pOutputChannel, pOutputNode.CH_OUT_SIGNAL);
            //                return KLANG_CONNECTION_ERROR;
            //            }
            bool        mStatus;
            Connection* c = new Connection(pOutputNode,
                                           pOutputChannel,
                                           pInputNode,
                                           pInputChannel,
                                           mStatus);
            if (!mStatus) {
                KLANG_LOG_ERR("+++ NODE_%02i(OUT%02i) <CONNECT_%02i> NODE_%02i(IN%02i) error connecting, rejected by node. might be invalid `CHANNEL_ID`",
                              pOutputNode.ID(), pOutputChannel,
                              c->ID(),
                              pInputNode.ID(), pInputChannel);
            }
            mConnections.push_back(c);
#ifdef DEBUG_CONNECTION_POOL
            KLANG_LOG("+++ NODE_%02i(OUT%02i) <CONNECT_%02i> NODE_%02i(IN%02i)  +++",
                      pOutputNode.ID(), pOutputChannel,
                      c->ID(),
                      pInputNode.ID(), pInputChannel);
#endif
            return c->ID();
        }

        bool disconnect(Node&      pOutputNode,
                        CHANNEL_ID pOutputChannel,
                        Node&      pInputNode,
                        CHANNEL_ID pInputChannel) {
            std::vector<Connection*>::iterator it = mConnections.begin();
            while (it != mConnections.end()) {
                Connection* m              = *it;
                uint16_t    mNodeID        = m->output_node.ID();
                CHANNEL_ID  mOutputChannel = m->output_channel_id;
                if (mNodeID == pOutputNode.ID() && mOutputChannel == pOutputChannel) {
#ifdef DEBUG_CONNECTION_POOL
                    KLANG_LOG("+++ NODE_%02i(OUT%02i) disconnected                +++", pOutputNode.ID(), pOutputChannel);
#endif
                    it = mConnections.erase(it);
                    delete m;
                    return true;
                } else {
                    ++it;
                }
            }
            return false;
        }

        bool disconnect(CONNECTION_ID pConnectionID) {
            std::vector<Connection*>::iterator it = mConnections.begin();
            while (it != mConnections.end()) {
                Connection* m = *it;
                if (m->ID() == pConnectionID) {
                    it = mConnections.erase(it);
                    delete m;
                    return true;
                }
            }
            return false;
        }

        void reset() {
            for (uint16_t i = 0; i < mConnections.size(); i++) {
                delete mConnections[i];
            }
            mConnections.clear();
            Connection::reset();
        }

    private:
        ConnectionPool(ConnectionPool const&);
        void operator=(ConnectionPool const&);

        ConnectionPool() {
#ifdef DEBUG_CONNECTION_POOL
            KLANG_LOG("++++++++++++++++++++++++++++++++++++++++++++++++++");
            KLANG_LOG("+++ @ConnectionPool                            +++");
#endif
        }

        std::vector<Connection*> mConnections;
    };
}  // namespace klang
#endif /* ConnectionPool_hpp */
