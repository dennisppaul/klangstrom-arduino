//
//  NodePatch16.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_PATCH_16       ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |  ...          ...   |
 * IN15--| SIGNAL       SIGNAL |--OUT15
 *       |                     |
 *       +---------------------+
 */

#ifndef NodePatch16_hpp
#define NodePatch16_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodePatch16 : public Node {
    public:
        //@TODO(only create channels when they are needed … this is very inefficient)
        static const CHANNEL_ID NUM_CH_IN  = 16;
        static const CHANNEL_ID NUM_CH_OUT = NUM_CH_IN;

        NodePatch16() {
            for (uint8_t i = 0; i < NUM_CH_IN; i++) {
                mInputs.push_back(NodePassthrough());
                mOutputs.push_back(NodeBuffer());
            }
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel < NUM_CH_IN) {
                return input(pInChannel).connect(pConnection, CH_IN_SIGNAL);
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel < NUM_CH_IN) {
                return input(pInChannel).disconnect(CH_IN_SIGNAL);
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                for (uint8_t i = 0; i < NUM_CH_IN; i++) {
                    mBlocks_IN[i]           = AudioBlockPool::instance().request();
                    SIGNAL_TYPE* mBlockData = AudioBlockPool::instance().data(mBlocks_IN[i]);
                    output(i).update(Node::CH_IN_SIGNAL, mBlockData);
                }
                flag_updated();
            }
            if (pChannel < NUM_CH_OUT) {
                SIGNAL_TYPE* mBlockData = AudioBlockPool::instance().data(mBlocks_IN[pChannel]);
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBlockData[i];
                }
            }
        }

        NodeBuffer& output(CHANNEL_ID pChannel) {
            return mOutputs[pChannel];
        }

        NodePassthrough& input(CHANNEL_ID pChannel) {
            return mInputs[pChannel];
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        std::vector<NodePassthrough> mInputs;
        std::vector<NodeBuffer>      mOutputs;
        std::vector<AUDIO_BLOCK_ID>  mBlocks_IN;
    };
}  // namespace klang

#endif /* NodePatch16_hpp */
