//
//  NodeSampler.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_SAMPLER        ]
 *       +---------------------+
 *       |                     |
 *       |              SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeSampler_hpp
#define NodeSampler_hpp

#include "KlangNode.hpp"

namespace klang {
    template <class BUFFER_TYPE>
    class NodeSamplerT : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 0;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeSamplerT() : NodeSamplerT(nullptr, 0) {}

        NodeSamplerT(const BUFFER_TYPE* pBuffer, uint32_t pLength) {
            mLoop    = true;
            mSpeed   = 1.0;
            mBuffer  = pBuffer;
            mLength  = pLength;
            mCounter = mLength;
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) { return false; }

        bool disconnect(CHANNEL_ID pInChannel) { return false; }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = next_sample();
                }
            }
        }

        void start() {
            mCounter = 0;
        }

        void set_position(uint32_t pCounter) {
            mCounter = pCounter;
        }

        uint32_t get_position() {
            return mCounter;
        }

        void set_buffer(const BUFFER_TYPE* pBuffer) {
            mBuffer = pBuffer;
        }

        const BUFFER_TYPE* get_buffer() {
            return mBuffer;
        }

        void set_speed(float pSpeed) {
            mSpeed = pSpeed;
        }

        float get_speed() {
            return mSpeed;
        }

        void set_buffer_size(uint32_t pLength) {
            mLength  = pLength;
            mCounter = mLength;
        }

        uint32_t get_buffer_size() {
            return mLength;
        }

        void loop(bool pLoop) {
            mLoop = pLoop;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_SPEED_F32:
                    set_speed(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    private:
        bool               mLoop;
        const BUFFER_TYPE* mBuffer;
        uint32_t           mLength;
        float              mCounter;
        float              mSpeed;

        inline SIGNAL_TYPE next_sample() {
            if (mBuffer != nullptr) {
                if (mCounter >= mLength) {
                    if (mLoop) {
                        while (mCounter >= mLength) {
                            mCounter -= mLength;
                        }
                    } else {
                        return 0.0;
                    }
                }
                const uint32_t mIndex = (uint32_t)mCounter;
                mCounter += mSpeed;
                if (std::is_same<BUFFER_TYPE, float>::value) {
                    return mBuffer[mIndex];
                } else if (std::is_same<BUFFER_TYPE, uint8_t>::value) {
                    // const float s = mBuffer[mIndex];
                    // const float mScale = 1.0 / ((1 << 7) - 1.0);
                    // const float a = s * mScale - 1.0;
                    // return a;
                    return (mBuffer[mIndex] / 255.0) * 2.0 - 1.0;
                } else if (std::is_same<BUFFER_TYPE, uint16_t>::value) {
                    const float s      = mBuffer[mIndex];
                    const float mScale = 1.0 / ((1 << 15) - 1.0);
                    const float a      = s * mScale - 1.0;
                    return a;
                } else {
                    return 0.0;
                }
            } else {
                return 0.0;
            }
        }
    };

    using NodeSampler     = NodeSamplerT<SIGNAL_TYPE>;
    using NodeSamplerF32  = NodeSamplerT<float>;
    using NodeSamplerUI8  = NodeSamplerT<uint8_t>;
    using NodeSamplerUI16 = NodeSamplerT<uint16_t>;
}  // namespace klang

#endif /* NodeSampler_hpp */
