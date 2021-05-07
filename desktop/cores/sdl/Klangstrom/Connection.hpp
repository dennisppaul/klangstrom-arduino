//
//  Connection.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef Connection_hpp
#define Connection_hpp

#include "Node.hpp"

namespace klang {
    class Connection {
    public:
        Connection(Node& pOutputNode, CHANNEL_ID pOutputChannel,
                   Node& pInputNode, CHANNEL_ID pInputChannel,
                   bool& pStatus);
        Connection(Node& pOutputNode, Node& pInputNode);
        ~Connection();
        
        CONNECTION_ID ID() {
            return mID;
        }
        
        Node &output_node;
        CHANNEL_ID output_channel_id;
        Node &input_node;
        CHANNEL_ID input_channel_id;
        
        void update(SIGNAL_TYPE* pAudioBlock);
        void update(AUDIO_BLOCK_ID pAudioBlockID);
        
        static void reset();
    private:
        CONNECTION_ID mID;
        static CONNECTION_ID oID;
    };
}

#endif /* Connection_hpp */
