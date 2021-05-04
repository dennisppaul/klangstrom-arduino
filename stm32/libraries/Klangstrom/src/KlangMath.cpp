//
//  Util.cpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "KlangMath.hpp"

#include <cstdint>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cfloat>

using namespace klang;

uint32_t KlangMath::x32Seed = 123456789;

// from https://github.com/SRombauts/SimplexNoise

/**
 * @file    SimplexNoise.cpp
 * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D, 4D).
 *
 * Copyright (c) 2014-2015 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * This C++ implementation is based on the speed-improved Java version 2012-03-09
 * by Stefan Gustavson (original Java source code in the public domain).
 * http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java:
 * - Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * - Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * - Better rank ordering method by Stefan Gustavson in 2012.
 *
 * This implementation is "Simplex Noise" as presented by
 * Ken Perlin at a relatively obscure and not often cited course
 * session "Real-Time Shading" at Siggraph 2001 (before real
 * time shading actually took on), under the title "hardware noise".
 * The 3D function is numerically equivalent to his Java reference
 * code available in the PDF course notes, although I re-implemented
 * it from scratch to get more readable code. The 1D, 2D and 4D cases
 * were implemented from scratch by me from Ken Perlin's text.
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/**
 * Computes the largest integer value not greater than the float one
 *
 * This method is faster than using (int32_t)std::floor(fp).
 *
 * I measured it to be approximately twice as fast:
 *  float:  ~18.4ns instead of ~39.6ns on an AMD APU),
 *  double: ~20.6ns instead of ~36.6ns on an AMD APU),
 * Reference: http://www.codeproject.com/Tips/700780/Fast-floor-ceiling-functions
 *
 * @param[in] fp    float input value
 *
 * @return largest integer value not greater than fp
 */
static inline int32_t fastfloor(float fp) {
    int32_t i = (int32_t)fp;
    return (fp < i) ? (i - 1) : (i);
}

/**
 * Permutation table. This is just a random jumble of all numbers 0-255.
 *
 * This produce a repeatable pattern of 256, but Ken Perlin stated
 * that it is not a problem for graphic texture as the noise features disappear
 * at a distance far enough to be able to see a repeatable pattern of 256.
 *
 * This needs to be exactly the same for all instances on all platforms,
 * so it's easiest to just keep it as static explicit data.
 * This also removes the need for any initialisation of this class.
 *
 * Note that making this an uint32_t[] instead of a uint8_t[] might make the
 * code run faster on platforms with a high penalty for unaligned single
 * byte addressing. Intel x86 is generally single-byte-friendly, but
 * some other CPUs are faster with 4-aligned reads.
 * However, a char[] is smaller, which avoids cache trashing, and that
 * is probably the most important aspect on most architectures.
 * This array is accessed a *lot* by the noise functions.
 * A vector-valued noise over 3D accesses it 96 times, and a
 * float-valued 4D noise 64 times. We want this to fit in the cache!
 */
static const uint8_t perm[256] = {
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

/**
 * Helper function to hash an integer using the above permutation table
 *
 *  This inline function costs around 1ns, and is called N+1 times for a noise of N dimension.
 *
 *  Using a real hash function would be better to improve the "repeatability of 256" of the above permutation table,
 * but fast integer Hash functions uses more time and have bad random properties.
 *
 * @param[in] i Integer value to hash
 *
 * @return 8-bits hashed value
 */
static inline uint8_t _hash(int32_t i) {
    return perm[static_cast<uint8_t>(i)];
}

/* NOTE Gradient table to test if lookup-table are more efficient than calculs
 static const float gradients1D[16] = {
 -8.f, -7.f, -6.f, -5.f, -4.f, -3.f, -2.f, -1.f,
 1.f,  2.f,  3.f,  4.f,  5.f,  6.f,  7.f,  8.f
 };
 */

/**
 * Helper function to compute gradients-dot-residual vectors (1D)
 *
 * @note that these generate gradients of more than unit length. To make
 * a close match with the value range of classic Perlin noise, the final
 * noise values need to be rescaled to fit nicely within [-1,1].
 * (The simplex noise functions as such also have different scaling.)
 * Note also that these noise functions are the most practical and useful
 * signed version of Perlin noise.
 *
 * @param[in] hash  hash value
 * @param[in] x     distance to the corner
 *
 * @return gradient value
 */
static float grad(int32_t hash, float x) {
    int32_t h = hash & 0x0F;        // Convert low 4 bits of hash code
    float grad = 1.0f + (h & 7);    // Gradient value 1.0, 2.0, ..., 8.0
    if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient
    //  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
    return (grad * x);              // Multiply the gradient with the distance
}

/**
 * 1D Perlin simplex noise
 *
 *  Takes around 74ns on an AMD APU.
 *
 * @param[in] x float coordinate
 *
 * @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
 */
float SimplexNoise::noise(float x) {
    float n0, n1;   // Noise contributions from the two "corners"
    
    // No need to skew the input space in 1D
    
    // Corners coordinates (nearest integer values):
    int32_t i0 = fastfloor(x);
    int32_t i1 = i0 + 1;
    // Distances to corners (between 0 and 1):
    float x0 = x - i0;
    float x1 = x0 - 1.0f;
    
    // Calculate the contribution from the first corner
    float t0 = 1.0f - x0*x0;
    //  if(t0 < 0.0f) t0 = 0.0f; // not possible
    t0 *= t0;
    n0 = t0 * t0 * grad(_hash(i0), x0);
    
    // Calculate the contribution from the second corner
    float t1 = 1.0f - x1*x1;
    //  if(t1 < 0.0f) t1 = 0.0f; // not possible
    t1 *= t1;
    n1 = t1 * t1 * grad(_hash(i1), x1);
    
    // The maximum value of this noise is 8*(3/4)^4 = 2.53125
    // A factor of 0.395 scales to fit exactly within [-1,1]
    return 0.395f * (n0 + n1);
}

// for alternative simplex noise see also http://staffwww.itn.liu.se/~stegu/aqsis/aqsis-newnoise/

/* ------------------------------------------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-variable"
# pragma clang diagnostic ignored "-Wstrict-aliasing"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif


    /* --------------------------------------------------------------------------------------------------- */
    
    // from http://www.ganssle.com/approx.htm
    //
    // The following code implements approximations to various trig functions.
    //
    // This is demo code to guide developers in implementing their own approximation
    // software. This code is merely meant to illustrate algorithms.
    
    static double const twopi=2.0*PI;            // pi times 2
    static double const two_over_pi= 2.0/PI;        // 2/pi
    static double const halfpi=PI/2.0;            // pi divided by 2
    static double const threehalfpi=3.0*PI/2.0;          // pi times 3/2, used in tan routines
    static double const four_over_pi=4.0/PI;        // 4/pi, used in tan routines
    static double const sixthpi=PI/6.0;            // pi/6.0, used in atan routines
    static double const tansixthpi=0.57735026;//tan(sixthpi);        // tan(pi/6), used in atan routines
    static double const tantwelfthpi=0.2679492;//tan(twelfthpi);    // tan(pi/12), used in atan routines
    //static double const qtrpi=PI/4.0;            // pi/4.0, used in tan routines
    //static double const twelfthpi=PI/12.0;            // pi/12.0, used in atan routines
    
    // *********************************************************
    // ***
    // ***   Routines to compute sine and cosine to 3.2 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        cos_32s computes cosine (x)
    //
    //  Accurate to about 3.2 decimal digits over the range [0, pi/2].
    //  The input argument is in radians.
    //
    //  Algorithm:
    //        cos(x)= c1 + c2*x**2 + c3*x**4
    //   which is the same as:
    //        cos(x)= c1 + x**2(c2 + c3*x**2)
    //
    static float cos_32s(float x)
    {
        const float c1= 0.99940307;
        const float c2=-0.49558072;
        const float c3= 0.03679168;
        
        float x2;                            // The input argument squared
        
        x2=x * x;
        return (c1 + x2*(c2 + c3 * x2));
    }
    
    //
    // This is the main cosine approximation "driver"
    // It reduces the input argument's range to [0, pi/2],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    //
    static float cos_32(float x){
        int quad;                        // what quadrant are we in?
        
        x=fmod(x, twopi);                // Get rid of values > 2* pi
        if(x<0)x=-x;                    // cos(-x) = cos(x)
        quad=(int)(x * two_over_pi);            // Get quadrant # (0 to 3) we're in
        switch (quad){
            case 0: return  cos_32s(x);
            case 1: return -cos_32s(PI-x);
            case 2: return -cos_32s(x-PI);
            case 3: return  cos_32s(twopi-x);
        }
        return 0;
    }
    //
    // The sine is just cosine shifted a half-pi, so
    // we'll adjust the argument and call the cosine approximation.
    //
    float sin_32(float x){
        return cos_32(halfpi-x);
    }
    
    // *********************************************************
    // ***
    // ***   Routines to compute sine and cosine to 5.2 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        cos_52s computes cosine (x)
    //
    //  Accurate to about 5.2 decimal digits over the range [0, pi/2].
    //  The input argument is in radians.
    //
    //  Algorithm:
    //        cos(x)= c1 + c2*x**2 + c3*x**4 + c4*x**6
    //   which is the same as:
    //        cos(x)= c1 + x**2(c2 + c3*x**2 + c4*x**4)
    //        cos(x)= c1 + x**2(c2 + x**2(c3 + c4*x**2))
    //
    static float cos_52s(float x)
    {
        const float c1= 0.9999932946;
        const float c2=-0.4999124376;
        const float c3= 0.0414877472;
        const float c4=-0.0012712095;
        
        float x2;                            // The input argument squared
        
        x2=x * x;
        return (c1 + x2*(c2 + x2*(c3 + c4*x2)));
    }
    
    //
    //  This is the main cosine approximation "driver"
    // It reduces the input argument's range to [0, pi/2],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    //
    static float cos_52(float x){
        int quad;                        // what quadrant are we in?
        
        x=fmod(x, twopi);                // Get rid of values > 2* pi
        if(x<0)x=-x;                    // cos(-x) = cos(x)
        quad=(int)(x * two_over_pi);            // Get quadrant # (0 to 3) we're in
        switch (quad){
            case 0: return  cos_52s(x);
            case 1: return -cos_52s(PI-x);
            case 2: return -cos_52s(x-PI);
            case 3: return  cos_52s(twopi-x);
        }
        return 0;
    }
    //
    //   The sine is just cosine shifted a half-pi, so
    // we'll adjust the argument and call the cosine approximation.
    //
    float sin_52(float x){
        return cos_52(halfpi-x);
    }
    
    // *********************************************************
    // ***
    // ***   Routines to compute sine and cosine to 7.3 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        cos_73s computes cosine (x)
    //
    //  Accurate to about 7.3 decimal digits over the range [0, pi/2].
    //  The input argument is in radians.
    //
    //  Algorithm:
    //        cos(x)= c1 + c2*x**2 + c3*x**4 + c4*x**6 + c5*x**8
    //   which is the same as:
    //        cos(x)= c1 + x**2(c2 + c3*x**2 + c4*x**4 + c5*x**6)
    //        cos(x)= c1 + x**2(c2 + x**2(c3 + c4*x**2 + c5*x**4))
    //        cos(x)= c1 + x**2(c2 + x**2(c3 + x**2(c4 + c5*x**2)))
    //
    static double cos_73s(double x)
    {
        const double c1= 0.999999953464;
        const double c2=-0.499999053455;
        const double c3= 0.0416635846769;
        const double c4=-0.0013853704264;
        const double c5= 0.00002315393167;
        
        
        double x2;                            // The input argument squared
        
        x2=x * x;
        return (c1 + x2*(c2 + x2*(c3 + x2*(c4 + c5*x2))));
    }
    
    //
    //  This is the main cosine approximation "driver"
    // It reduces the input argument's range to [0, pi/2],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    //
    static double cos_73(double x){
        int quad;                        // what quadrant are we in?
        
        x=fmod(x, twopi);                // Get rid of values > 2* pi
        if(x<0)x=-x;                    // cos(-x) = cos(x)
        quad=(int)(x * two_over_pi);            // Get quadrant # (0 to 3) we're in
        switch (quad){
            case 0: return  cos_73s(x);
            case 1: return -cos_73s(PI-x);
            case 2: return -cos_73s(x-PI);
            case 3: return  cos_73s(twopi-x);
        }
        return 0;
    }
    //
    //   The sine is just cosine shifted a half-pi, so
    // we'll adjust the argument and call the cosine approximation.
    //
    double sin_73(double x){
        return cos_73(halfpi-x);
    }
    
    // *********************************************************
    // ***
    // ***   Routines to compute sine and cosine to 12.1 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        cos_121s computes cosine (x)
    //
    //  Accurate to about 12.1 decimal digits over the range [0, pi/2].
    //  The input argument is in radians.
    //
    //  Algorithm:
    //        cos(x)= c1 + c2*x**2 + c3*x**4 + c4*x**6 + c5*x**8 + c6*x**10 + c7*x**12
    //   which is the same as:
    //        cos(x)= c1 + x**2(c2 + c3*x**2 + c4*x**4 + c5*x**6 + c6*x**8 + c7*x**10)
    //        cos(x)= c1 + x**2(c2 + x**2(c3 + c4*x**2 + c5*x**4 + c6*x**6 + c7*x**8 ))
    //        cos(x)= c1 + x**2(c2 + x**2(c3 + x**2(c4 + c5*x**2 + c6*x**4 + c7*x**6 )))
    //        cos(x)= c1 + x**2(c2 + x**2(c3 + x**2(c4 + x**2(c5 + c6*x**2 + c7*x**4 ))))
    //        cos(x)= c1 + x**2(c2 + x**2(c3 + x**2(c4 + x**2(c5 + x**2(c6 + c7*x**2 )))))
    //
    static double cos_121s(double x)
    {
        const double c1= 0.99999999999925182;
        const double c2=-0.49999999997024012;
        const double c3= 0.041666666473384543;
        const double c4=-0.001388888418000423;
        const double c5= 0.0000248010406484558;
        const double c6=-0.0000002752469638432;
        const double c7= 0.0000000019907856854;
        
        double x2;                            // The input argument squared
        
        x2=x * x;
        return (c1 + x2*(c2 + x2*(c3 + x2*(c4 + x2*(c5 + x2*(c6 + c7*x2))))));
    }
    
    //
    //  This is the main cosine approximation "driver"
    // It reduces the input argument's range to [0, pi/2],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    //
    static double cos_121(double x){
        int quad;                        // what quadrant are we in?
        
        x=fmod(x, twopi);                // Get rid of values > 2* pi
        if(x<0)x=-x;                    // cos(-x) = cos(x)
        quad=(int)(x * two_over_pi);            // Get quadrant # (0 to 3) we're in
        switch (quad){
            case 0: return  cos_121s(x);
            case 1: return -cos_121s(PI-x);
            case 2: return -cos_121s(x-PI);
            case 3: return  cos_121s(twopi-x);
        }
        return 0;
    }
    //
    //   The sine is just cosine shifted a half-pi, so
    // we'll adjust the argument and call the cosine approximation.
    //
    double sin_121(double x){
        return cos_121(halfpi-x);
    }
    
    // *********************************************************
    // ***
    // ***   Routines to compute tangent to 3.2 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        tan_32s computes tan(pi*x/4)
    //
    //  Accurate to about 3.2 decimal digits over the range [0, pi/4].
    //  The input argument is in radians. Note that the function
    //  computes tan(pi*x/4), NOT tan(x); it's up to the range
    //  reduction algorithm that calls this to scale things properly.
    //
    //  Algorithm:
    //        tan(x)= x*c1/(c2 + x**2)
    //
    static float tan_32s(float x)
    {
        const float c1=-3.6112171;
        const float c2=-4.6133253;
        
        float x2;                            // The input argument squared
        
        x2=x * x;
        return (x*c1/(c2 + x2));
    }
    
    //
    //  This is the main tangent approximation "driver"
    // It reduces the input argument's range to [0, pi/4],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    // Enter with positive angles only.
    //
    // WARNING: We do not test for the tangent approaching infinity,
    // which it will at x=pi/2 and x=3*pi/2. If this is a problem
    // in your application, take appropriate action.
    //
    float tan_32(float x){
        int octant;                        // what octant are we in?
        
        x=fmod(x, twopi);                // Get rid of values >2 *pi
        octant=(int)(x * four_over_pi);            // Get octant # (0 to 7)
        switch (octant){
            case 0: return      tan_32s(x              *four_over_pi);
            case 1: return  1.0/tan_32s((halfpi-x)     *four_over_pi);
            case 2: return -1.0/tan_32s((x-halfpi)     *four_over_pi);
            case 3: return -    tan_32s((PI-x)         *four_over_pi);
            case 4: return      tan_32s((x-PI)         *four_over_pi);
            case 5: return  1.0/tan_32s((threehalfpi-x)*four_over_pi);
            case 6: return -1.0/tan_32s((x-threehalfpi)*four_over_pi);
            case 7: return -    tan_32s((twopi-x)      *four_over_pi);
        }
        return 0;
    }
    
    // *********************************************************
    // ***
    // ***   Routines to compute tangent to 5.6 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        tan_56s computes tan(pi*x/4)
    //
    //  Accurate to about 5.6 decimal digits over the range [0, pi/4].
    //  The input argument is in radians. Note that the function
    //  computes tan(pi*x/4), NOT tan(x); it's up to the range
    //  reduction algorithm that calls this to scale things properly.
    //
    //  Algorithm:
    //        tan(x)= x(c1 + c2*x**2)/(c3 + x**2)
    //
    static float tan_56s(float x)
    {
        const float c1=-3.16783027;
        const float c2= 0.134516124;
        const float c3=-4.033321984;
        
        float x2;                            // The input argument squared
        
        x2=x * x;
        return (x*(c1 + c2 * x2)/(c3 + x2));
    }
    
    //
    //  This is the main tangent approximation "driver"
    // It reduces the input argument's range to [0, pi/4],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    // Enter with positive angles only.
    //
    // WARNING: We do not test for the tangent approaching infinity,
    // which it will at x=pi/2 and x=3*pi/2. If this is a problem
    // in your application, take appropriate action.
    //
    float tan_56(float x){
        int octant;                        // what octant are we in?
        
        x=fmod(x, twopi);                // Get rid of values >2 *pi
        octant=(int)(x * four_over_pi);            // Get octant # (0 to 7)
        switch (octant){
            case 0: return      tan_56s(x              *four_over_pi);
            case 1: return  1.0/tan_56s((halfpi-x)     *four_over_pi);
            case 2: return -1.0/tan_56s((x-halfpi)     *four_over_pi);
            case 3: return -    tan_56s((PI-x)         *four_over_pi);
            case 4: return      tan_56s((x-PI)         *four_over_pi);
            case 5: return  1.0/tan_56s((threehalfpi-x)*four_over_pi);
            case 6: return -1.0/tan_56s((x-threehalfpi)*four_over_pi);
            case 7: return -    tan_56s((twopi-x)      *four_over_pi);
        }
        return 0;
    }
    
    
    // *********************************************************
    // ***
    // ***   Routines to compute tangent to 8.2 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        tan_82s computes tan(pi*x/4)
    //
    //  Accurate to about 8.2 decimal digits over the range [0, pi/4].
    //  The input argument is in radians. Note that the function
    //  computes tan(pi*x/4), NOT tan(x); it's up to the range
    //  reduction algorithm that calls this to scale things properly.
    //
    //  Algorithm:
    //        tan(x)= x(c1 + c2*x**2)/(c3 + c4*x**2 + x**4)
    //
    static double tan_82s(double x)
    {
        const double c1= 211.849369664121;
        const double c2=- 12.5288887278448 ;
        const double c3= 269.7350131214121;
        const double c4=- 71.4145309347748;
        
        double x2;                            // The input argument squared
        
        x2=x * x;
        return (x*(c1 + c2 * x2)/(c3 + x2*(c4 + x2)));
    }
    
    //
    //  This is the main tangent approximation "driver"
    // It reduces the input argument's range to [0, pi/4],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    // Enter with positive angles only.
    //
    // WARNING: We do not test for the tangent approaching infinity,
    // which it will at x=pi/2 and x=3*pi/2. If this is a problem
    // in your application, take appropriate action.
    //
    double tan_82(double x){
        int octant;                        // what octant are we in?
        
        x=fmod(x, twopi);                // Get rid of values >2 *pi
        octant=(int)(x * four_over_pi);            // Get octant # (0 to 7)
        switch (octant){
            case 0: return      tan_82s(x              *four_over_pi);
            case 1: return  1.0/tan_82s((halfpi-x)     *four_over_pi);
            case 2: return -1.0/tan_82s((x-halfpi)     *four_over_pi);
            case 3: return -    tan_82s((PI-x)         *four_over_pi);
            case 4: return      tan_82s((x-PI)         *four_over_pi);
            case 5: return  1.0/tan_82s((threehalfpi-x)*four_over_pi);
            case 6: return -1.0/tan_82s((x-threehalfpi)*four_over_pi);
            case 7: return -    tan_82s((twopi-x)      *four_over_pi);
        }
        return 0;
    }
    
    // *********************************************************
    // ***
    // ***   Routines to compute tangent to 14 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        tan_14s computes tan(pi*x/4)
    //
    //  Accurate to about 14 decimal digits over the range [0, pi/4].
    //  The input argument is in radians. Note that the function
    //  computes tan(pi*x/4), NOT tan(x); it's up to the range
    //  reduction algorithm that calls this to scale things properly.
    //
    //  Algorithm:
    //        tan(x)= x(c1 + c2*x**2 + c3*x**4)/(c4 + c5*x**2 + c6*x**4 + x**6)
    //
    static double tan_14s(double x)
    {
        const double c1=-34287.4662577359568109624;
        const double c2=  2566.7175462315050423295;
        const double c3=-   26.5366371951731325438;
        const double c4=-43656.1579281292375769579;
        const double c5= 12244.4839556747426927793;
        const double c6=-  336.611376245464339493;
        
        double x2;                            // The input argument squared
        
        x2=x * x;
        return (x*(c1 + x2*(c2 + x2*c3))/(c4 + x2*(c5 + x2*(c6 + x2))));
    }
    
    //
    //  This is the main tangent approximation "driver"
    // It reduces the input argument's range to [0, pi/4],
    // and then calls the approximator.
    // See the notes for an explanation of the range reduction.
    // Enter with positive angles only.
    //
    // WARNING: We do not test for the tangent approaching infinity,
    // which it will at x=pi/2 and x=3*pi/2. If this is a problem
    // in your application, take appropriate action.
    //
    double tan_14(double x){
        int octant;                        // what octant are we in?
        
        x=fmod(x, twopi);                // Get rid of values >2 *pi
        octant=(int)(x * four_over_pi);            // Get octant # (0 to 7)
        switch (octant){
            case 0: return      tan_14s(x              *four_over_pi);
            case 1: return  1.0/tan_14s((halfpi-x)     *four_over_pi);
            case 2: return -1.0/tan_14s((x-halfpi)     *four_over_pi);
            case 3: return -    tan_14s((PI-x)         *four_over_pi);
            case 4: return      tan_14s((x-PI)         *four_over_pi);
            case 5: return  1.0/tan_14s((threehalfpi-x)*four_over_pi);
            case 6: return -1.0/tan_14s((x-threehalfpi)*four_over_pi);
            case 7: return -    tan_14s((twopi-x)      *four_over_pi);
        }
        return 0;
    }
    
    
    // *********************************************************
    // ***
    // ***   Routines to compute arctangent to 6.6 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        atan_66s computes atan(x)
    //
    //  Accurate to about 6.6 decimal digits over the range [0, pi/12].
    //
    //  Algorithm:
    //        atan(x)= x(c1 + c2*x**2)/(c3 + x**2)
    //
    static double atan_66s(double x)
    {
        const double c1=1.6867629106;
        const double c2=0.4378497304;
        const double c3=1.6867633134;
        
        
        double x2;                            // The input argument squared
        
        x2=x * x;
        return (x*(c1 + x2*c2)/(c3 + x2));
    }
    
    //
    //  This is the main arctangent approximation "driver"
    // It reduces the input argument's range to [0, pi/12],
    // and then calls the approximator.
    //
    //
    double atan_66(double x){
        double y;                            // return from atan__s function
        int complement= FALSE;                // true if arg was >1
        int region= FALSE;                    // true depending on region arg is in
        int sign= FALSE;                    // true if arg was < 0
        
        if (x <0 ){
            x=-x;
            sign=TRUE;                        // arctan(-x)=-arctan(x)
        }
        if (x > 1.0){
            x=1.0/x;                        // keep arg between 0 and 1
            complement=TRUE;
        }
        if (x > tantwelfthpi){
            x = (x-tansixthpi)/(1+tansixthpi*x);    // reduce arg to under tan(pi/12)
            region=TRUE;
        }
        
        y=atan_66s(x);                        // run the approximation
        if (region) y+=sixthpi;                // correct for region we're in
        if (complement)y=halfpi-y;            // correct for 1/x if we did that
        if (sign)y=-y;                        // correct for negative arg
        return (y);
        
    }
    
    // *********************************************************
    // ***
    // ***   Routines to compute arctangent to 13.7 digits
    // ***  of accuracy.
    // ***
    // *********************************************************
    //
    //        atan_137s computes atan(x)
    //
    //  Accurate to about 13.7 decimal digits over the range [0, pi/12].
    //
    //  Algorithm:
    //        atan(x)= x(c1 + c2*x**2 + c3*x**4)/(c4 + c5*x**2 + c6*x**4 + x**6)
    //
    static double atan_137s(double x)
    {
        const double c1= 48.70107004404898384;
        const double c2= 49.5326263772254345;
        const double c3=  9.40604244231624;
        const double c4= 48.70107004404996166;
        const double c5= 65.7663163908956299;
        const double c6= 21.587934067020262;
        
        double x2;                            // The input argument squared
        
        x2=x * x;
        return (x*(c1 + x2*(c2 + x2*c3))/(c4 + x2*(c5 + x2*(c6 + x2))));
    }
    
    //
    //  This is the main arctangent approximation "driver"
    // It reduces the input argument's range to [0, pi/12],
    // and then calls the approximator.
    //
    //
    double atan_137(double x){
        double y;                            // return from atan__s function
        int complement= FALSE;                // true if arg was >1
        int region= FALSE;                    // true depending on region arg is in
        int sign= FALSE;                    // true if arg was < 0
        
        if (x <0 ){
            x=-x;
            sign=TRUE;                        // arctan(-x)=-arctan(x)
        }
        if (x > 1.0){
            x=1.0/x;                        // keep arg between 0 and 1
            complement=TRUE;
        }
        if (x > tantwelfthpi){
            x = (x-tansixthpi)/(1+tansixthpi*x);    // reduce arg to under tan(pi/12)
            region=TRUE;
        }
        
        y=atan_137s(x);                        // run the approximation
        if (region) y+=sixthpi;                // correct for region we're in
        if (complement)y=halfpi-y;            // correct for 1/x if we did that
        if (sign)y=-y;                        // correct for negative arg
        return (y);
    }
    
    float cos_j(float x) {
        if (x < -PI) {
            while (x < -PI) {
                x += TWO_PI;
            }
        } else if (x > PI) {
            while (x > PI) {
                x -= TWO_PI;
            }
        }
        const float x2 = x * x;
        const float numerator = -(-39251520 + x2 * (18471600 + x2 * (-1075032 + 14615 * x2)));
        const float denominator = 39251520 + x2 * (1154160 + x2 * (16632 + x2 * 127));
        return numerator / denominator;
    }
    
    float sin_j(float x) {
        if (x < -PI) {
            while (x < -PI) {
                x += TWO_PI;
            }
        } else if (x > PI) {
            while (x > PI) {
                x -= TWO_PI;
            }
        }
        const float x2 = x * x;
        const float numerator = -x * (-11511339840l + x2 * (1640635920 + x2 * (-52785432 + x2 * 479249)));
        const float denominator = 11511339840l + x2 * (277920720 + x2 * (3177720 + x2 * 18361));
        return numerator / denominator;
    }
    
    float tan_j(float x) {
        auto x2 = x * x;
        auto numerator = x * (-135135 + x2 * (17325 + x2 * (-378 + x2)));
        auto denominator = -135135 + x2 * (62370 + x2 * (-3150 + 28 * x2));
        return numerator / denominator;
    }
    
    float sinh_j (float x) {
        auto x2 = x * x;
        auto numerator = -x * (11511339840 + x2 * (1640635920 + x2 * (52785432 + x2 * 479249)));
        auto denominator = -11511339840 + x2 * (277920720 + x2 * (-3177720 + x2 * 18361));
        return numerator / denominator;
    }
    
    float cosh_j (float x) {
        auto x2 = x * x;
        auto numerator = -(39251520 + x2 * (18471600 + x2 * (1075032 + 14615 * x2)));
        auto denominator = -39251520 + x2 * (1154160 + x2 * (-16632 + 127 * x2));
        return numerator / denominator;
    }
    
    float tanh_j(float x) {
        auto x2 = x * x;
        auto numerator = x * (135135 + x2 * (17325 + x2 * (378 + x2)));
        auto denominator = 135135 + x2 * (62370 + x2 * (3150 + 28 * x2));
        return numerator / denominator;
    }
    
    float exp_j(float x) {
        auto numerator = 1680 + x * (840 + x * (180 + x * (20 + x)));
        auto denominator = 1680 + x *(-840 + x * (180 + x * (-20 + x)));
        return numerator / denominator;
    }
    
    float __inv_sqrt(float x) {
        float xhalf = 0.5f*x;
        int i = *(int*)&x;
        i = 0x5f3759df - (i>>1);
        x = *(float*)&i; // @note(original code … breaks strict-aliasing rules)
        return x*(1.5f - xhalf*x*x);
    }
    
    /* ----------------------------------------------------------------------------------------------------- */
    
    float klang_math_fast_sqrt(float x) {
        return 1.0/__inv_sqrt(x);
    }
    
    float klang_math_sin(float r) {
        return sin_j(r);
        //        return sin_32(r);
    }
    
    float klang_math_cos(float r) {
        return cos_j(r);
        //        return cos_32(r);
    }
    
    float klang_math_tan(float r) {
        return tan_j(r);
        //        return tan_32(r);
    }
    
    float klang_math_sinh (float x) {
        return sinh_j(x);
    }
    
    float klang_math_cosh (float x) {
        return cosh_j(x);
    }

#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

//    float synthesizer_util_sin_fast(float r) {
//        /* wrap r to -PI..PI */
//        if (r < -PI) {
//            while (r < -PI) {
//                r += TWO_PI;
//            }
//        } else if (r > PI) {
//            while (r > PI) {
//                r -= TWO_PI;
//            }
//        }
//
//        const float B = 4/PI;
//        const float C = -4/(PI*PI);
//        float y = B * r + C * r * fabs(r);
//
//        return y;
//    }
//
//    float synthesizer_util_cos_fast(float pRad) {
//        return synthesizer_util_sin_fast(pRad + HALF_PI);
//    }
//
//    float synthesizer_util_sin_fast_precise(float r) {
//        /* wrap r to -PI..PI */
//        if (r < -PI) {
//            while (r < -PI) {
//                r += TWO_PI;
//            }
//        } else if (r > PI) {
//            while (r > PI) {
//                r -= TWO_PI;
//            }
//        }
//
//        const float B = 4/PI;
//        const float C = -4/(PI*PI);
//        float y = B * r + C * r * fabs(r);
//
//#define EXTRA_PRECISION TRUE
//#if (EXTRA_PRECISION)
//        const float P = 0.225f;
//        y = P * (y * fabs(y) - y) + y;
//#endif
//        return y;
//    }
//
//    float synthesizer_util_cos_fast_precise(float pRad) {
//        return synthesizer_util_sin_fast_precise(pRad + HALF_PI);
//    }
    
#ifdef __cplusplus
}
#endif
