/* IMPLEMENTATION */
#define VOCLIB_IMPLEMENTATION
#ifdef VOCLIB_IMPLEMENTATION


#ifdef __cplusplus
extern "C" {
#endif

#include "voclib.h"

extern float klang_math_sin(float r);
extern float klang_math_sinh(float r);
extern float klang_math_cos(float r);
extern float klang_math_fast_sqrt(float r);

#include <math.h>
#include <assert.h>

#ifdef _MSC_VER
#define VOCLIB_INLINE __forceinline
#else
#ifdef __GNUC__
#define VOCLIB_INLINE inline __attribute__((always_inline))
#else
#define VOCLIB_INLINE inline
#endif
#endif

#define USE_FAST_TRIGONOMETRY

/* Filters
 *
 * The filter code below was derived from http://www.musicdsp.org/files/biquad.c. The comment at the top of biquad.c file reads:
 *
 * Simple implementation of Biquad filters -- Tom St Denis
 *
 * Based on the work
 
 Cookbook formulae for audio EQ biquad filter coefficients
 ---------------------------------------------------------
 by Robert Bristow-Johnson, pbjrbj@viconet.com  a.k.a. robert@audioheads.com
 
 * Available on the web at
 
 http://www.smartelectronix.com/musicdsp/text/filters005.txt
 
 * Enjoy.
 *
 * This work is hereby placed in the public domain for all purposes, whether
 * commercial, free [as in speech] or educational, etc.  Use the code and please
 * give me credit if you wish.
 *
 * Tom St Denis -- http://tomstdenis.home.dhs.org
 */

#ifndef VOCLIB_M_LN2
#define VOCLIB_M_LN2       0.69314718055994530942
#endif

#ifndef VOCLIB_M_PI
#define VOCLIB_M_PI        3.14159265358979323846
#endif

/* Computes a BiQuad filter on a sample. */
static VOCLIB_INLINE float voclib_BiQuad ( float sample, voclib_biquad* b )
{
    float result;
    
    /* compute the result. */
    result = b->a0 * sample + b->a1 * b->x1 + b->a2 * b->x2 -
    b->a3 * b->y1 - b->a4 * b->y2;
    
    /* shift x1 to x2, sample to x1. */
    b->x2 = b->x1;
    b->x1 = sample;
    
    /* shift y1 to y2, result to y1. */
    b->y2 = b->y1;
    b->y1 = result;
    
    return result;
}

/* filter types. */
enum
{
    VOCLIB_LPF, /* low pass filter */
    VOCLIB_HPF, /* High pass filter */
    VOCLIB_BPF, /* band pass filter */
    VOCLIB_NOTCH, /* Notch Filter */
    VOCLIB_PEQ, /* Peaking band EQ filter */
    VOCLIB_LSH, /* Low shelf filter */
    VOCLIB_HSH /* High shelf filter */
};

/* sets up a BiQuad Filter. */
static void voclib_BiQuad_new ( voclib_biquad* b, int type, float dbGain, /* gain of filter */
                               float freq, /* center frequency */
                               float srate, /* sampling rate */
                               float bandwidth ) /* bandwidth in octaves */
{
    float A, omega, sn, cs, alpha, beta;
    float a0, a1, a2, b0, b1, b2;
    
    /* setup variables. */
    A = ( float ) pow ( 10, dbGain / 40.0f );
    omega = ( float ) ( 2.0 * VOCLIB_M_PI * freq / srate );
#ifdef USE_FAST_TRIGONOMETRY
    sn = klang_math_sin ( omega );
    cs = klang_math_cos ( omega );
    alpha = sn * klang_math_sinh( VOCLIB_M_LN2 / 2 * bandwidth * omega / sn );
    beta = klang_math_fast_sqrt( A + A );
#else
    sn = ( float ) sin ( omega );
    cs = ( float ) cos ( omega );
    alpha = sn * ( float ) sinh ( VOCLIB_M_LN2 / 2 * bandwidth * omega / sn );
    beta = ( float ) sqrt ( A + A );
#endif

    switch ( type )
    {
        case VOCLIB_LPF:
            b0 = ( 1 - cs ) / 2;
            b1 = 1 - cs;
            b2 = ( 1 - cs ) / 2;
            a0 = 1 + alpha;
            a1 = -2 * cs;
            a2 = 1 - alpha;
            break;
        case VOCLIB_HPF:
            b0 = ( 1 + cs ) / 2;
            b1 = - ( 1 + cs );
            b2 = ( 1 + cs ) / 2;
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
            b0 = 1 + ( alpha * A );
            b1 = -2 * cs;
            b2 = 1 - ( alpha * A );
            a0 = 1 + ( alpha / A );
            a1 = -2 * cs;
            a2 = 1 - ( alpha / A );
            break;
        case VOCLIB_LSH:
            b0 = A * ( ( A + 1 ) - ( A - 1 ) * cs + beta * sn );
            b1 = 2 * A * ( ( A - 1 ) - ( A + 1 ) * cs );
            b2 = A * ( ( A + 1 ) - ( A - 1 ) * cs - beta * sn );
            a0 = ( A + 1 ) + ( A - 1 ) * cs + beta * sn;
            a1 = -2 * ( ( A - 1 ) + ( A + 1 ) * cs );
            a2 = ( A + 1 ) + ( A - 1 ) * cs - beta * sn;
            break;
        case VOCLIB_HSH:
            b0 = A * ( ( A + 1 ) + ( A - 1 ) * cs + beta * sn );
            b1 = -2 * A * ( ( A - 1 ) + ( A + 1 ) * cs );
            b2 = A * ( ( A + 1 ) + ( A - 1 ) * cs - beta * sn );
            a0 = ( A + 1 ) - ( A - 1 ) * cs + beta * sn;
            a1 = 2 * ( ( A - 1 ) - ( A + 1 ) * cs );
            a2 = ( A + 1 ) - ( A - 1 ) * cs - beta * sn;
            break;
        default:
            assert ( 0 ); /* Misuse. */
            return;
    }
    
    /* precompute the coefficients. */
    b->a0 = b0 / a0;
    b->a1 = b1 / a0;
    b->a2 = b2 / a0;
    b->a3 = a1 / a0;
    b->a4 = a2 / a0;
}

/* Reset the filter history. */
static void voclib_BiQuad_reset ( voclib_biquad* b )
{
    b->x1 = b->x2 = 0.0f;
    b->y1 = b->y2 = 0.0f;
}

/* Envelope follower. */

static void voclib_envelope_configure ( voclib_envelope* envelope, double time_in_seconds, double sample_rate )
{
    envelope->coef = ( float ) ( pow ( 0.01, 1.0 / ( time_in_seconds * sample_rate ) ) );
}

/* Reset the envelope history. */
static void voclib_envelope_reset ( voclib_envelope* envelope )
{
    envelope->history[0] = 0.0f;
    envelope->history[1] = 0.0f;
    envelope->history[2] = 0.0f;
    envelope->history[3] = 0.0f;
}

static VOCLIB_INLINE float voclib_envelope_tick ( voclib_envelope* envelope, float sample )
{
    const float coef = envelope->coef;
    envelope->history[0] = ( float ) ( ( 1.0f - coef ) * fabs ( sample ) ) + ( coef * envelope->history[0] );
    envelope->history[1] = ( ( 1.0f - coef ) * envelope->history[0] ) + ( coef * envelope->history[1] );
    envelope->history[2] = ( ( 1.0f - coef ) * envelope->history[1] ) + ( coef * envelope->history[2] );
    envelope->history[3] = ( ( 1.0f - coef ) * envelope->history[2] ) + ( coef * envelope->history[3] );
    return envelope->history[3];
}

/* Initialize the vocoder filterbank. */
static void voclib_initialize_filterbank ( voclib_instance* instance, int carrier_only )
{
    unsigned char i;
    double step;
    double lastfreq = 0.0;
    double minfreq = 80.0;
    double maxfreq = instance->sample_rate;
    if ( maxfreq > 12000.0 )
    {
        maxfreq = 12000.0;
    }
    step = pow ( ( maxfreq / minfreq ), ( 1.0 / instance->bands ) );
    
    for ( i = 0; i < instance->bands; ++i )
    {
        unsigned char i2;
        double bandwidth, nextfreq;
        double priorfreq = lastfreq;
        if ( lastfreq > 0.0 )
        {
            lastfreq *= step;
        }
        else
        {
            lastfreq = minfreq;
        }
        nextfreq = lastfreq * step;
        bandwidth = ( nextfreq - priorfreq ) / lastfreq;
        
        if ( !carrier_only )
        {
            voclib_BiQuad_new ( &instance->analysis_bands[i].filters[0], VOCLIB_BPF, 0.0f, ( float ) lastfreq, ( float ) instance->sample_rate, ( float ) bandwidth );
            for ( i2 = 1; i2 < instance->filters_per_band; ++i2 )
            {
                instance->analysis_bands[i].filters[i2].a0 = instance->analysis_bands[i].filters[0].a0;
                instance->analysis_bands[i].filters[i2].a1 = instance->analysis_bands[i].filters[0].a1;
                instance->analysis_bands[i].filters[i2].a2 = instance->analysis_bands[i].filters[0].a2;
                instance->analysis_bands[i].filters[i2].a3 = instance->analysis_bands[i].filters[0].a3;
                instance->analysis_bands[i].filters[i2].a4 = instance->analysis_bands[i].filters[0].a4;
            }
        }
        
        if ( instance->formant_shift != 1.0f )
        {
            voclib_BiQuad_new ( &instance->synthesis_bands[i].filters[0], VOCLIB_BPF, 0.0f, ( float ) ( lastfreq * instance->formant_shift ), ( float ) instance->sample_rate, ( float ) bandwidth );
        }
        else
        {
            instance->synthesis_bands[i].filters[0].a0 = instance->analysis_bands[i].filters[0].a0;
            instance->synthesis_bands[i].filters[0].a1 = instance->analysis_bands[i].filters[0].a1;
            instance->synthesis_bands[i].filters[0].a2 = instance->analysis_bands[i].filters[0].a2;
            instance->synthesis_bands[i].filters[0].a3 = instance->analysis_bands[i].filters[0].a3;
            instance->synthesis_bands[i].filters[0].a4 = instance->analysis_bands[i].filters[0].a4;
        }
        
        instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a0 = instance->synthesis_bands[i].filters[0].a0;
        instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a1 = instance->synthesis_bands[i].filters[0].a1;
        instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a2 = instance->synthesis_bands[i].filters[0].a2;
        instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a3 = instance->synthesis_bands[i].filters[0].a3;
        instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[0].a4 = instance->synthesis_bands[i].filters[0].a4;
        
        for ( i2 = 1; i2 < instance->filters_per_band; ++i2 )
        {
            instance->synthesis_bands[i].filters[i2].a0 = instance->synthesis_bands[i].filters[0].a0;
            instance->synthesis_bands[i].filters[i2].a1 = instance->synthesis_bands[i].filters[0].a1;
            instance->synthesis_bands[i].filters[i2].a2 = instance->synthesis_bands[i].filters[0].a2;
            instance->synthesis_bands[i].filters[i2].a3 = instance->synthesis_bands[i].filters[0].a3;
            instance->synthesis_bands[i].filters[i2].a4 = instance->synthesis_bands[i].filters[0].a4;
            
            instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[i2].a0 = instance->synthesis_bands[i].filters[0].a0;
            instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[i2].a1 = instance->synthesis_bands[i].filters[0].a1;
            instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[i2].a2 = instance->synthesis_bands[i].filters[0].a2;
            instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[i2].a3 = instance->synthesis_bands[i].filters[0].a3;
            instance->synthesis_bands[i + VOCLIB_MAX_BANDS].filters[i2].a4 = instance->synthesis_bands[i].filters[0].a4;
        }
    }
    
}

/* Initialize the vocoder envelopes. */
static void voclib_initialize_envelopes ( voclib_instance* instance )
{
    unsigned char i;
    
    voclib_envelope_configure ( &instance->analysis_envelopes[0], instance->reaction_time, ( double ) instance->sample_rate );
    for ( i = 1; i < instance->bands; ++i )
    {
        instance->analysis_envelopes[i].coef = instance->analysis_envelopes[0].coef;
    }
}

int voclib_initialize ( voclib_instance* instance, unsigned char bands, unsigned char filters_per_band, unsigned int sample_rate, unsigned char carrier_channels )
{
    if ( !instance )
    {
        return 0;
    }
    if ( bands < 4 || bands > VOCLIB_MAX_BANDS )
    {
        return 0;
    }
    if ( filters_per_band < 1 || filters_per_band > VOCLIB_MAX_FILTERS_PER_BAND )
    {
        return 0;
    }
    if ( sample_rate < 8000 || sample_rate > 192000 )
    {
        return 0;
    }
    if ( carrier_channels < 1 || carrier_channels > 2 )
    {
        return 0;
    }
    
    instance->reaction_time = 0.03f;
    instance->formant_shift = 1.0f;
    instance->sample_rate = sample_rate;
    instance->bands = bands;
    instance->filters_per_band = filters_per_band;
    instance->carrier_channels = carrier_channels;
    
    voclib_reset_history ( instance );
    voclib_initialize_filterbank ( instance, 0 );
    voclib_initialize_envelopes ( instance );
    
    return 1;
}

void voclib_reset_history ( voclib_instance* instance )
{
    unsigned char i;
    
    for ( i = 0; i < instance->bands; ++i )
    {
        unsigned char i2;
        
        for ( i2 = 0; i2 < instance->filters_per_band; ++i2 )
        {
            voclib_BiQuad_reset ( &instance->analysis_bands[i].filters[i2] );
            voclib_BiQuad_reset ( &instance->synthesis_bands[i].filters[i2] );
        }
        voclib_envelope_reset ( &instance->analysis_envelopes[i] );
    }
}

int voclib_process ( voclib_instance* instance, const float* carrier_buffer, const float* modulator_buffer, float* output_buffer, unsigned int frames )
{
    unsigned int i;
    const unsigned char bands = instance->bands;
    const unsigned char filters_per_band = instance->filters_per_band;
    
    if ( !carrier_buffer )
    {
        return 0;
    }
    if ( !modulator_buffer )
    {
        return 0;
    }
    if ( !output_buffer )
    {
        return 0;
    }
    if ( frames == 0 )
    {
        return 0;
    }
    
    if ( instance->carrier_channels == 2 )
    {
        
        /* The carrier has two channels and the modulator has 1. */
        for ( i = 0; i < frames * 2; i += 2, ++modulator_buffer )
        {
            unsigned char i2;
            float out_left = 0.0f;
            float out_right = 0.0f;
            
            /* Run the bands in parallel and accumulate the output. */
            for ( i2 = 0; i2 < bands; ++i2 )
            {
                unsigned char i3;
                float analysis_band = voclib_BiQuad ( *modulator_buffer, &instance->analysis_bands[i2].filters[0] );
                float synthesis_band_left = voclib_BiQuad ( carrier_buffer[i], &instance->synthesis_bands[i2].filters[0] );
                float synthesis_band_right = voclib_BiQuad ( carrier_buffer[i + 1], &instance->synthesis_bands[i2 + VOCLIB_MAX_BANDS].filters[0] );
                
                for ( i3 = 1; i3 < filters_per_band; ++i3 )
                {
                    analysis_band = voclib_BiQuad ( analysis_band, &instance->analysis_bands[i2].filters[i3] );
                    synthesis_band_left = voclib_BiQuad ( synthesis_band_left, &instance->synthesis_bands[i2].filters[i3] );
                    synthesis_band_right = voclib_BiQuad ( synthesis_band_right, &instance->synthesis_bands[i2 + VOCLIB_MAX_BANDS].filters[i3] );
                }
                analysis_band = voclib_envelope_tick ( &instance->analysis_envelopes[i2], analysis_band );
                out_left += synthesis_band_left * analysis_band;
                out_right += synthesis_band_right * analysis_band;
            }
            output_buffer[i] = out_left;
            output_buffer[i + 1] = out_right;
        }
        
    }
    else
    {
        
        /* Both the carrier and the modulator have a single channel. */
        for ( i = 0; i < frames; ++i )
        {
            unsigned char i2;
            float out = 0.0f;
            
            /* Run the bands in parallel and accumulate the output. */
            for ( i2 = 0; i2 < bands; ++i2 )
            {
                unsigned char i3;
                float analysis_band = voclib_BiQuad ( modulator_buffer[i], &instance->analysis_bands[i2].filters[0] );
                float synthesis_band = voclib_BiQuad ( carrier_buffer[i], &instance->synthesis_bands[i2].filters[0] );
                
                for ( i3 = 1; i3 < filters_per_band; ++i3 )
                {
                    analysis_band = voclib_BiQuad ( analysis_band, &instance->analysis_bands[i2].filters[i3] );
                    synthesis_band = voclib_BiQuad ( synthesis_band, &instance->synthesis_bands[i2].filters[i3] );
                }
                analysis_band = voclib_envelope_tick ( &instance->analysis_envelopes[i2], analysis_band );
                out += synthesis_band * analysis_band;
            }
            output_buffer[i] = out;
        }
    }
    
    return 1;
}

int voclib_set_reaction_time ( voclib_instance* instance, float reaction_time )
{
    if ( reaction_time < 0.002f || reaction_time > 2.0f )
    {
        return 0;
    }
    
    instance->reaction_time = reaction_time;
    voclib_initialize_envelopes ( instance );
    return 1;
}

float voclib_get_reaction_time ( const voclib_instance* instance )
{
    return instance->reaction_time;
}

int voclib_set_formant_shift ( voclib_instance* instance, float formant_shift )
{
    if ( formant_shift < 0.25f || formant_shift > 4.0f )
    {
        return 0;
    }
    
    instance->formant_shift = formant_shift;
    voclib_initialize_filterbank ( instance, 1 );
    return 1;
}

float voclib_get_formant_shift ( const voclib_instance* instance )
{
    return instance->formant_shift;
}

#ifdef __cplusplus
}
#endif

#endif /* VOCLIB_IMPLEMENTATION */

/* REVISION HISTORY
 *
 * Version 1.1 - 2019-02-16
 * Breaking change: Introduced a new argument to voclib_initialize called carrier_channels. This allows the vocoder to output stereo natively.
 * Better assignment of band frequencies when using lower sample rates.
 * The shell now automatically normalizes the output file to match the peak amplitude in the carrier.
 * Fixed a memory corruption bug in the shell which would occur in response to an error condition.
 *
 * Version 1.0 - 2019-01-27
 * Initial release.
 */

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
 This is free and unencumbered software released into the public domain.
 Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
 software, either in source code form or as a compiled binary, for any purpose,
 commercial or non-commercial, and by any means.
 
 In jurisdictions that recognize copyright laws, the author or authors of this
 software dedicate any and all copyright interest in the software to the public
 domain. We make this dedication for the benefit of the public at large and to
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
