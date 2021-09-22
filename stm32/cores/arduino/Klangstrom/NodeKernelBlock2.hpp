//
//  NodeKernelBlock2.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_KERNEL_BLOCK_2 ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL_A     SIGNAL |--OUT00
 * IN00--| SIGNAL_B            |
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          abstract node that allows the easy creation of new signal processing nodes by
 *          overriding the `kernel` method. this kernel node allows to process two input
 *          signals.
 *       )
 */

#ifndef NodeKernelBlock2_hpp
#define NodeKernelBlock2_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeKernelBlock2 : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL_A = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_B = 1;
        static const CHANNEL_ID NUM_CH_IN      = 2;
        static const CHANNEL_ID NUM_CH_OUT     = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_A) {
                mConnection_CH_IN_SIGNAL_A = pConnection;
                return true;
            } else if (pInChannel == CH_IN_SIGNAL_B) {
                mConnection_CH_IN_SIGNAL_B = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_A) {
                mConnection_CH_IN_SIGNAL_A = nullptr;
                return true;
            } else if (pInChannel == CH_IN_SIGNAL_B) {
                mConnection_CH_IN_SIGNAL_B = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            // @todo(check if the behavior ( only process if both if both input channels are connected ) is desired)
            if (mConnection_CH_IN_SIGNAL_A != nullptr && mConnection_CH_IN_SIGNAL_B != nullptr) {
                if (is_not_updated()) {
                    if (mConnection_CH_IN_SIGNAL_A != nullptr) {
                        mBlock_SIGNAL_A = AudioBlockPool::instance().request();
                        mConnection_CH_IN_SIGNAL_A->update(mBlock_SIGNAL_A);
                    }
                    if (mConnection_CH_IN_SIGNAL_B != nullptr) {
                        mBlock_SIGNAL_B = AudioBlockPool::instance().request();
                        mConnection_CH_IN_SIGNAL_B->update(mBlock_SIGNAL_B);
                    }
                    flag_updated();
                }
                if (pChannel == CH_OUT_SIGNAL) {
                    SIGNAL_TYPE* mBlockData_IN_SIGNAL_A = AudioBlockPool::instance().data(mBlock_SIGNAL_A);
                    SIGNAL_TYPE* mBlockData_IN_SIGNAL_B = AudioBlockPool::instance().data(mBlock_SIGNAL_B);
                    kernel(pAudioBlock, mBlockData_IN_SIGNAL_A, mBlockData_IN_SIGNAL_B);
                }
            } else {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = 0;
                }
            }
        }

        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad){};

    protected:
        /* override kernel method to implement custom kernels. */
        virtual void kernel(SIGNAL_TYPE* pOutputSignal,
                            SIGNAL_TYPE* pInputSignal_A,
                            SIGNAL_TYPE* pInputSignal_B) = 0;

    private:
        Connection* mConnection_CH_IN_SIGNAL_A = nullptr;
        Connection* mConnection_CH_IN_SIGNAL_B = nullptr;

        AUDIO_BLOCK_ID mBlock_SIGNAL_A = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_SIGNAL_B = AudioBlockPool::NO_ID;
    };
}  // namespace klang

#endif /* NodeKernelBlock2_hpp */
