//
//  NodeBuffer.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_BUFFER         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUTFF
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeBuffer_hpp
#define NodeBuffer_hpp

#include "Node.hpp"

namespace klang {
    class NodeBuffer : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN       = 1;
        static const CHANNEL_ID NUM_CH_OUT      = 255;
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            }
            return false;
        }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mBlock_SIGNAL = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL->update(mBlock_SIGNAL);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                SIGNAL_TYPE* mBlockData_IN_SIGNAL = AudioBlockPool::instance().data(mBlock_SIGNAL);
                // @TODO(should probably use `std::copy` here)
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBlockData_IN_SIGNAL[i];
                }
            }
        }
        
        bool ouput_allow_multiple_connections() {
            return false;
        }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}
        
    private:
        AUDIO_BLOCK_ID mBlock_SIGNAL = AudioBlockPool::NO_ID;
        
        Connection* mConnection_CH_IN_SIGNAL = nullptr;
    };
}

#endif /* NodeBuffer_hpp */
