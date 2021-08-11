//
//  NodePatch.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_PATCH          ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

/**
 *       [ NODE_PATCH_STEREO   ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| SIGNAL       SIGNAL |--OUT01
 *       |                     |
 *       +---------------------+
 */

/**
 *       [ NODE_PATCH_MULTI    ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |  ...          ...   |
 * INFF--| SIGNAL       SIGNAL |--OUTFF
 *       |                     |
 *       +---------------------+
 */
// @TODO implement `NodePatchStereo` and `NodePatchMulti` for multiple in- and outputs

#ifndef NodePatch_hpp
#define NodePatch_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodePatch : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        void setup() {
            // @TODO("get rid of this method call … but constructor does not work … yet")
            Klang::connect(input(), Node::CH_OUT_SIGNAL, *this, Node::CH_IN_SIGNAL);
            Klang::connect(*this, Node::CH_OUT_SIGNAL, output(), Node::CH_IN_SIGNAL);
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                return input().connect(pConnection, pInChannel);
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                return input().disconnect(pInChannel);
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                output().update(Node::CH_IN_SIGNAL, pAudioBlock);
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
            }
        }

        NodePassthrough& output() {
            return mOutput;
        }

        NodePassthrough& input() {
            return mInput;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        NodePassthrough mInput;
        NodePassthrough mOutput;
    };
}  // namespace klang

#endif /* NodePatch_hpp */
