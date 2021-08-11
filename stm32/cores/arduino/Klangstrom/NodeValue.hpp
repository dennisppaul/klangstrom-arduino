//
//  NodeValue.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_VALUE          ]
 *       +---------------------+
 *       |                     |
 *       |              SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeValue_hpp
#define NodeValue_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeValue : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 0;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) { return false; }

        bool disconnect(CHANNEL_ID pInChannel) { return false; }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mValue;
                }
            }
        }

        void set_value(SIGNAL_TYPE pValue) {
            mValue = pValue;
        }

        SIGNAL_TYPE get_value() {
            return mValue;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_VALUE_F32:
                    set_value(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    private:
        SIGNAL_TYPE mValue = 0.0;
    };
}  // namespace klang

#endif /* NodeValue_hpp */
