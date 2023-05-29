/*
 * Wellen
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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
 * PROCESSOR INTERFACE
 *
 * - [ ] float process()
 * - [ ] float process(float)‌
 * - [ ] void process(Signal&)
 * - [ ] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 * - [x] float process(float, float)‌
 * - [x] void process(float*, float*, float*, uint32_t)
 *
 */

#pragma once

#include <stdint.h>

#include "KlangWellen.h"

namespace klangwellen {
    /**
     * superimposes a modulator signal ( e.g a human voice ) onto a carrier signal ( e.g sawtooth oscillator ).
     * <p>
     * *voclib* is an implementation of a traditional channel vocoder by Philip Bennefall from
     * https://github.com/blastbay/voclib.
     */

    class Vocoder {
    public:
        /* LICENSE

         This software is available under 2 licenses -- choose whichever you prefer.
         ------------------------------------------------------------------------------
         ALTERNATIVE A - MIT No Attribution License
         Copyright (c) 2019 Philip Bennefall

         Permission is hereby granted, free of charge, to any person obtaining a copy of
         this software and associated documentation files (the "Software"), to deal in
         the Software without restriction, including without limitation the rights to
         use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
         of the Software, and to permit persons to whom the Software is furnished to do
         so.

         THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
         IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
         FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
         LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
         OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
         SOFTWARE.
         ------------------------------------------------------------------------------
         ALTERNATIVE B - Public Domain (www.unlicense.org)
         This is free and unencumbered software released into the  domain.
         Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
         software, either in source code form or as a compiled binary, for any purpose,
         commercial or non-commercial, and by any means.

         In jurisdictions that recognize copyright laws, the author or authors of this
         software dedicate any and all copyright interest in the software to the
         domain. We make this dedication for the benefit of the  at large and to
         the detriment of our heirs and successors. We intend this dedication to be an
         overt act of relinquishment in perpetuity of all present and future rights to
         this software under copyright law.
         THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
         IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
         FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
         AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
         ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
         WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
         ------------------------------------------------------------------------------
         */

        /* Filters
         *
         * The filter code below was derived from http://www.musicdsp.org/files/biquad.c. The comment at the top of biquad.c
         * file reads:
         *
         * Simple implementation of Biquad filters -- Tom St Denis
         *
         * Based on the work
         *
         *      Cookbook formulae for audio EQ biquad filter coefficients
         *      ---------------------------------------------------------
         *      by Robert Bristow-Johnson, pbjrbj@viconet.com  a.k.a. robert@audioheads.com
         *
         * Available on the web at
         *
         *     http://www.smartelectronix.com/musicdsp/text/filters005.txt
         *
         * Enjoy.
         *
         * This work is hereby placed in the  domain for all purposes, whether
         * commercial, free [as in speech] or educational, etc.  Use the code and please
         * give me credit if you wish.
         *
         * Tom St Denis -- http://tomstdenis.home.dhs.org
         */

        /* Initialize a instance structure.
         *
         * Call this function to initialize the instance structure.
         * bands is the number of bands that the vocoder should use; recommended values are between 12 and 64.
         * bands must be between 4 and VOCLIB_MAX_BANDS (inclusive).
         * filters_per_band determines the steapness with which the filterbank divides the signal; a value of 6 is
         * recommended.
         * filters_per_band must be between 1 and VOCLIB_MAX_FILTERS_PER_BAND (inclusive).
         * sample_rate is the number of samples per second in hertz, and should be between 8000 and 192000 (inclusive).
         * Note: The modulator must always have only one channel.
         */
        Vocoder(uint8_t  pBands          = 24,
                uint8_t  pFiltersPerBand = 4,
                uint32_t pSampleRate     = KlangWellen::DEFAULT_SAMPLING_RATE) : bands(pBands),
                                                                             filters_per_band(pFiltersPerBand),
                                                                             sample_rate(pSampleRate) {
            // if (pBands < 4 || pBands > VOCLIB_MAX_BANDS) {
            //     // System.out.println("ERROR @" + Vocoder.class.getSimpleName() + " / bands: " + pBands);
            // }
            // if (pFiltersPerBand < 1 || pFiltersPerBand > VOCLIB_MAX_FILTERS_PER_BAND) {
            //     // System.out.println("ERROR @" + Vocoder.class.getSimpleName() + " / filters per band: " + pFiltersPerBand);
            // }
            // if (pSampleRate < 8000 || pSampleRate > 192000) {
            //     // System.out.println("ERROR @" + Vocoder.class.getSimpleName() + " / sample rate: " + pSampleRate);
            // }
            // if (pCarrierChannels < 1 || pCarrierChannels > 2) {
            //     // System.out.println("ERROR @" + Vocoder.class.getSimpleName() + " / carrier channels: " + pCarrierChannels);
            // }

            reaction_time = 0.03;
            formant_shift = 1.0;

            reset_history();
            initialize_filterbank(false);
            initialize_envelopes();

            fRectifyVolume = 1.0;
        }

        void set_volume(float pRectifyVolume) {
            fRectifyVolume = pRectifyVolume;
        }

        /* Run the vocoder.
         *
         * Call this function continuously to generate your output.
         * carrier_buffer and modulator_buffer should contain the carrier and modulator signals respectively.
         * The modulator must always have one channel.
         * If the carrier has two channels, the samples in carrier_buffer must be interleaved.
         * output_buffer will be filled with the result, and must be able to hold as many channels as the carrier.
         * If the carrier has two channels, the output buffer will be filled with interleaved samples.
         * output_buffer may be the same pointer as either carrier_buffer or modulator_buffer as long as it can hold the
         * same number of channels as the carrier.
         * The processing is performed in place.
         * frames specifies the number of sample frames that should be processed.
         * Returns nonzero (true) on success or 0 (false) on failure.
         * The function will only fail if one or more of the parameters are invalid.
         */
        void process(float*         carrier_buffer,
                     float*         modulator_buffer,
                     float*         output_buffer,
                     const uint32_t frames = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            /* Both the carrier and the modulator have a single channel. */
            for (uint32_t i = 0; i < frames; ++i) {
                float out = 0.0f;

                /* Run the bands in parallel and accumulate the output. */
                for (uint8_t j = 0; j < bands; ++j) {
                    float analysis_band  = BiQuad(modulator_buffer[i], analysis_bands[j].filters[0]);
                    float synthesis_band = BiQuad(carrier_buffer[i], synthesis_bands[j].filters[0]);

                    for (uint8_t k = 1; k < filters_per_band; ++k) {
                        analysis_band  = BiQuad(analysis_band, analysis_bands[j].filters[k]);
                        synthesis_band = BiQuad(synthesis_band, synthesis_bands[j].filters[k]);
                    }
                    analysis_band = envelope_tick(analysis_envelopes[j], analysis_band);
                    out += synthesis_band * analysis_band;
                }
                output_buffer[i] = out * fRectifyVolume;
            }
        }

        void process(float*         carrier_buffer_left,
                     float*         carrier_buffer_right,
                     float*         modulator_buffer,
                     float*         output_buffer_left,
                     float*         output_buffer_right,
                     const uint32_t frames = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            /* The carrier has two channels and the modulator has 1. */
            for (uint32_t i = 0; i < frames; i++) {
                float out_left  = 0.0f;
                float out_right = 0.0f;

                /* Run the bands in parallel and accumulate the output. */
                for (uint8_t j = 0; j < bands; ++j) {
                    float analysis_band        = BiQuad(modulator_buffer[j], analysis_bands[j].filters[0]);
                    float synthesis_band_left  = BiQuad(carrier_buffer_left[i], synthesis_bands[j].filters[0]);
                    float synthesis_band_right = BiQuad(carrier_buffer_right[i], synthesis_bands[j + VOCLIB_MAX_BANDS].filters[0]);

                    for (uint8_t k = 1; k < filters_per_band; ++k) {
                        analysis_band        = BiQuad(analysis_band, analysis_bands[j].filters[k]);
                        synthesis_band_left  = BiQuad(synthesis_band_left, synthesis_bands[j].filters[k]);
                        synthesis_band_right = BiQuad(synthesis_band_right, synthesis_bands[j + VOCLIB_MAX_BANDS].filters[k]);
                    }
                    analysis_band = envelope_tick(analysis_envelopes[j], analysis_band);
                    out_left += synthesis_band_left * analysis_band;
                    out_right += synthesis_band_right * analysis_band;
                }
                output_buffer_left[i]  = out_left * fRectifyVolume;
                output_buffer_right[i] = out_right * fRectifyVolume;
            }
        }

        /* Set the formant shift of the vocoder in octaves.
         *
         * Formant shifting changes the size of the speaker's head.
         * A value of 1.0 leaves the head size unmodified.
         * Values lower than 1.0 make the head larger, and values above 1.0 make it smaller.
         * The value must be between 0.25 and 4.0 (inclusive).
         * Returns nonzero (true) on success or 0 (false) on failure.
         * The function will only fail if the parameter is invalid.
         */
        uint8_t set_formant_shift(float pFormant_shift) {
            if (formant_shift < 0.25f || formant_shift > 4.0f) {
                return 0;
            }

            formant_shift = pFormant_shift;
            initialize_filterbank(true);
            return 1;
        }

        /* Reset the vocoder sample history.
         *
         * In order to run smoothly, the vocoder needs to store a few recent samples internally.
         * This function resets that internal history. This should only be done if you are processing a new stream.
         * Resetting the history in the middle of a stream will cause clicks.
         */
        void reset_history() {
            for (uint8_t i = 0; i < bands; ++i) {
                for (uint8_t j = 0; j < filters_per_band; ++j) {
                    BiQuad_reset(analysis_bands[i].filters[j]);
                    BiQuad_reset(synthesis_bands[i].filters[j]);
                }
                envelope_reset(analysis_envelopes[i]);
            }
        }

        /* Set the reaction time of the vocoder in seconds.
         *
         * The reaction time is the time it takes for the vocoder to respond to a volume change in the modulator.
         * A value of 0.03 (AKA 30 milliseconds) is recommended for intelligible speech.
         * Values lower than about 0.02 will make the output sound raspy and unpleasant.
         * Values above 0.2 or so will make the speech hard to understand, but can be used for special effects.
         * The value must be between 0.002 and 2.0 (inclusive).
         * Returns nonzero (true) on success or 0 (false) on failure.
         * The function will only fail if the parameter is invalid.
         */
        uint8_t set_reaction_time(float pReaction_time) {
            if (reaction_time < 0.002f || reaction_time > 2.0f) {
                return 0;
            }

            reaction_time = pReaction_time;
            initialize_envelopes();
            return 1;
        }

        /* Get the current formant shift of the vocoder in octaves. */
        float get_formant_shift() {
            return formant_shift;
        }

        /* Get the current reaction time of the vocoder in seconds. */
        float get_reaction_time() {
            return reaction_time;
        }

        float process(float pCarrierSample, float pModulatorSample) {
            float mOutputSamples[1];
            float mCarrierSample[1]   = {pCarrierSample};
            float mModulatorSample[1] = {pModulatorSample};
            process(mCarrierSample, mModulatorSample, mOutputSamples, 1);
            return mOutputSamples[0];
        }

    private:
        /**
         * The maximum number of filters per vocoder band (lower this number to save memory).
         */
        static const uint8_t VOCLIB_MAX_FILTERS_PER_BAND = 8;

    public:
        /**
         * this holds the data required to update samples thru a filter.
         */
        struct biquad {
            float a0, a1, a2, a3, a4 = 0;
            float x1, x2, y1, y2 = 0;
        };

        /**
         * Stores the state required for our envelope follower.
         */
        struct envelope {
            float coef       = 0;
            float history[4] = {0};
        };

        /**
         * Holds a set of filters required for one vocoder band.
         */
        struct band {
        public:
            biquad filters[VOCLIB_MAX_FILTERS_PER_BAND];
            // band() {
            //     for (uint8_t i = 0; i < VOCLIB_MAX_FILTERS_PER_BAND; i++) {
            //         filters[i] = new biquad();
            //     }
            // }
        };

    private:
        /* filter types. */
        static const uint8_t   VOCLIB_LPF       = 0;                      /* low pass filter */
        static const uint8_t   VOCLIB_HPF       = 1;                      /* High pass filter */
        static const uint8_t   VOCLIB_BPF       = 2;                      /* band pass filter */
        static const uint8_t   VOCLIB_NOTCH     = 3;                      /* Notch Filter */
        static const uint8_t   VOCLIB_PEQ       = 4;                      /* Peaking band EQ filter */
        static const uint8_t   VOCLIB_LSH       = 5;                      /* Low shelf filter */
        static const uint8_t   VOCLIB_HSH       = 6;                      /* High shelf filter */
        static const uint8_t   VOCLIB_MAX_BANDS = 96;                     /* The maximum number of bands that the vocoder can be initialized with (lower this number to save memory). */
        static constexpr float VOCLIB_M_LN2     = 0.69314718055994530942; /**/
        static constexpr float VOCLIB_M_PI      = 3.14159265358979323846; /**/
        band                   analysis_bands[VOCLIB_MAX_BANDS];          /* The filterbank used for analysis (these are applied to the modulator). */
        const uint8_t          bands;                                     /**/
        const uint8_t          filters_per_band;                          /**/
        envelope               analysis_envelopes[VOCLIB_MAX_BANDS];      /* The envelopes used to smooth the analysis bands. */
        float                  formant_shift;                             /* In octaves. 1.0 is unchanged. */
        float                  reaction_time;                             /* In seconds. Higher values make the vocoder respond more slowly to changes in the modulator. */
        band                   synthesis_bands[VOCLIB_MAX_BANDS * 2];     /* The filterbank used for synthesis (these are applied to the carrier). The second half of the array is only used for stereo carriers. */
        const uint32_t         sample_rate;                               /* in Hz */
        float                  fRectifyVolume;                            /**/

        /* Computes a BiQuad filter on a sample. */
        float BiQuad(float sample, biquad& b) {
            /* compute the result. */
            const float r0     = b.a0 * sample;
            const float r1     = b.a1 * b.x1;
            const float r2     = b.a2 * b.x2;
            const float r3     = b.a3 * b.y1;
            const float r4     = b.a4 * b.y2;
            const float r5     = r0 + r1;
            const float r6     = r2 - r3 - r4;
            const float result = r5 + r6;

            /* shift x1 to x2, sample to x1. */
            b.x2 = b.x1;
            b.x1 = sample;

            /* shift y1 to y2, result to y1. */
            b.y2 = b.y1;
            b.y1 = result;

            return result;
        }

        /* Envelope follower. */

        /* sets up a BiQuad Filter. */
        void BiQuad_new(biquad& b, uint8_t type, float dbGain, /* gain of filter */
                        float freq,                            /* center frequency */
                        float srate,                           /* sampling rate */
                        float bandwidth) /* bandwidth in octaves */ {
            float A, omega, sn, cs, alpha, beta;
            float a0, a1, a2, b0, b1, b2;

            /* setup variables. */
            A     = KlangWellen::pow(10, dbGain / 40.0f);
            omega = (2.0 * VOCLIB_M_PI * freq / srate);
#define VOCODER__USE_FAST_TRIG
#ifdef VOCODER__USE_FAST_TRIG
            sn    = KlangWellen::fast_sin(omega);
            cs    = KlangWellen::fast_cos(omega);
            alpha = sn * KlangWellen::fast_sinh(VOCLIB_M_LN2 / 2 * bandwidth * omega / sn);
            beta  = KlangWellen::fast_sqrt(A + A);
#else
            sn    = KlangWellen::sin(omega);
            cs    = KlangWellen::cos(omega);
            alpha = sn * KlangWellen::sinh(VOCLIB_M_LN2 / 2 * bandwidth * omega / sn);
            beta  = KlangWellen::sqrt(A + A);
#endif
            switch (type) {
                case VOCLIB_LPF:
                    b0 = (1 - cs) / 2;
                    b1 = 1 - cs;
                    b2 = (1 - cs) / 2;
                    a0 = 1 + alpha;
                    a1 = -2 * cs;
                    a2 = 1 - alpha;
                    break;
                case VOCLIB_HPF:
                    b0 = (1 + cs) / 2;
                    b1 = -(1 + cs);
                    b2 = (1 + cs) / 2;
                    a0 = 1 + alpha;
                    a1 = -2 * cs;
                    a2 = 1 - alpha;
                    break;
                case VOCLIB_BPF:
                    b0 = alpha;
                    b1 = 0;
                    b2 = -alpha;
                    a0 = 1 + alpha;
                    a1 = -2 * cs;
                    a2 = 1 - alpha;
                    break;
                case VOCLIB_NOTCH:
                    b0 = 1;
                    b1 = -2 * cs;
                    b2 = 1;
                    a0 = 1 + alpha;
                    a1 = -2 * cs;
                    a2 = 1 - alpha;
                    break;
                case VOCLIB_PEQ:
                    b0 = 1 + (alpha * A);
                    b1 = -2 * cs;
                    b2 = 1 - (alpha * A);
                    a0 = 1 + (alpha / A);
                    a1 = -2 * cs;
                    a2 = 1 - (alpha / A);
                    break;
                case VOCLIB_LSH:
                    b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
                    b1 = 2 * A * ((A - 1) - (A + 1) * cs);
                    b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
                    a0 = (A + 1) + (A - 1) * cs + beta * sn;
                    a1 = -2 * ((A - 1) + (A + 1) * cs);
                    a2 = (A + 1) + (A - 1) * cs - beta * sn;
                    break;
                case VOCLIB_HSH:
                    b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
                    b1 = -2 * A * ((A - 1) + (A + 1) * cs);
                    b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
                    a0 = (A + 1) - (A - 1) * cs + beta * sn;
                    a1 = 2 * ((A - 1) - (A + 1) * cs);
                    a2 = (A + 1) - (A - 1) * cs - beta * sn;
                    break;
                default:
                    return;
            }

            /* precompute the coefficients. */
            b.a0 = b0 / a0;
            b.a1 = b1 / a0;
            b.a2 = b2 / a0;
            b.a3 = a1 / a0;
            b.a4 = a2 / a0;
        }

        /* Reset the filter history. */
        void BiQuad_reset(biquad& b) {
            b.x1 = b.x2 = 0.0f;
            b.y1 = b.y2 = 0.0f;
        }

        void envelope_configure(envelope& envelope, float time_in_seconds, float sample_rate) {
            envelope.coef = (float)(KlangWellen::pow(0.01, 1.0 / (time_in_seconds * sample_rate)));
        }

        /* Reset the envelope history. */
        void envelope_reset(envelope& envelope) {
            envelope.history[0] = 0.0f;
            envelope.history[1] = 0.0f;
            envelope.history[2] = 0.0f;
            envelope.history[3] = 0.0f;
        }

        float envelope_tick(envelope& envelope, float sample) {
            const float coef = envelope.coef;
            const float e00  = (1.0f - coef) * KlangWellen::abs(sample);
            const float e01  = coef * envelope.history[0];
            const float e10  = (1.0f - coef) * envelope.history[0];
            const float e11  = coef * envelope.history[1];
            const float e20  = (1.0f - coef) * envelope.history[1];
            const float e21  = coef * envelope.history[2];
            const float e30  = (1.0f - coef) * envelope.history[2];
            const float e31  = coef * envelope.history[3];

            envelope.history[0] = (e00) + (e01);
            envelope.history[1] = (e10) + (e11);
            envelope.history[2] = (e20) + (e21);
            envelope.history[3] = (e30) + (e31);
            return envelope.history[3];
        }

        /* Initialize the vocoder envelopes. */
        void initialize_envelopes() {
            uint8_t i;

            envelope_configure(analysis_envelopes[0], reaction_time, sample_rate);
            for (i = 1; i < bands; ++i) {
                analysis_envelopes[i].coef = analysis_envelopes[0].coef;
            }
        }

        /* Initialize the vocoder filterbank. */
        void initialize_filterbank(bool pCarrier_only) {
            uint8_t i;
            float   step;
            float   lastfreq = 0.0;
            float   minfreq  = 80.0;
            float   maxfreq  = sample_rate;
            if (maxfreq > 12000.0) {
                maxfreq = 12000.0;
            }
            step = KlangWellen::pow((maxfreq / minfreq), (1.0 / bands));

            for (i = 0; i < bands; ++i) {
                float bandwidth, nextfreq;
                float priorfreq = lastfreq;
                if (lastfreq > 0.0) {
                    lastfreq *= step;
                } else {
                    lastfreq = minfreq;
                }
                nextfreq  = lastfreq * step;
                bandwidth = (nextfreq - priorfreq) / lastfreq;

                if (!pCarrier_only) {
                    BiQuad_new(analysis_bands[i].filters[0],
                               VOCLIB_BPF,
                               0.0f,
                               lastfreq,
                               sample_rate,
                               bandwidth);
                    for (uint8_t j = 1; j < filters_per_band; ++j) {
                        analysis_bands[i].filters[j].a0 = analysis_bands[i].filters[0].a0;
                        analysis_bands[i].filters[j].a1 = analysis_bands[i].filters[0].a1;
                        analysis_bands[i].filters[j].a2 = analysis_bands[i].filters[0].a2;
                        analysis_bands[i].filters[j].a3 = analysis_bands[i].filters[0].a3;
                        analysis_bands[i].filters[j].a4 = analysis_bands[i].filters[0].a4;
                    }
                }

                if (formant_shift != 1.0f) {
                    BiQuad_new(synthesis_bands[i].filters[0],
                               VOCLIB_BPF,
                               0.0f,
                               (float)(lastfreq * formant_shift),
                               (float)sample_rate,
                               (float)bandwidth);
                } else {
                    synthesis_bands[i].filters[0].a0 = analysis_bands[i].filters[0].a0;
                    synthesis_bands[i].filters[0].a1 = analysis_bands[i].filters[0].a1;
                    synthesis_bands[i].filters[0].a2 = analysis_bands[i].filters[0].a2;
                    synthesis_bands[i].filters[0].a3 = analysis_bands[i].filters[0].a3;
                    synthesis_bands[i].filters[0].a4 = analysis_bands[i].filters[0].a4;
                }

                synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a0 = synthesis_bands[i].filters[0].a0;
                synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a1 = synthesis_bands[i].filters[0].a1;
                synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a2 = synthesis_bands[i].filters[0].a2;
                synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a3 = synthesis_bands[i].filters[0].a3;
                synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a4 = synthesis_bands[i].filters[0].a4;

                for (uint8_t j = 1; j < filters_per_band; ++j) {
                    synthesis_bands[i].filters[j].a0 = synthesis_bands[i].filters[0].a0;
                    synthesis_bands[i].filters[j].a1 = synthesis_bands[i].filters[0].a1;
                    synthesis_bands[i].filters[j].a2 = synthesis_bands[i].filters[0].a2;
                    synthesis_bands[i].filters[j].a3 = synthesis_bands[i].filters[0].a3;
                    synthesis_bands[i].filters[j].a4 = synthesis_bands[i].filters[0].a4;

                    synthesis_bands[i + VOCLIB_MAX_BANDS].filters[j].a0 = synthesis_bands[i].filters[0].a0;
                    synthesis_bands[i + VOCLIB_MAX_BANDS].filters[j].a1 = synthesis_bands[i].filters[0].a1;
                    synthesis_bands[i + VOCLIB_MAX_BANDS].filters[j].a2 = synthesis_bands[i].filters[0].a2;
                    synthesis_bands[i + VOCLIB_MAX_BANDS].filters[j].a3 = synthesis_bands[i].filters[0].a3;
                    synthesis_bands[i + VOCLIB_MAX_BANDS].filters[j].a4 = synthesis_bands[i].filters[0].a4;
                }
            }
        }

        /* REVISION HISTORY
         *
         * Version 1.1 - 2019-02-16
         * Breaking change: Introduced a new argument to initialize called carrier_channels. This allows the
         * vocoder to output stereo natively.
         * Better assignment of band frequencies when using lower sample rates.
         * The shell now automatically normalizes the output file to match the peak amplitude in the carrier.
         * Fixed a memory corruption bug in the shell which would occur in response to an error condition.
         *
         * Version 1.0 - 2019-01-27
         * Initial release.
         */
    };
}  // namespace klangwellen
