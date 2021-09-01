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

#include <CMSIS_DSP.h>

#ifndef NodeFFT_hpp
#define NodeFFT_hpp

namespace klang {
    class NodeFFT : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

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

        void update(SIGNAL_TYPE* pAudioBlock) {
            update(CH_OUT_SIGNAL, pAudioBlock);
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                flag_updated();
                std::copy(pAudioBlock,
                          pAudioBlock + KLANG_SAMPLES_PER_AUDIO_BLOCK, 
                          fft_input);
                transform_to_frequency_domain();
            }
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

        float get_frequency() {
            return maxIndex * frequency_resolution;
        }

        float* get_bands() {
            return fft_power;
        }

        const uint32_t get_number_of_bands() {
            return KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF;
        }

    private:
        static const uint32_t KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF = KLANG_SAMPLES_PER_AUDIO_BLOCK / 2;
        Connection*           mConnection_CH_IN_SIGNAL           = nullptr;
        float                 fft_input[KLANG_SAMPLES_PER_AUDIO_BLOCK];
        float                 fft_output[KLANG_SAMPLES_PER_AUDIO_BLOCK];
        float                 fft_power[KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF];
        float32_t             maxValue             = 0.0;
        uint32_t              maxIndex             = 0;
        uint8_t               ifftFlag             = 0;
        float                 frequency_resolution = (float)KLANG_AUDIO_RATE / (float)KLANG_SAMPLES_PER_AUDIO_BLOCK;

        void transform_to_frequency_domain() {
            /* analyze signal */
            arm_rfft_fast_instance_f32 fft;
            arm_rfft_fast_init_f32(&fft, KLANG_SAMPLES_PER_AUDIO_BLOCK);
            arm_rfft_fast_f32(&fft, fft_input, fft_output, ifftFlag);
            arm_cmplx_mag_f32(fft_output, fft_power, KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF);
            /* find dominant frequency */
            arm_max_f32(fft_power, KLANG_SAMPLES_PER_AUDIO_BLOCK_HALF, &maxValue, &maxIndex);
        }
    };
}  // namespace klang

#endif /* NodeFFT_hpp */
