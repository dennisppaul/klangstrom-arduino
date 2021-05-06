//
//  NodeADC.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_ADC            ]
 *       +---------------------+
 *       |                     |
 *       |       SIGNAL(_LEFT) |--OUT00
 *       |       SIGNAL_RIGHT  |--OUT01
 *       |                     |
 *       +---------------------+
 *
 */

#ifndef NodeADC_hpp
#define NodeADC_hpp

#include "Node.hpp"
#include <algorithm>

namespace klang {
    class NodeADC : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_LEFT = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_OUT = 2;
        
        bool connect(Connection *pConnection, CHANNEL_ID pInChannel) {
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            return false;
        }
        
        void process_frame(SIGNAL_TYPE *pLeft, SIGNAL_TYPE *pRight) {
            std::copy(pLeft, pLeft + KLANG_SAMPLES_PER_AUDIO_BLOCK, mBufferLeft);
            std::copy(pRight, pRight + KLANG_SAMPLES_PER_AUDIO_BLOCK, mBufferRight);
        }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE *pAudioBlock) {
            if (pChannel == CH_OUT_SIGNAL_LEFT) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBufferLeft[i];
                }
            }
            if (pChannel == CH_OUT_SIGNAL_RIGHT) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBufferRight[i];
                }
            }
        }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}
        
    private:
        SIGNAL_TYPE mBufferLeft[KLANG_SAMPLES_PER_AUDIO_BLOCK];
        SIGNAL_TYPE mBufferRight[KLANG_SAMPLES_PER_AUDIO_BLOCK];
    };
}

#endif /* NodeADC_hpp */
