//
//  NodeKernel.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_KERNEL         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          abstract node that allows the easy creation of new signal processing nodes by
 *          overriding the `kernel` method.
 *       )
 */

#ifndef NodeKernel_hpp
#define NodeKernel_hpp

#include "Node.hpp"

namespace klang {
    class NodeKernel : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN         = 1;
        static const CHANNEL_ID NUM_CH_OUT        = 1;
               
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
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = kernel(pAudioBlock[i]);
                }
            }
        }
        
        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {};
        
    protected:
        /* override kernel method to implement custom kernels. */
        virtual SIGNAL_TYPE kernel(SIGNAL_TYPE s) = 0;
        
    private:
        Connection* mConnection_CH_IN_SIGNAL   = nullptr;
    };
}

#endif /* NodeKernel_hpp */
