//
//  NodeNoise.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_NOISE          ]
 *       +---------------------+
 *       |                     |
 *       |              SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeNoise_hpp
#define NodeNoise_hpp

#include "Node.hpp"

namespace klang {
    class NodeNoise : public Node {
    public:
        enum TYPE {
            RANDOM,
            SIMPLEX,
        };

        static const CHANNEL_ID NUM_CH_IN  = 0;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) { return false; }

        bool disconnect(CHANNEL_ID pInChannel) { return false; }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (pChannel == CH_OUT_SIGNAL) {
                switch (mType) {
                    case RANDOM:
                        for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                            pAudioBlock[i] = KlangMath::random();
                        }
                        break;
                    case SIMPLEX:
                        for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                            pAudioBlock[i] = SimplexNoise::noise(mStrideCounter += mStride);
                        }
                        break;
                }
            }
        }

        void set_stride(SIGNAL_TYPE pStride) {
            mStride = pStride;
        }

        SIGNAL_TYPE get_stride() {
            return mStride;
        }

        void set_type(TYPE pType) {
            mType = pType;
        }

        TYPE get_type() {
            return mType;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_STRIDE_F32:
                    set_stride(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_TYPE_I8:
                    set_type(static_cast<TYPE>(pPayLoad[0]));
                    break;
            }
        }

    private:
        float       mStrideCounter = 0.0;
        SIGNAL_TYPE mStride        = 0.01;
        TYPE        mType          = RANDOM;
    };
}  // namespace klang

#endif /* NodeNoise_hpp */
