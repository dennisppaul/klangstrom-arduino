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
 *       [ NODE_ALIEN_WAH      ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          Author or source: Nasca Octavian Paul (or.liame@acsanluap) from
 *          https://www.musicdsp.org/en/latest/Effects/70-alien-wah.html
 *       )
 */

#ifndef NodeAlienWah_hpp
#define NodeAlienWah_hpp

#include <complex.h>

#include "NodeKernelBlock.hpp"
#include "KlangNodes.hpp"
//#include <fcntl.h>
//#include <stdio.h>
//#include <math.h>

/*
 Alien-Wah by Nasca Octavian Paul from Tg. Mures, Romania
 e-mail:  <paulnasca@email.ro> or <paulnasca@yahoo.com>.
 */

/*
 The algorithm was found by me by mistake(I was looking for something else);
 I called this effect "Alien Wah" because sounds a bit like wahwah, but more strange.
 The ideea of this effect is very simple: It is a feedback delay who uses complex numbers.
 If x[] represents the input and y[] is the output, so a simple feedback delay looks like this:
 y[n]=y[n-delay]*fb+x[n]*(1-fb)
 
 'fb' is a real number between 0 and 1.
 If you change the fb with a complex number who has the MODULUS smaller than 1, it will look like this.
 
 fb=R*(cos(alpha)+i*sin(alpha));  i^2=-1; R<1;
 y[n]=y[n-delay]*R*(cos(alpha)+i*sin(alpha))+x[n]*(1-R);
 
 alpha is the phase of the number and is controlled by the LFO(Low Frequency Oscillator).
 If the 'delay' parameter is low, the effect sounds more like wah-wah,
 but if it is big, the effect will sound very interesting.
 The input x[n] has the real part of the samples from the wavefile and the imaginary part is zero.
 The output of this effect is the real part of y[n].
 
 Here it is a simple and unoptimised implementation of the effect. All parameters should be changed at compile time.
 It was tested only with Borland C++ 3.1.
 
 Please send me your opinions about this effect.
 Hope you like it (especially if you are play to guitar).
 Paul.
 */

/*
 Alien Wah Parameters
 
 freq       - "Alien Wah" LFO frequency
 startphase - "Alien Wah" LFO startphase (radians), needed for stereo
 fb         - "Alien Wah" FeedBack (0.0 - low feedback, 1.0 = 100% high feedback)
 delay      -  delay in samples at 44100 KHz (recomanded from 5 to 50...)
 */

namespace klang {
#define lfoskipsamples 25  // How many samples are processed before compute the lfo value again

    class NodeAlienWah : public NodeKernelBlock {
    public:
        NodeAlienWah() {
            init(0.6, 0, 0.5, 20);  //effects parameters
        }

    protected:
        void kernel(SIGNAL_TYPE* s) {
            float               lfo, out;
            std::complex<float> outc;
            for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                if (awint.t++ % lfoskipsamples == 0) {
                    lfo     = (1 + cos(awint.t * awint.lfoskip + awparams.startphase));
                    awint.c = std::complex<float>(cos(lfo) * awparams.fb, sin(lfo) * awparams.fb);
                };
                outc                    = awint.c * awint.delaybuf[awint.k] + (1 - awparams.fb) * s[i];
                awint.delaybuf[awint.k] = outc;
                if ((++awint.k) >= awparams.delay) {
                    awint.k = 0;
                }
                out = real(outc) * 3;  //take real part of outc
                                       //                if (out<-32768) {
                                       //                    out=-32768;
                                       //                } else if (out>32767) {
                                       //                    out=32767; //Prevents clipping
                                       //                }
                s[i] = out;
            }
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_FREQUENCY_F32:
                    set_frequency(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_FEEDBACK_F32:
                    set_feedback(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_DELAY_F32:
                    set_delay(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

        void set_frequency(float pFrequency) {
            awparams.freq = pFrequency;
            awint.lfoskip = pFrequency * 2 * 3.141592653589 / KLANG_AUDIO_RATE;
        }

        void set_feedback(float pFeedback) {
            awparams.fb = pFeedback / 4 + 0.74;
        }

        void set_delay(float pDelay) {
            if (pDelay < 1) {
                pDelay = 1;
            }
            awparams.delay = (int)(pDelay / 44100.0 * KLANG_AUDIO_RATE);
            awint.delaybuf = new std::complex<float>[awparams.delay];
            for (int i = 0; i < pDelay; i++) {
                awint.delaybuf[i] = std::complex<float>(0, 0);
            }
            awint.t = 0;
        }

    private:
        //alien wah internal parameters
        struct params {
            float freq;
            float startphase;
            float fb;
            int   delay;
        } awparams;

        struct alienwahinternals {
            std::complex<float>* delaybuf;
            float                lfoskip;
            long int             t;
            std::complex<float>  c;
            int                  k;
        } awint;

        void set_start_phase(float pStartphase) {
            awparams.startphase = pStartphase;
        }

        void init(float freq, float startphase, float fb, int delay) {
            set_frequency(freq);
            set_start_phase(startphase);
            set_feedback(fb);
            set_delay(delay);
        }
    };
}  // namespace klang

#endif /* NodeAlienWah_hpp */
