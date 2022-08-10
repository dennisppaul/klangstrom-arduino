/*
 * Klang – a node+text-based synthesizer library
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *       [ NODE_SAMPLER        ]
 *       +---------------------+
 *       |                     |
 *       |              SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

// @todo(implement input signal for speed)

#ifndef NodeSampler_hpp
#define NodeSampler_hpp

#include "KlangMath.hpp"
#include "KlangNode.hpp"

namespace klang {
    template <class BUFFER_TYPE>
    class NodeSamplerT : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 0;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeSamplerT() : NodeSamplerT(nullptr, 0) {}

        NodeSamplerT(const BUFFER_TYPE* pBuffer, uint32_t pLength) {
            fLoop          = true;
            fSpeed         = 1.0;
            fBuffer        = pBuffer;
            fLength        = pLength;
            fBufferPointer = fLength;
            fIsPlaying     = false;
            fIn            = 0.0;
            fOut           = 1.0;
            fAmplification = 1.0;
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
            fBufferPointer = 0;
            fIsPlaying     = true;
        }

        void set_position(uint32_t pCounter) {
            fBufferPointer = pCounter;
        }

        uint32_t get_position() {
            return fBufferPointer;
        }

        void set_buffer(const BUFFER_TYPE* pBuffer) {
            fBuffer = pBuffer;
        }

        const BUFFER_TYPE* get_buffer() {
            return fBuffer;
        }

        void set_speed(float pSpeed) {
            fSpeed = pSpeed;
        }

        float get_speed() {
            return fSpeed;
        }

        void set_amplification(float pAmplification) {
            fAmplification = pAmplification;
        }

        float get_amplification() {
            return fAmplification;
        }

        void set_buffer_size(uint32_t pLength) {
            fLength        = pLength;
            fBufferPointer = fLength;
        }

        uint32_t get_buffer_size() {
            return fLength;
        }

        void set_in(SIGNAL_TYPE pIn) {
            fIn = KlangMath::clamp(pIn, 0.0, fOut);
        }

        SIGNAL_TYPE get_in() {
            return fIn;
        }

        void set_out(SIGNAL_TYPE pOut) {
            fOut = KlangMath::clamp(pOut, fIn, 1.0);
        }

        SIGNAL_TYPE get_out() {
            return fOut;
        }

        void loop(bool pLoop) {
            fLoop = pLoop;
        }

        bool is_playing() {
            return fIsPlaying;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_SPEED_F32:
                    set_speed(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

        inline SIGNAL_TYPE next_sample() {
            if (fBuffer != nullptr) {
                /* quit early if buffer has no elements or loop ended */
                if (fLength == 0) {
                    return 0.0;
                }
                // if (!fLoop && (fBufferPointer >= fLength || fBufferPointer < 0)) {
                //     return 0.0;
                // }
                if (!fIsPlaying) {
                    return 0.0;
                }

                /* move forward in buffer */
                fBufferPointer += fSpeed;

                if (fIn == 0.0 && fOut == 1.0) {
                    /* check if done */
                    if (!fLoop && (fBufferPointer >= fLength || fBufferPointer < 0)) {
                        fIsPlaying = false;
                        return 0.0;
                    }

                    /* wrap around -- trying maintaing fraction */
                    while (fBufferPointer >= fLength) {
                        fBufferPointer -= fLength;
                    }
                    while (fBufferPointer < 0) {
                        fBufferPointer += fLength;
                    }
                } else {
                    const uint32_t mIn     = fLength * fIn;
                    const uint32_t mOut    = fLength * fOut;
                    const uint32_t mLength = mOut - mIn;
                    if (mLength == 0) {
                        return 0.0;
                    }

                    /* check if done */
                    if (!fLoop && (fBufferPointer >= mOut || fBufferPointer < mIn)) {
                        fIsPlaying = false;
                        return 0.0;
                    }

                    /* wrap around -- trying maintaing fraction */
                    while (fBufferPointer >= mOut) {
                        fBufferPointer -= mLength;
                    }
                    while (fBufferPointer < mIn) {
                        fBufferPointer += mLength;
                    }
                }

                /* retreive sample */
                // @TODO(consider interpolation between samples … see wavetable)
                uint32_t          mIndex  = (uint32_t)fBufferPointer;
                const BUFFER_TYPE mSample = fBuffer[mIndex];
                return convert_sample(mSample) * fAmplification;
            } else {
                return 0.0;
            }
        }

    private:
        bool               fLoop;
        const BUFFER_TYPE* fBuffer;
        uint32_t           fLength;
        float              fBufferPointer;
        float              fSpeed;
        float              fAmplification;
        bool               fIsPlaying;
        float              fIn;
        float              fOut;

        inline float convert_sample(const BUFFER_TYPE pRawSample) {
            return pRawSample;
        }
    };

    template <>
    float klang::NodeSamplerT<uint8_t>::convert_sample(const uint8_t pRawSample) {
        const static float mScale = 1.0 / ((1 << 8) - 1);
        const float        mRange = pRawSample * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    float klang::NodeSamplerT<int8_t>::convert_sample(const int8_t pRawSample) {
        const static float mScale  = 1.0 / ((1 << 8) - 1);
        const float        mOffset = pRawSample + (1 << 7);
        const float        mRange  = mOffset * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    float klang::NodeSamplerT<uint16_t>::convert_sample(const uint16_t pRawSample) {
        const static float mScale = 1.0 / ((1 << 16) - 1);
        const float        mRange = pRawSample * mScale;
        return mRange * 2.0 - 1.0;
        // @note(below: less precise but faster)
        // const float s      = pRawSample;
        // static const float mScale = 1.0 / (1 << 15);
        // const float a      = s * mScale - 1.0;
        // return a;
    }

    template <>
    float klang::NodeSamplerT<int16_t>::convert_sample(const int16_t pRawSample) {
        const static float mScale  = 1.0 / ((1 << 16) - 1);
        const float        mOffset = pRawSample + (1 << 15);
        const float        mRange  = mOffset * mScale;
        return mRange * 2.0 - 1.0;
    }

    using NodeSamplerUI8  = NodeSamplerT<uint8_t>;
    using NodeSamplerI8   = NodeSamplerT<int8_t>;
    using NodeSamplerUI16 = NodeSamplerT<uint16_t>;
    using NodeSamplerI16  = NodeSamplerT<int16_t>;
    using NodeSamplerF32  = NodeSamplerT<float>;
    using NodeSampler     = NodeSamplerT<SIGNAL_TYPE>;
}  // namespace klang

#endif /* NodeSampler_hpp */
