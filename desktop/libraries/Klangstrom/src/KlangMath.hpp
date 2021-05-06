//
//  Math.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef Math_hpp
#define Math_hpp

#include <float.h>
#include "Klang.hpp"

#ifndef PI
#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
#define TWO_PI   6.28318530717958647693
#endif

#ifndef M_TWO_PI
#define M_TWO_PI   6.28318530717958647693
#endif

#ifndef HALF_PI
#define HALF_PI  1.57079632679489661923
#endif

#ifdef __cplusplus
extern "C" {
#endif

    float klang_math_sin(float r);
    float klang_math_cos(float r);
    float klang_math_tan(float r);
    float klang_math_sinh(float x);
    float klang_math_cosh(float x);
    float klang_math_fast_sqrt(float x);

#ifdef __cplusplus
}
#endif

namespace klang {
    class KlangMath {
    public:
        static inline uint8_t LO_16i(uint16_t i) {
            return (i & 0xFF);
        }

        static inline uint8_t HI_16i(uint16_t i) {
            return ((i >> 8) & 0xFF);
        }

        static inline uint16_t INT_16i(uint8_t pHighByte, uint8_t pLowByte) {
            return (pHighByte << 8) + pLowByte;
        }

        static inline float FLOAT_32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
            float output;

            *((uint8_t*)(&output) + 0) = b0;
            *((uint8_t*)(&output) + 1) = b1;
            *((uint8_t*)(&output) + 2) = b2;
            *((uint8_t*)(&output) + 3) = b3;

            return output;
        }

        static inline float FLOAT_32(uint8_t* pBytes) {
            float output;

            *((uint8_t*)(&output) + 0) = pBytes[0];
            *((uint8_t*)(&output) + 1) = pBytes[1];
            *((uint8_t*)(&output) + 2) = pBytes[2];
            *((uint8_t*)(&output) + 3) = pBytes[3];

            return output;
        }

        static inline float FLOAT_32(uint8_t* pBytes, uint32_t pOffset) {
            float output;

            *((uint8_t*)(&output) + 0) = pBytes[0 + pOffset];
            *((uint8_t*)(&output) + 1) = pBytes[1 + pOffset];
            *((uint8_t*)(&output) + 2) = pBytes[2 + pOffset];
            *((uint8_t*)(&output) + 3) = pBytes[3 + pOffset];

            return output;
        }

        static inline uint8_t* FLOAT_BYTES_32(float& pFloat) {
            return (uint8_t*)&pFloat;
        }

        /**
         * wraps an index to buffer length. buffer length is required to be *power of 2*
         */
        static inline float wrap_float_index(const float pFloatIndex, const uint16_t pBufferLength) {
            const uint16_t mInt = (uint16_t)pFloatIndex;
            const float mFrac = pFloatIndex - mInt;
            const uint16_t mNextInt = mInt & (pBufferLength - 1);
            return mNextInt + mFrac;
        }

        /**
         * returns a random number between -1 ... 1
         */
        static float random() {
            // TODO replace with mapping, without division
            return ((float)xorshift32() / UINT32_MAX) * 2 - 1;
        }

        /**
         * returns a random number between 0 ... 1
         */
        static float random_normalized() {
            // TODO replace with mapping, without division
            return ((float)xorshift32() / UINT32_MAX);
        }

        /* xorshift32 ( ref: https://en.wikipedia.org/wiki/Xorshift ) */
        static uint32_t x32Seed;
        static uint32_t xorshift32() {
            x32Seed ^= x32Seed << 13;
            x32Seed ^= x32Seed >> 17;
            x32Seed ^= x32Seed << 5;
            return x32Seed;
        }

        static uint8_t power_of_2(uint16_t p) {
            uint8_t c = 0;
            while ((p /= 2) > 0) {
                c++;
            }
            return c;
        }

        static uint16_t shift(float pValue, uint8_t pShifts) {
            for (uint8_t i = 0; i < pShifts; i++) {
                pValue *= 2;
            }
            return (uint16_t) pValue;
        }

        static SIGNAL_TYPE clamp(SIGNAL_TYPE pValue, SIGNAL_TYPE pMin, SIGNAL_TYPE pMax) {
            return pValue > pMax ? pMax : (pValue < pMin ? pMin : pValue);
        }

        static SIGNAL_TYPE clamp(SIGNAL_TYPE pValue) {
            return pValue > SIGNAL_MAX ? SIGNAL_MAX : (pValue < SIGNAL_MIN ? SIGNAL_MIN : pValue);
        }

        static SIGNAL_TYPE clamp(uint8_t pValue, uint8_t pMin, uint8_t pMax) {
            return pValue > pMax ? pMax : (pValue < pMin ? pMin : pValue);
        }

        static int clamp127(uint8_t pValue) {
            return pValue > 127 ? 127 : pValue;
        }

        static SIGNAL_TYPE clamp_signal(SIGNAL_TYPE pValue) {
            return pValue > SIGNAL_MAX ? SIGNAL_MAX : (pValue < SIGNAL_MIN ? SIGNAL_MIN : pValue);
        }

        static float lerp(float v0, float v1, float t) {
            return v0 + t * (v1 - v0);
        }

        static float map(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
            return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
        }

        static int16_t map_i16(int16_t value, int16_t inputMin, int16_t inputMax, int16_t outputMin, int16_t outputMax) {
            return ((value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin);
        }

#define USE_FMOD_WITH_WHILE TRUE
        static float mod(float a, float b) {
#if USE_FMOD_WITH_WHILE
            while(a >= b) {
                a -= b;
            }
            return a;
#else
            return a >= b ? fmodf(a, b) : a;
            //            return input >= ceil ? input % ceil : input; // from https://stackoverflow.com/questions/33333363/built-in-mod-vs-custom-mod-function-improve-the-performance-of-modulus-op
#endif
        }

        static constexpr float MIDI_NOTE_CONVERSION_BASE_FREQUENCY = 440.0;
        static const uint8_t NOTE_OFFSET = 69;
        static float note_to_frequency(uint8_t pMidiNote) {
            return MIDI_NOTE_CONVERSION_BASE_FREQUENCY * pow(2, ((pMidiNote - NOTE_OFFSET) / 12.0));
        }
    };

   /**
    * @file    SimplexNoise.h
    * @brief   A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D, 4D).
    *
    * Copyright (c) 2014-2015 Sebastien Rombauts (sebastien.rombauts@gmail.com)
    *
    * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
    * or copy at http://opensource.org/licenses/MIT)
    */

   /**
    * @brief A Perlin Simplex Noise C++ Implementation (1D, 2D, 3D, 4D).
    */
   class SimplexNoise {
   public:
       // 1D Perlin simplex noise
       static float noise(float x);
       // 2D Perlin simplex noise
       static float noise(float x, float y);

       static float noise_normalized(float x) { return noise(x) * 0.5 + 0.5; }
       static float noise_normalized(float x, float y) { return noise(x, y) * 0.5 + 0.5; }
   };
}

#endif /* Math_hpp */
