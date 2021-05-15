//
//  NodeVCOWavetable.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

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
 *       )
 *
 *       @set_value(FREQUENCY_F32,AMPLITUDE_F32,OFFSET_F32,WAVEFORM)
 */

#ifndef NodeVCOWavetable_hpp
#define NodeVCOWavetable_hpp

#include "NodeVCO.hpp"
#include "KlangMath.hpp"

#if (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_INT16)

#define OSCIL_F_BITS 16
#define OSCIL_F_BITS_AS_MULTIPLIER 65536
#define SCALE_WAVETABLE(x) ((x)*((1<<15)-1))
#define Q_FRAC                       15
#define FADD(a,b) ((a)+(b))
#define FSUB(a,b) ((a)-(b))
#define FMUL(a,b) (((a)*(b))>>(Q_FRAC))
#define FDIV(a,b) (((a)<<(Q_FRAC))/(b))

#elif (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_FLOAT)

#define SCALE_WAVETABLE(x) (x)

#endif

namespace klang {
    class NodeVCOWavetable : public NodeVCO {
    public:
        static const CHANNEL_ID NUM_CH_IN       = 2;
        static const CHANNEL_ID NUM_CH_OUT      = 1;
        
        enum WAVEFORM {
            SINE = 0,
            TRIANGLE,
            SAWTOOTH,
            SQUARE,
            EXPONENT
        };
        
        NodeVCOWavetable(uint16_t pBufferLength = 256) : M_BUFFER_LENGTH(pBufferLength) {
            mWavetable = new SIGNAL_TYPE[M_BUFFER_LENGTH]; // @TODO(consider reusing wavetable where possible)
            if (!isPowerOfTwo(pBufferLength)) {
                KLANG_LOG_ERR("### warning wavetable length needs to be *power of two*");
            }
            set_frequency(OSC_DEFAULT_FREQUENCY);
            set_amplitude(OSC_DEFAULT_AMPLITUDE);
        }
        
        ~NodeVCOWavetable() {
            delete[] mWavetable;
        }
        
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
        
        void static fill_wavetable(WAVEFORM pWaveform, SIGNAL_TYPE* pWavetable, const uint16_t pWavetableSize) {
            switch(pWaveform) {
                case SINE:
                {
                    for (uint16_t i=0;i<pWavetableSize;i++) {
                        pWavetable[i] = SCALE_WAVETABLE(sin(2.0 * PI * ((float)i/(float)(pWavetableSize))));
                    }
                    break;
                }
                case TRIANGLE:
                {
                    for(uint16_t i=0;i<(pWavetableSize/4);i++) {
                        pWavetable[i                     ]   =  SCALE_WAVETABLE( i / (float)(pWavetableSize/4));
                        pWavetable[i+(pWavetableSize*1/4)]   =  SCALE_WAVETABLE( ((pWavetableSize/4)-i) / (float)(pWavetableSize/4));
                        pWavetable[i+(pWavetableSize*2/4)]   =  SCALE_WAVETABLE(-i / (float)(pWavetableSize/4));
                        pWavetable[i+(pWavetableSize*3/4)]   =  SCALE_WAVETABLE(-((pWavetableSize/4)-i) / (float)(pWavetableSize/4));
                    }
                    break;
                }
                case SAWTOOTH:
                {
                    for(uint16_t i=0;i<pWavetableSize;i++) {
                        pWavetable[i] = SCALE_WAVETABLE(2.0*((float)i/(float)(pWavetableSize-1))-1.0);
                    }
                    break;
                }
                case SQUARE:
                {
                    for(uint16_t i=0;i<(pWavetableSize/2);i++) {
                        pWavetable[i]                    = SCALE_WAVETABLE( 1.0);
                        pWavetable[i+(pWavetableSize/2)] = SCALE_WAVETABLE(-1.0);
                    }
                    break;
                }
                case EXPONENT:
                {
                    /* symetric exponent similar to triangle */
                    float e = (float)exp(1.0f);
                    for(uint16_t i=0;i<(pWavetableSize/2);i++) {
                        pWavetable[i]                       = SCALE_WAVETABLE(2.0 * ((exp(i/(float)(pWavetableSize/2)) - 1.0) / (e - 1.0)) - 1.0);
                        pWavetable[i+(pWavetableSize/2)]    = SCALE_WAVETABLE(2.0 * ((exp(((pWavetableSize/2)-i)/(float)(pWavetableSize/2)) - 1.0) / (e - 1.0)) - 1.0);
                    }
                    break;
                }
                default:
                {
                    break;
                }
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
        
        void set_waveform(WAVEFORM pWaveform) {
            fill_wavetable(pWaveform, wavetable_ptr(), wavetable_size());
        }
        
        void set_waveform(uint8_t pWaveform) {
            fill_wavetable((WAVEFORM)pWaveform, wavetable_ptr(), wavetable_size());
        }
        
        void wavetable_write(uint16_t pIndex, SIGNAL_TYPE pSample) {
            mWavetable[pIndex] = pSample;
        }
        
        uint16_t wavetable_size() {
            return M_BUFFER_LENGTH;
        }
        
        SIGNAL_TYPE * wavetable_ptr() {
            return mWavetable;
        }
        
#if (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_INT16)
        /* +++++++++++++++++++++++++++++++++++++++++++++++++++ */
        /* ++++++++++++++ FIXED-POINT FUNCTIONS ++++++++++++++ */
        /* +++++++++++++++++++++++++++++++++++++++++++++++++++ */
        
        void set_frequency(SIGNAL_TYPE pFrequency) {
            phase_increment_fractional = ((WAVETABLE_t)pFrequency) * ((OSCIL_F_BITS_AS_MULTIPLIER * M_BUFFER_LENGTH) / KLANG_AUDIO_RATE_UINT16);
        }
        
        SIGNAL_TYPE get_frequency() { return 0; }
        
        void set_frequency_float(float pFrequency) {
            phase_increment_fractional = (WAVETABLE_t)((((float)M_BUFFER_LENGTH * pFrequency)/KLANG_AUDIO_RATE_UINT16) * OSCIL_F_BITS_AS_MULTIPLIER);
        }
        
        void set_amplitude_float(float pAmplitude) {
            mAmplitude = pAmplitude * SIGNAL_MAX;
        }
        
        void set_offset_float(float pOffset) {
            mOffset = pOffset * SIGNAL_MAX;
        }
        
        void set_input_FREQ_scale(uint16_t pInput_FREQ_Scale) {
            mInput_FREQ_Scale = pInput_FREQ_Scale;
        }
        
        void set_input_FREQ_offset(uint16_t pInput_FREQ_Offset) {
            mInput_FREQ_Offset = pInput_FREQ_Offset;
        }
#elif (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_FLOAT)
        /* +++++++++++++++++++++++++++++++++++++++++++++++++++ */
        /* +++++++++++++ FLOATING-POINT FUNCTIONS ++++++++++++ */
        /* +++++++++++++++++++++++++++++++++++++++++++++++++++ */
        
        void set_frequency(SIGNAL_TYPE pFrequency) {
            if (mFrequency != pFrequency) {
                mFrequency = pFrequency;
                mStepSize = mFrequency * ((float)M_BUFFER_LENGTH / (float)KLANG_AUDIO_RATE_UINT16); // @TODO maybe move this to constructor to do it only once
            }
        }
        
        SIGNAL_TYPE get_frequency() { return mFrequency; }
#endif
        
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
#if (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_INT16)
                    /* frequency */
                    if (mBlock_FREQ != AudioBlockPool::NO_ID) {
                        phase_increment_fractional = ((WAVETABLE_t)mInput_FREQ_Offset + mInput_FREQ_Scale * mBlockData_FREQ[i] / OSCIL_F_BITS_AS_MULTIPLIER) * ((WAVETABLE_t)OSCIL_F_BITS_AS_MULTIPLIER * M_BUFFER_LENGTH / KLANG_AUDIO_RATE_UINT16);
                    }
                    /* amp */
                    const SIGNAL_TYPE mAmpTmp = (mBlock_AMP != AudioBlockPool::NO_ID) ? mBlockData_AMP[i] : mAmplitude;
                    /* wavetable */
                    incrementPhase();
                    const uint16_t mIndex = ((phase_fractional >> OSCIL_F_BITS) & (M_BUFFER_LENGTH - 1));
                    mBlock[i] = FADD(FMUL(mWavetable[mIndex], mAmpTmp), mOffset);
#elif (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_FLOAT)
                    /* frequency */
                    if (mBlock_FREQ != AudioBlockPool::NO_ID) {
                        set_frequency(mBlockData_FREQ[i]);
                    }
                    /* wavetable */
                    //                    mArrayPtr += mStepSize;
                    //                    wrap_array_ptr();
                    //                    const uint16_t mIndex = (uint16_t)mArrayPtr;
                    //                    // @TODO(check if this is faster `Util::wrap_float_index(mArrayPtr + mStepSize, M_BUFFER_LENGTH);`)
                    mArrayPtr += mStepSize;
                    const uint16_t mInt = (uint16_t)mArrayPtr;
                    const float mFrac = mArrayPtr - mInt;
                    const uint16_t mIndex = mInt & (M_BUFFER_LENGTH - 1);
                    mArrayPtr = mIndex + mFrac;
#define INTERPOLATE_WAVETABLE_SAMPLES 1
#if INTERPOLATE_WAVETABLE_SAMPLES
                    //                    const float mFrac = mArrayPtr - mIndex;
                    //                    const uint16_t mIndexNext = (mIndex + 1) % M_BUFFER_LENGTH;
                    // @TODO(check if this is faster `(mIndex + 1) & (M_BUFFER_LENGTH - 1);`)
                    const uint16_t mIndexNext = (mIndex + 1) & (M_BUFFER_LENGTH - 1);
                    pAudioBlock[i] = mWavetable[mIndex] * (1.0 - mFrac) + mWavetable[mIndexNext] * mFrac;
#else
                    pAudioBlock[i] = mWavetable[mIndex];
#endif
                    pAudioBlock[i] *= (mBlock_AMP != AudioBlockPool::NO_ID) ? mBlockData_AMP[i] : mAmplitude;
                    pAudioBlock[i] += mOffset;
#endif
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
#if (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_INT16)
        typedef int32_t WAVETABLE_t;
        WAVETABLE_t phase_fractional;
        WAVETABLE_t phase_increment_fractional;
        
        uint16_t mInput_FREQ_Scale  = 1;
        uint16_t mInput_FREQ_Offset = 0;
        
        inline void incrementPhase() {
            phase_fractional += phase_increment_fractional;
        }
#elif (KLANG_SIGNAL_TYPE==SIGNAL_TYPE_FLOAT)
        SIGNAL_TYPE mFrequency      = 0.0;
        float mStepSize             = 0.0;
        float mArrayPtr             = 0.0;
        
        void wrap_array_ptr() {
            while (mArrayPtr >= M_BUFFER_LENGTH) {
                mArrayPtr -= M_BUFFER_LENGTH;
            }
        }
#endif
        
        SIGNAL_TYPE* mWavetable;
        const uint16_t M_BUFFER_LENGTH;
        
        bool isPowerOfTwo(uint16_t x) {
            while (((x % 2) == 0) && x > 1) {
                x /= 2;
            }
            return (x == 1);
        }
        
        AUDIO_BLOCK_ID mBlock_FREQ  = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_AMP   = AudioBlockPool::NO_ID;
        
        Connection* mConnection_CH_IN_FREQ  = nullptr;
        Connection* mConnection_CH_IN_AMP   = nullptr;
        
        SIGNAL_TYPE mAmplitude      = SIGNAL_MAX;
        SIGNAL_TYPE mOffset         = 0.0;
    };
}

#endif /* NodeVCOWavetable_hpp */