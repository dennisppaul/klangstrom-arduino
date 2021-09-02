---
layout: libdoc
title: NodeFFT.hpp
permalink: /NodeFFT.hpp/
index: 53
---

```c
//
//  NodeFFT.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_FFT            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

// @note(uses_cmsis_dsp_library)

#include <CMSIS_DSP.h>

#ifndef NodeFFT_hpp
#define NodeFFT_hpp

#ifndef KLANG_NODE_FFT_PRECOMPUTE_HAMMING
#define KLANG_NODE_FFT_PRECOMPUTE_HAMMING 1
#endif

namespace klang {
    class NodeFFT : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

#if KLANG_NODE_FFT_PRECOMPUTE_HAMMING
        NodeFFT() {
            _fill_hamming_buffer();
        }
#endif

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

        void perform_analysis() {
            if (mPerfomHammingWindow) {
                window_with_hamming();
            }
            transform_to_frequency_domain();
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                flag_updated();
                std::copy(pAudioBlock,
                          pAudioBlock + KLANG_SAMPLES_PER_AUDIO_BLOCK,
                          mInputBuffer);
                if (mPerfomAnalysisInline) {
                    perform_analysis();
                }
            }
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

        float get_frequency() {
            return mMaxIndex * mFrequencyResolution;
        }

        float get_frequency_gaussian_interpolation() {
            // from [Improving FFT Frequency Resolution](http://www.add.ece.ufl.edu/4511/references/ImprovingFFTResoltuion.pdf)
            if (mMaxIndex > 0 && mMaxIndex < (KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF - 1)) {
                //calculate the intermittent bin on continuous spectrum using GI
                const float mInterBin = mMaxIndex + log(mPowerBuffer[mMaxIndex + 1] / mPowerBuffer[mMaxIndex - 1]) * 0.5 / log(mPowerBuffer[mMaxIndex] * mPowerBuffer[mMaxIndex] / (mPowerBuffer[mMaxIndex + 1] * mPowerBuffer[mMaxIndex - 1]));
                //calculate max input frequency
                return (mFrequencyResolution * mInterBin);
            } else {
                return mPowerBuffer[mMaxIndex];
            }
        }

        float* get_bands() {
            return mPowerBuffer;
        }

        const uint32_t get_number_of_bands() {
            return KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF;
        }

        void enable_hamming_window(bool pEnableWindowHamming) {
            mPerfomHammingWindow = pEnableWindowHamming;
        }

        bool is_hamming_window_enabled() {
            return mPerfomHammingWindow;
        }

        void enable_inline_analysis(bool pPerfomAnalysisInline) {
            mPerfomAnalysisInline = pPerfomAnalysisInline;
        }

    private:
        static const uint32_t KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF = KLANG_SAMPLES_PER_AUDIO_BLOCK / 2;
        Connection*           mConnection_CH_IN_SIGNAL           = nullptr;
        float                 mInputBuffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];
        float                 mOutputBuffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];
        float                 mPowerBuffer[KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF];
        float32_t             mMaxValue             = 0.0;
        uint32_t              mMaxIndex             = 0;
        uint8_t               mIFFTFlag             = 0;
        float                 mFrequencyResolution  = (float)KLANG_AUDIO_RATE / (float)KLANG_SAMPLES_PER_AUDIO_BLOCK;
        bool                  mPerfomAnalysisInline = true;
        bool                  mPerfomHammingWindow  = true;

        void transform_to_frequency_domain() {
            /* analyze signal */
            arm_rfft_fast_instance_f32 fft;
            arm_rfft_fast_init_f32(&fft, KLANG_SAMPLES_PER_AUDIO_BLOCK);
            arm_rfft_fast_f32(&fft, mInputBuffer, mOutputBuffer, mIFFTFlag);
            arm_cmplx_mag_f32(mOutputBuffer, mPowerBuffer, KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF);
            /* find dominant frequency */
            arm_max_f32(mPowerBuffer, KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF, &mMaxValue, &mMaxIndex);
        }

#if KLANG_NODE_FFT_PRECOMPUTE_HAMMING
        float mHammingBuffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];
        void  _fill_hamming_buffer() {
            for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                const float r = TWO_PI * i / (KLANG_SAMPLES_PER_AUDIO_BLOCK - 1);
                const float c = arm_cos_f32(r);
                mHammingBuffer[i] *= 0.54 - 0.46 * c;
            }
        }
#endif

        void window_with_hamming() {
#if KLANG_NODE_FFT_PRECOMPUTE_HAMMING
            arm_mult_f32(mInputBuffer, mHammingBuffer, mInputBuffer, KLANG_SAMPLES_PER_AUDIO_BLOCK);
#else
            /* windows the data in samples with a Hamming window */
            for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                const float r = TWO_PI * i / (KLANG_SAMPLES_PER_AUDIO_BLOCK - 1);
                // const float c = klang_math_cos(r);
                const float c = arm_cos_f32(r);
                mInputBuffer[i] *= 0.54 - 0.46 * c;
            }
#endif
        }
    };

}  // namespace klang

#endif /* NodeFFT_hpp */

```
