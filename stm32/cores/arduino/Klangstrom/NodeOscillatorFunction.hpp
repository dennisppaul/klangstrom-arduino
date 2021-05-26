//
//  NodeOscillatorFunction.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_OSC_FUNC       ]
 *       +---------------------+
 *       |                     |
 * IN00--| FREQ         SIGNAL |--OUT00
 * IN01--| AMP                 |
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *       )
 */

#ifndef NodeOscillatorFunction_hpp
#define NodeOscillatorFunction_hpp

#include "NodeOscillatorInterface.hpp"
#include "KlangMath.hpp"

namespace klang {
    class NodeVCOFunction : public NodeOscillatorInterface {
    public:
        
        enum WAVEFORM {
            SINE = 0,
            TRIANGLE,
            SAWTOOTH,
            SQUARE
        };
        
        static const CHANNEL_ID NUM_CH_IN       = 2; // defined in `NodeOscillator`
        static const CHANNEL_ID NUM_CH_OUT      = 1;
        
        NodeVCOFunction() {
            set_frequency(OSC_DEFAULT_FREQUENCY);
            set_amplitude(OSC_DEFAULT_AMPLITUDE);
        }
        
        void set_waveform(WAVEFORM pWaveform) { mWaveform = pWaveform; }
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_FREQ) {
                mConnection_CH_IN_FREQ = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_AMP) {
                mConnection_CH_IN_AMP = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_FREQ) {
                mConnection_CH_IN_FREQ = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_AMP) {
                mConnection_CH_IN_AMP = nullptr;
                return true;
            }
            return false;
        }
        
        void set_value(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayload) {
            switch (pCommand) {
                case KLANG_SET_FREQUENCY_F32:
                {
                    const float mData = KlangMath::FLOAT_32(pPayload);
                    set_frequency(mData);
                }
                    break;
                case KLANG_SET_AMPLITUDE_F32:
                {
                    const float mData = KlangMath::FLOAT_32(pPayload);
                    set_amplitude(mData);
                }
                    break;
            }
        }
        
        void set_amplitude(SIGNAL_TYPE pAmplitude) {
            mAmplitude = pAmplitude;
        }
        
        SIGNAL_TYPE get_amplitude() {
            return mAmplitude;
        }
        
        void set_offset(SIGNAL_TYPE pOffset) {
            mOffset = pOffset;
        }
        
        SIGNAL_TYPE get_offset() {
            return mOffset;
        }
        
        void set_frequency(SIGNAL_TYPE pFrequency) {
            if (mFrequency != pFrequency) {
                mFrequency = pFrequency;
                mStepSize = mFrequency * TWO_PI / KLANG_AUDIO_RATE_UINT16;
            }
        }
        
        SIGNAL_TYPE get_frequency() { return mFrequency; }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
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
                SIGNAL_TYPE* mBlockData_FREQ = nullptr;
                if (mBlock_FREQ != AudioBlockPool::NO_ID) {
                    mBlockData_FREQ = AudioBlockPool::instance().data(mBlock_FREQ);
                }
                SIGNAL_TYPE* mBlockData_AMP = nullptr;
                if (mBlock_AMP != AudioBlockPool::NO_ID) {
                    mBlockData_AMP = AudioBlockPool::instance().data(mBlock_AMP);
                }
                
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    if (mBlock_FREQ != AudioBlockPool::NO_ID) {
                        set_frequency(mBlockData_FREQ[i]);
                    }
                    switch(mWaveform) {
                        case SINE:
                            process_sine(i, pAudioBlock);
                            break;
                        case TRIANGLE:
                            process_triangle(i, pAudioBlock);
                            break;
                        case SAWTOOTH:
                            process_sawtooth(i, pAudioBlock);
                            break;
                        case SQUARE:
                            process_square(i, pAudioBlock);
                            break;
                    }
                    pAudioBlock[i] *= (mBlock_AMP != AudioBlockPool::NO_ID) ? mBlockData_AMP[i] : mAmplitude;
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
        SIGNAL_TYPE mFrequency      = 0.0;
        float mStepSize             = 0.0;
        double mPhase               = 0.0; // @NOTE("single precision introduces drift")
        SIGNAL_TYPE mAmplitude      = SIGNAL_MAX;
        SIGNAL_TYPE mOffset         = 0.0;
        WAVEFORM mWaveform          = WAVEFORM::SINE;
        
        AUDIO_BLOCK_ID mBlock_FREQ  = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_AMP   = AudioBlockPool::NO_ID;
        Connection* mConnection_CH_IN_FREQ  = nullptr;
        Connection* mConnection_CH_IN_AMP   = nullptr;
        
        
        void process_sine(uint16_t i, SIGNAL_TYPE *pAudioBlock) {
            mPhase += mStepSize;
            if (mPhase > TWO_PI) {
                mPhase -= TWO_PI;
            }
            pAudioBlock[i] = klang_math_sin(mPhase);
        }
        
        void process_triangle(uint16_t i, SIGNAL_TYPE *pAudioBlock) {
            mPhase += mFrequency;
            mPhase = KlangMath::mod(mPhase, KLANG_AUDIO_RATE);
            const float mPhaseShifted = mPhase - (KLANG_AUDIO_RATE/2);
            const float mPhaseShiftedAbs = mPhaseShifted > 0 ? mPhaseShifted : -mPhaseShifted;
            const float a = KLANG_AUDIO_RATE/4;
            const float b = mPhaseShiftedAbs - a;
            pAudioBlock[i] = b / a;
        }
        
        void process_sawtooth(uint16_t i, SIGNAL_TYPE *pAudioBlock) {
            mPhase += mFrequency;
            mPhase = KlangMath::mod(mPhase, KLANG_AUDIO_RATE);
            const float a = KLANG_AUDIO_RATE/2;
            const float b = mPhase / a;
            pAudioBlock[i] = b + SIGNAL_MIN;
        }
        
        void process_square(uint16_t i, SIGNAL_TYPE *pAudioBlock) {
            mPhase += mFrequency;
            mPhase = KlangMath::mod(mPhase, KLANG_AUDIO_RATE);
            pAudioBlock[i] = mPhase > (KLANG_AUDIO_RATE/2) ? SIGNAL_MAX : SIGNAL_MIN;
        }
    };
}

#endif /* NodeOscillatorFunction_hpp */
