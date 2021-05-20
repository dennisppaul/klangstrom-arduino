//
//  NodeChorus.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_CHORUS         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL  SIGNAL_LEFT |--OUT00
 *       |        SIGNAL_RIGHT |--OUT01
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeChorus_hpp
#define NodeChorus_hpp

#include "Node.hpp"
#include "LUTSine.hpp"

namespace klang {
    class NodeChorus : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN           = 1;
        static const CHANNEL_ID CH_OUT_SIGNAL_LEFT  = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_OUT          = 2;
        
        NodeChorus() {
            Chorus_init();
        }
        
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
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    AUDIO_BLOCK_ID mBlock_IN_SIGNAL = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SIGNAL->update(mBlock_IN_SIGNAL);
                    /* process and store output blocks */
                    SIGNAL_TYPE* mBlockData_IN_SIGNAL       = AudioBlockPool::instance().data(mBlock_IN_SIGNAL);
                    mBlock_OUT_SIGNAL_L                     = AudioBlockPool::instance().request();
                    SIGNAL_TYPE* mBlockData_OUT_SIGNAL_L    = AudioBlockPool::instance().data(mBlock_OUT_SIGNAL_L);
                    mBlock_OUT_SIGNAL_R                     = AudioBlockPool::NO_ID;
                    SIGNAL_TYPE* mBlockData_OUT_SIGNAL_R    = nullptr;
                    if (mStereoOutput) {
                        mBlock_OUT_SIGNAL_R = AudioBlockPool::instance().request();
                        mBlockData_OUT_SIGNAL_R = AudioBlockPool::instance().data(mBlock_OUT_SIGNAL_R);
                    }
                    for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        mBlockData_OUT_SIGNAL_L[i] = mono_chorus_compute(&delL, &lfoL, mBlockData_IN_SIGNAL[i]);
                        if (mStereoOutput) {
                            mBlockData_OUT_SIGNAL_R[i] = mono_chorus_compute(&delR, &lfoR, mBlockData_IN_SIGNAL[i]);
                        }
                    }
                    AudioBlockPool::instance().release(mBlock_IN_SIGNAL);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL_LEFT && mBlock_OUT_SIGNAL_L != AudioBlockPool::NO_ID) {
                SIGNAL_TYPE* mBlockData_OUT_SIGNAL_L = AudioBlockPool::instance().data(mBlock_OUT_SIGNAL_L);
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBlockData_OUT_SIGNAL_L[i];
                }
                // @TODO(should probably use `std::copy` here
                // memcpy( mBlockData_OUT_SIGNAL_L,
                //         pAudioBlock,
                //         sizeof(SIGNAL_TYPE) * KLANG_SAMPLES_PER_AUDIO_BLOCK);
            } else if (pChannel == CH_OUT_SIGNAL_RIGHT && mStereoOutput && mBlock_OUT_SIGNAL_R != AudioBlockPool::NO_ID) {
                SIGNAL_TYPE* mBlockData_OUT_SIGNAL_R = AudioBlockPool::instance().data(mBlock_OUT_SIGNAL_R);
                for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = mBlockData_OUT_SIGNAL_R[i];
                }
            }
        }
        
        void set_stereo(bool pStereoOutput) {
            mStereoOutput = pStereoOutput;
        }
        
        void set_feedback(SIGNAL_TYPE pValue) {
            ChorusFeedback_set(pValue);
        }
        
        void set_rate(SIGNAL_TYPE pValue) {
            ChorusRate_set(pValue);
        }
        
        void set_delay(SIGNAL_TYPE pValue) {
            ChorusDelay_set(pValue);
        }
        
        void set_sweep(SIGNAL_TYPE pValue) {
            ChorusSweep_set(pValue);
        }
        
        void toggle_mode() {
            ChorusMode_toggle();
        }
        
        void set_mode(bool pValue) {
            ChorusMode_switch(pValue ? 127 : 0);
        }
        
        void toggle_feedback_sign() {
            ChorusFDBsign_change();
        }

        void set_feedback_sign(bool pValue) {
            ChorusFDBsign_switch(pValue ? 127 : 0);
        }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_STEREO_I8:
                    set_stereo(pPayLoad[0]);
                    break;
                case KLANG_SET_FEEDBACK_F32:
                    set_feedback(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_DELAY_F32:
                    set_delay(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_RATE_F32:
                    set_rate(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_SWEEP_F32:
                    set_sweep(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }
        
    private:
        AUDIO_BLOCK_ID mBlock_OUT_SIGNAL_L  = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_OUT_SIGNAL_R  = AudioBlockPool::NO_ID;
        
        Connection* mConnection_CH_IN_SIGNAL = nullptr;
        
        bool mStereoOutput = true;
        
        /*------------------------------------*/
        
#define DEPTH               1400 // Size of delay buffer, in samples : 29.17 ms
#define LEFT_DELAY          3*240 /*  initial left delay */
#define RIGHT_DELAY         3*240 /*  initial right delay */
#define LEFT_SWEEP          2*50
#define RIGHT_SWEEP         2*50
#define MAX_RATE            7.f // in Hz
#define MIN_RATE            0.02f // in Hz
#define LEFT_RATE           0.11f // in Hz
#define RIGHT_RATE          0.12f // in Hz
#define FEEDBACK            2*-0.2f // look at the diagram
#define FORWARD             0.5f
#define MIX                 0.3f
        
        float Fs = ((float)(KLANG_AUDIO_RATE_UINT16));    // samplerate
        float Ts = (1.f/Fs);  // sample period
        
        /*------------------------------------*/
        typedef struct
        {
            float     amp;
            float     freq;
            float     phase;
            float     out;
            
        } Lfo_t;
        
        /*------------------------------------*/
        
        typedef struct
        {
            float        mix;               /* delay blend parameter */
            float        fb;               /* feedback volume */
            float        fw;               /* delay tap mix volume */
            int32_t      in_idx;            /* delay write index */
            float        dline[DEPTH] ;    /* delay buffer */
            float        baseDelay;        /* tap position */
            int8_t       mode;             /* constant or variable delayed feedback ? */
            
        } monochorus_t ;
        
        Lfo_t           lfoL;
        Lfo_t           lfoR; // 2 independant LFOs
        monochorus_t    delR;
        monochorus_t    delL; // 2 fractional delay lines
        float           rateCoeff;
        
        /*------------------------------------*/
        
#define MARGIN            6 // minimal delay (in samples)
        
        /*************************************************************************************************
         *                       chorus/flanger diagram (one channel) :
         *
         *                    ---------[mix >----------------------------
         *                    |                                         |
         *                    |                                         |
         *                    |x1                                       v
         *     xin ------>[+]----->[z^-M]--[interp.]----[fw >--------->[+]-----> yout
         *                 ^         delay line      |
         *                 |                         |
         *                 --< fb]<-------------------
         *
         ************************************************************************************************/
        
        void Chorus_init() {
            ChorusDelay_init(&delL, LEFT_DELAY, FEEDBACK, FORWARD, MIX);
            ChorusDelay_init(&delR, RIGHT_DELAY, FEEDBACK, FORWARD, MIX);
            lfoL.amp = LEFT_SWEEP;
            lfoR.amp = RIGHT_SWEEP;
            lfoL.freq = LEFT_RATE ;
            lfoR.freq = RIGHT_RATE ;
            rateCoeff = 1;
            lfoL.phase = PI/2; // initial phases for quadrature
            lfoR.phase = 0;
            
            for (int var = 0; var < DEPTH; ++var) {
                delR.dline[var] = 0;
                delL.dline[var] = 0;
            }
        }
        
        /*-------------------------------------------------------------------------------------------*/
        float Lfo_SampleCompute(Lfo_t * op) // ! returns a positive value between 0 and op.amp !
        {
            float z;
            
            op->phase += TWO_PI * Ts * op->freq; // increment phase
            while (op->phase < 0) // keep phase in [0, 2pi]
                op->phase += TWO_PI;
            while (op->phase >= TWO_PI)
                op->phase -= TWO_PI;
            
            z = LUTSine::WAVETABLE[lrintf(LUTSine::ALPHA * (op->phase))];
            op->out = op->amp * (z + 1); // returns a positive value between 0 and op.amp
            
            return op->out;
        }
        
        /*---------------------------------------------------------------------------------------------*/
        void ChorusDelay_init(monochorus_t *del, float delay, float dfb, float dfw, float dmix)
        {
            Delay_set_fb(del, dfb);
            Delay_set_fw(del, dfw);
            Delay_set_mix(del, dmix);
            Delay_set_delay(del, delay);
            del->in_idx = DEPTH - 1;// Place the input pointer at the end of the buffer
            del->mode = 1;
        }
        
        /*---------------------------------------------------------------------------------------------*/
        void Chorus_reset() {
            Chorus_init();
        }
        /*-------------------------------------------------------------------------------------------*/
        void inc_chorusRate(void)
        {
            lfoL.freq *= 1.05f;
            lfoR.freq *= 1.05f;
        }
        /*-------------------------------------------------------------------------------------------*/
        void dec_chorusRate(void)
        {
            lfoL.freq *= .95f;
            lfoR.freq *= .95f;
        }
        /*---------------------------------------------------------------------------------------------*/
        void ChorusRate_set(float val)
        {
            float rate;
            rate = MAX_RATE * val + MIN_RATE;
            lfoL.freq = rate;
            //lfoR.freq = 0.98f * rate;
            lfoR.freq = rateCoeff * rate;
        }
        /*---------------------------------------------------------------------------------------------*/
        void ChorusSecondRate_set(float val)
        {
            rateCoeff = 0.9f * val + 0.1f; // from 10 % to 100 % of lfoL rate
            lfoR.freq = rateCoeff * lfoL.freq;
        }
        /*-------------------------------------------------------------------------------------------*/
        void inc_chorusDelay(void)
        {
            float d;
            
            d = delL.baseDelay * 1.1f;
            if (d < DEPTH)
            {
                delL.baseDelay = d;
            }
            
            d = delR.baseDelay * 1.1f;
            if (d < DEPTH)
            {
                delR.baseDelay = d;
            }
        }
        /*-------------------------------------------------------------------------------------------*/
        void dec_chorusDelay(void)
        {
            delL.baseDelay *= .9f;
            delR.baseDelay *= .9f;
        }
        /*---------------------------------------------------------------------------------------------*/
        void ChorusDelay_set(float val)
        {
            float d;
            d = 0.5f * DEPTH * val ;
            delL.baseDelay = d;
            delR.baseDelay = d;
        }
        /*-------------------------------------------------------------------------------------------*/
        void inc_chorusFeedback(void)
        {
            /* increase feedback delay */
            
            delL.fb *= 1.02f ;//
            delR.fb *= 1.02f ;//
        }
        /*-------------------------------------------------------------------------------------------*/
        void dec_chorusFeedback(void)
        {
            /* decrease feedback delay */
            
            delL.fb *= 0.95f ;//
            delR.fb *= 0.95f ;//
        }
        /*---------------------------------------------------------------------------------------------*/
        void ChorusFeedback_set(float val)
        {
            float_t fb;
            fb = 0.95f * val;
            delL.fb = fb;
            delR.fb = fb;
        }
        /*-------------------------------------------------------------------------------------------*/
        void inc_chorusSweep(void)
        {
            lfoL.amp *= 1.05f ;//
            lfoR.amp *= 1.05f ;//
        }
        /*-------------------------------------------------------------------------------------------*/
        void dec_chorusSweep(void)
        {
            lfoL.amp *= .95f ;//
            lfoR.amp *= .95f ;//
        }
        /*---------------------------------------------------------------------------------------------*/
        void ChorusSweep_set(float val)
        {
            float sw;
            sw = 0.1f * (DEPTH - MARGIN) * val;
            lfoL.amp = sw;
            lfoR.amp = sw;
        }
        /*-------------------------------------------------------------------------------------------*/
        void ChorusMode_toggle(void)
        {
            delL.mode *= -1 ;//
            delR.mode *= -1 ;//
        }
        /*---------------------------------------------------------------------------------------------*/
        void ChorusMode_switch(uint8_t val)
        {
            switch (val)
            {
                case 127 :     delR.mode = delL.mode = -1 ; break;
                case 0 :     delR.mode = delL.mode = 1 ; break;
            }
        }
        /*-------------------------------------------------------------------------------------------*/
        void ChorusFDBsign_change(void)
        {
            delL.fb *= -1.f ;//
            delR.fb *= -1.f ;//
        }
        /*---------------------------------------------------------------------------------------------*/
        void ChorusFDBsign_switch(uint8_t val)
        {
            switch (val)
            {
                case 127 :     delR.fb = fabsf(delR.fb); delL.fb = fabsf(delL.fb); break; // positive feedback;
                case 0 :     delR.fb = - fabsf(delR.fb); delL.fb = - fabsf(delL.fb); break; // negative feedback;
            }
        }
        /*-------------------------------------------------------------------------------------------*/
        void Delay_set_delay(monochorus_t *del, float delay)
        {
            del->baseDelay = delay;
        }
        /*-------------------------------------------------------------------------------------------*/
        void Delay_set_fb(monochorus_t *del, float val)
        {
            del->fb = val;
        }
        /*-------------------------------------------------------------------------------------------*/
        void Delay_set_fw(monochorus_t *del, float val)
        {
            del->fw = val;
        }
        /*-------------------------------------------------------------------------------------------*/
        void Delay_set_mix(monochorus_t *del, float val)
        {
            del->mix = val;
        }
        /*-------------------------------------------------------------------------------------------*/
        float Delay_get_fb(monochorus_t *del )
        {
            return del->fb;
        }
        /*-------------------------------------------------------------------------------------------*/
        float Delay_get_fw(monochorus_t *del )
        {
            return del->fw;
        }
        /*-------------------------------------------------------------------------------------------*/
        float Delay_get_mix(monochorus_t *del )
        {
            return del->mix;
        }
        /*-------------------------------------------------------------------------------------------*/
        void delay_write (monochorus_t *del, float xin)
        {
            del->dline[del->in_idx] = xin;
            
            (del->in_idx)++;
            if (del->in_idx >= DEPTH)
                del->in_idx = 0;
        }
        /*-------------------------------------------------------------------------------------------*/
        float delay_read (monochorus_t *del, float delay) // "delay" is a floating point number of samples
        {
            float d;        // true requested delay (floating point number of samples)
            float f;        // fractional part of delay
            int32_t i;        // integer part of delay
            float y_n;        // y(n)
            float y_n_1;    // y(n-1)
            float y_n_2;    // y(n-2)
            float y_n_3;    // y(n-3)
            int32_t idx;
            
            d = delay;
            if (d < MARGIN) d = MARGIN; // MARGIN is the minimum allowed delay
            if (d > DEPTH-MARGIN) d = DEPTH-MARGIN;
            
            i = (int32_t)floorf(d);
            f = d - i;
            
            idx = del->in_idx - i;
            if (idx < 0) idx += DEPTH;
            y_n = del->dline[idx];         // y(n)
            
            idx--;
            if (idx < 0) idx += DEPTH;
            y_n_1 = del->dline[idx];    // y(n-1)
            
            idx--;
            if (idx < 0) idx += DEPTH;
            y_n_2 = del->dline[idx];    // y(n-2)
            
            idx--;
            if (idx < 0) idx += DEPTH;
            y_n_3 = del->dline[idx];    // y(n-3)
            
            //return (y_n_1 - y_n) * f + y_n ; // linear interpolation
            
            //return (.5f)*(f-1)*(f-2)*y_n - f*(f-2)*y_n_1 + (.5f)*f*(f-1)*y_n_2 ; // 2nd order Lagrange interpolation
            
            //return .5f*(f-1)*((f-2)*y_n + f*y_n_2) - f*(f-2)*y_n_1 ;    // 2nd order Lagrange interpolation (faster)
            
            /* 3rd order Lagrange interpolation :  */
            return (f-2)*(f-3)*(-0.16666666666f *(f-1)*y_n + 0.5f * f * y_n_1) + f*(f-1)*(-0.5f * (f-3)*y_n_2 + 0.166666666666f * (f-2)*y_n_3);
            
        }
        /*---------------------------------------------------------------------------------------------*/
        
        float mono_chorus_compute(monochorus_t *del, Lfo_t *lfo, float xin)
        {
            float yout;
            float x1;
            float x2;
            
            x2 = delay_read (del, del->baseDelay + Lfo_SampleCompute(lfo) + MARGIN);
            
            if (del->mode == 1)
                x1 = xin + del->fb * x2; // variable delay feedback signal or ...
            else
                x1 = xin + del->fb * delay_read (del, del->baseDelay + MARGIN); // fixed delay feedback signal
            
            x1 = (x1 > 1.0f) ? 1.0f : x1 ; //clip too loud samples
            x1 = (x1 < -1.0f) ? -1.0f : x1 ;
            
            yout = del->mix * x1 + del->fw * x2;
            //yout = del->mix * xin + del->fw * x2; // not good sounding...
            delay_write(del, x1);
            
            return yout;
        }
        
        /*--------------------This is the main stereo chorus function : ----------------------------*/
        
        void stereoChorus_compute (float * left_out, float * right_out, float in)
        {
            *left_out = mono_chorus_compute(&delL, &lfoL, in);
            *right_out = mono_chorus_compute(&delR, &lfoR, in);
        }
    };
}

#endif /* NodeChorus_hpp */
