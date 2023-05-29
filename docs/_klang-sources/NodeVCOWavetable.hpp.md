---
layout: libdoc
title: NodeVCOWavetable.hpp
permalink: /NodeVCOWavetable.hpp/
index: 89
---

```c
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
 *       [ NODE_VCO_WAVETABLE  ]
 *       +---------------------+
 *       |                     |
 * IN00--| FREQ         SIGNAL |--OUT00
 * IN01--| AMP                 |
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          the wavetable node produce a periodic signal from a chunk of memory ( wavetable ).
 *          the speed at which the signal is produced from the wavetable can be adjusted by the
 *          changing its frequency `FREQ` and the amplitude by changing `AMP`. the node is often
 *          used as an *oscillator* to reproduce a sinewave, triangle, square or sawtooth shaped
 *          signal at an audible frequency ( Voltage-Controlled Oscillator (`VCO`) ). however, a
 *          wavetable node can also be used to manipulate other parameters ( e.g the frequency
 *          of another wavetable node ) at slower speeds ( Low Frequency Oscillator (`LFO`) ).
 *          note that the maximum size of a wavetable is 65536 samples.
 *       )
 *
 *       @set_value(FREQUENCY_F32,AMPLITUDE_F32,OFFSET_F32,WAVEFORM)
 */

#ifndef NodeVCOWavetable_hpp
#define NodeVCOWavetable_hpp

#include "KlangMath.hpp"

namespace klang {
    template <class BUFFER_TYPE>
    class NodeVCOWavetableT : public Node {
        // class NodeVCOWavetable : public Node {
    public:
        static const CHANNEL_ID CH_IN_FREQ = 0;
        static const CHANNEL_ID CH_IN_AMP  = 1;
        static const CHANNEL_ID NUM_CH_IN  = 2;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        static const uint16_t WAVETABLE_SIZE_DEFAULT = 256;

        enum WAVEFORM {
            SINE = 0,
            TRIANGLE,
            SAWTOOTH,
            SQUARE,
            EXPONENT
        };

        NodeVCOWavetableT(uint16_t pWavetableLength) : mWavetableLength(pWavetableLength) {
            mWavetable   = new BUFFER_TYPE[mWavetableLength];  // @TODO(consider reusing wavetable where possible)
            mDeleteArray = true;
            if (!isPowerOfTwo(pWavetableLength)) {
                KLANG_LOG_ERR("### warning wavetable length needs to be *power of two*");
            }
            set_frequency(OSC_DEFAULT_FREQUENCY);
            set_amplitude(OSC_DEFAULT_AMPLITUDE);
        }

        NodeVCOWavetableT() : NodeVCOWavetableT(WAVETABLE_SIZE_DEFAULT) {}

        NodeVCOWavetableT(BUFFER_TYPE* pWavetable, uint16_t pWavetableLength) : mWavetable(pWavetable), mWavetableLength(pWavetableLength) {
            mDeleteArray = false;
            set_frequency(OSC_DEFAULT_FREQUENCY);
            set_amplitude(OSC_DEFAULT_AMPLITUDE);
        }

        NodeVCOWavetableT(const NodeVCOWavetableT& obj) : mWavetableLength(obj.mWavetableLength) {
            mDeleteArray = obj.mDeleteArray;
            mWavetable   = obj.mWavetable;
            set_frequency(obj.mFrequency);
            set_amplitude(obj.mAmplitude);
        }

        NodeVCOWavetableT& operator=(const NodeVCOWavetableT& obj) {
            mDeleteArray = obj.mDeleteArray;
            mWavetable   = obj.mWavetable;
            set_frequency(obj.mFrequency);
            set_amplitude(obj.mAmplitude);
            return *this;
        }

        ~NodeVCOWavetableT() {
            if (mDeleteArray) {
                delete[] mWavetable;
            }
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_FREQ) {
                mConnection_CH_IN_FREQ = pConnection;
                return true;
            } else if (pInChannel == CH_IN_AMP) {
                mConnection_CH_IN_AMP = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_FREQ) {
                mConnection_CH_IN_FREQ = nullptr;
                return true;
            } else if (pInChannel == CH_IN_AMP) {
                mConnection_CH_IN_AMP = nullptr;
                return true;
            }
            return false;
        }

        void set_wavetable(BUFFER_TYPE* pWavetable) {
            mWavetable = pWavetable;
        }

        const BUFFER_TYPE* get_wavetable() {
            return mWavetable;
        }

        const uint16_t get_wavetable_length() {
            return mWavetableLength;
        }

        void set_array_ptr(const uint16_t pArrayPtr) {
            const uint16_t mLastIndex = mWavetableLength - 1;
            mArrayPtr                 = pArrayPtr > mLastIndex ? mLastIndex : pArrayPtr;
        }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_FREQUENCY_F32:
                    set_frequency(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_AMPLITUDE_F32:
                    set_amplitude(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_OFFSET_F32:
                    set_offset(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_WAVEFORM_I8:
                    set_waveform(static_cast<WAVEFORM>(pPayLoad[0]));
                    break;
            }
        }

        void static fill_wavetable(WAVEFORM       pWaveform,
                                   BUFFER_TYPE*   pWavetable,
                                   const uint16_t pWavetableSize) {
            switch (pWaveform) {
                case SINE: {
                    for (uint16_t i = 0; i < pWavetableSize; i++) {
                        pWavetable[i] = SCALE_WAVETABLE(sin(2.0 * PI * ((float)i / (float)(pWavetableSize))));
                    }
                    break;
                }
                case TRIANGLE: {
                    for (uint16_t i = 0; i < (pWavetableSize / 4); i++) {
                        pWavetable[i]                            = SCALE_WAVETABLE(i / (float)(pWavetableSize / 4));
                        pWavetable[i + (pWavetableSize * 1 / 4)] = SCALE_WAVETABLE(((pWavetableSize / 4) - i) / (float)(pWavetableSize / 4));
                        pWavetable[i + (pWavetableSize * 2 / 4)] = SCALE_WAVETABLE(-i / (float)(pWavetableSize / 4));
                        pWavetable[i + (pWavetableSize * 3 / 4)] = SCALE_WAVETABLE(-((pWavetableSize / 4) - i) / (float)(pWavetableSize / 4));
                    }
                    break;
                }
                case SAWTOOTH: {
                    for (uint16_t i = 0; i < pWavetableSize; i++) {
                        pWavetable[i] = SCALE_WAVETABLE(2.0 * ((float)i / (float)(pWavetableSize - 1)) - 1.0);
                    }
                    break;
                }
                case SQUARE: {
                    for (uint16_t i = 0; i < (pWavetableSize / 2); i++) {
                        pWavetable[i]                        = SCALE_WAVETABLE(1.0);
                        pWavetable[i + (pWavetableSize / 2)] = SCALE_WAVETABLE(-1.0);
                    }
                    break;
                }
                case EXPONENT: {
                    /* symetric exponent similar to triangle */
                    float e = (float)exp(1.0f);
                    for (uint16_t i = 0; i < (pWavetableSize / 2); i++) {
                        pWavetable[i]                        = SCALE_WAVETABLE(2.0 * ((exp(i / (float)(pWavetableSize / 2)) - 1.0) / (e - 1.0)) - 1.0);
                        pWavetable[i + (pWavetableSize / 2)] = SCALE_WAVETABLE(2.0 * ((exp(((pWavetableSize / 2) - i) / (float)(pWavetableSize / 2)) - 1.0) / (e - 1.0)) - 1.0);
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }

        void set_amplitude(float pAmplitude) {
            mAmplitude = pAmplitude;
        }

        const float get_amplitude() {
            return mAmplitude;
        }

        void set_offset(float pOffset) {
            mOffset = pOffset;
        }

        const float get_offset() {
            return mOffset;
        }

        void set_waveform(WAVEFORM pWaveform) {
            fill_wavetable(pWaveform, wavetable_ptr(), wavetable_size());
        }

        void set_waveform(uint8_t pWaveform) {
            fill_wavetable((WAVEFORM)pWaveform, wavetable_ptr(), wavetable_size());
        }

        void wavetable_write(uint16_t pIndex, BUFFER_TYPE pSample) {
            mWavetable[pIndex] = pSample;
        }

        uint16_t wavetable_size() {
            return mWavetableLength;
        }

        BUFFER_TYPE* wavetable_ptr() {
            return mWavetable;
        }

        void set_interpolate_samples(bool pInterpolateSamples) {
            fInterpolateSamples = pInterpolateSamples;
        }

        bool get_interpolate_samples() {
            return fInterpolateSamples;
        }

        /* +++++++++++++++++++++++++++++++++++++++++++++++++++ */
        /* +++++++++++++ FLOATING-POINT FUNCTIONS ++++++++++++ */
        /* +++++++++++++++++++++++++++++++++++++++++++++++++++ */

        void set_frequency(float pFrequency) {
            if (mFrequency != pFrequency) {
                mFrequency    = pFrequency;
                const float a = (float)mWavetableLength * KLANG_AUDIO_RATE_UINT16_INV;
                mStepSize     = mFrequency * a;
            }
        }

        const float get_frequency() { return mFrequency; }

        void update(CHANNEL_ID pChannel, float* pAudioBlock) {
            if (is_not_updated()) {
                mBlock_FREQ = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_FREQ != nullptr) {
                    mBlock_FREQ = AudioBlockPool::instance().request();
                    mConnection_CH_IN_FREQ->update(mBlock_FREQ);
                }
                mBlock_AMP = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_AMP != nullptr) {
                    mBlock_AMP = AudioBlockPool::instance().request();
                    mConnection_CH_IN_AMP->update(mBlock_AMP);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                float* mBlockData_FREQ = nullptr;
                if (mBlock_FREQ != AudioBlockPool::NO_ID) {
                    mBlockData_FREQ = AudioBlockPool::instance().data(mBlock_FREQ);
                }
                const bool mHasFreqBuffer = (mBlockData_FREQ != nullptr);

                const float* mBlockData_AMP = nullptr;
                if (mBlock_AMP != AudioBlockPool::NO_ID) {
                    mBlockData_AMP = AudioBlockPool::instance().data(mBlock_AMP);
                }
                const bool mHasAmpBuffer = (mBlockData_AMP != nullptr);

                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    /* frequency */
                    if (mHasFreqBuffer) {
                        set_frequency(mBlockData_FREQ[i]);
                    }
                    /* wavetable */
                    mArrayPtr += mStepSize;
                    const uint16_t mInt   = (uint16_t)mArrayPtr;
                    const float    mFrac  = mArrayPtr - mInt;
                    const uint16_t mIndex = mInt & (mWavetableLength - 1);
                    mArrayPtr             = mIndex + mFrac;
                    if (fInterpolateSamples) {
                        const uint16_t mIndexNext = (mIndex + 1) & (mWavetableLength - 1);
                        const float    r          = (1.0 - mFrac);
                        const float    a          = SCALE_SAMPLE(mWavetable[mIndex]) * r;
                        const float    b          = SCALE_SAMPLE(mWavetable[mIndexNext]) * mFrac;
                        pAudioBlock[i]            = a + b;
                    } else {
                        pAudioBlock[i] = SCALE_SAMPLE(mWavetable[mIndex]);
                    }
                    pAudioBlock[i] *= mHasAmpBuffer ? mBlockData_AMP[i] : mAmplitude;
                    pAudioBlock[i] += mOffset;
                }
            }

            /* release blocks */
            if (mBlock_AMP != AudioBlockPool::NO_ID) {
                AudioBlockPool::instance().release(mBlock_AMP);
            }

            if (mBlock_FREQ != AudioBlockPool::NO_ID) {
                AudioBlockPool::instance().release(mBlock_FREQ);
            }
        }

    private:
        inline float SCALE_SAMPLE(const BUFFER_TYPE pRawSample) {
            return pRawSample;
        }

        inline static BUFFER_TYPE SCALE_WAVETABLE(const float pRawSample) {
            return pRawSample;
        }

        float mFrequency = 0.0;
        float       mStepSize  = 0.0;
        float       mArrayPtr  = 0.0;

        void wrap_array_ptr() {
            while (mArrayPtr >= mWavetableLength) {
                mArrayPtr -= mWavetableLength;
            }
        }

        static constexpr float KLANG_AUDIO_RATE_UINT16_INV = 1.0 / (float)KLANG_AUDIO_RATE_UINT16;
        BUFFER_TYPE*           mWavetable;
        const uint16_t         mWavetableLength;
        bool                   mDeleteArray;
        bool                   fInterpolateSamples = false;

        bool isPowerOfTwo(uint16_t x) {
            while (((x % 2) == 0) && x > 1) {
                x /= 2;
            }
            return (x == 1);
        }

        AUDIO_BLOCK_ID mBlock_FREQ = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_AMP  = AudioBlockPool::NO_ID;

        Connection* mConnection_CH_IN_FREQ = nullptr;
        Connection* mConnection_CH_IN_AMP  = nullptr;

        float mAmplitude = SIGNAL_MAX;
        float mOffset    = 0.0;
    };

    template <>
    float klang::NodeVCOWavetableT<uint8_t>::SCALE_SAMPLE(const uint8_t pRawSample) {
        const static float mScale = 1.0 / ((1 << 8) - 1);
        const float        mRange = pRawSample * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    float klang::NodeVCOWavetableT<int8_t>::SCALE_SAMPLE(const int8_t pRawSample) {
        const static float mScale  = 1.0 / ((1 << 8) - 1);
        const float        mSampleOffset = pRawSample + (1 << 7);
        const float        mRange  = mSampleOffset * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    float klang::NodeVCOWavetableT<uint16_t>::SCALE_SAMPLE(const uint16_t pRawSample) {
        const static float mScale = 1.0 / ((1 << 16) - 1);
        const float        mRange = pRawSample * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    float klang::NodeVCOWavetableT<int16_t>::SCALE_SAMPLE(const int16_t pRawSample) {
        const static float mScale  = 1.0 / ((1 << 16) - 1);
        const float        mSampleOffset = pRawSample + (1 << 15);
        const float        mRange  = mSampleOffset * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    uint8_t klang::NodeVCOWavetableT<uint8_t>::SCALE_WAVETABLE(const float pRawSample) {
        static const float mScale = ((1 << 8) - 1) / 2.0;
        return pRawSample * mScale + mScale;
    }

    template <>
    int8_t klang::NodeVCOWavetableT<int8_t>::SCALE_WAVETABLE(const float pRawSample) {
        static const float mScale = ((1 << 8) - 1) / 2.0;
        return pRawSample * mScale - 0.5;
    }

    template <>
    uint16_t klang::NodeVCOWavetableT<uint16_t>::SCALE_WAVETABLE(const float pRawSample) {
        static const float mScale = ((1 << 16) - 1) / 2.0;
        return pRawSample * mScale + mScale;
    }

    template <>
    int16_t klang::NodeVCOWavetableT<int16_t>::SCALE_WAVETABLE(const float pRawSample) {
        static const float mScale = ((1 << 16) - 1) / 2.0;
        return pRawSample * mScale - 0.5;
    }

    using NodeVCOWavetableUI8  = NodeVCOWavetableT<uint8_t>;
    using NodeVCOWavetableI8   = NodeVCOWavetableT<int8_t>;
    using NodeVCOWavetableUI16 = NodeVCOWavetableT<uint16_t>;
    using NodeVCOWavetableI16  = NodeVCOWavetableT<int16_t>;
    using NodeVCOWavetableF32  = NodeVCOWavetableT<float>;
    using NodeVCOWavetable     = NodeVCOWavetableT<float>;
}  // namespace klang

#endif /* NodeVCOWavetable_hpp */

```
