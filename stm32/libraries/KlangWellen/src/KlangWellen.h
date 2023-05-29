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

#pragma once

#include <math.h>
#include <stdint.h>

#include <algorithm>
#include <limits>

#ifndef KLANG_SAMPLES_PER_AUDIO_BLOCK
#warning KLANG_SAMPLES_PER_AUDIO_BLOCK not defined - using default value of 512
#define KLANG_SAMPLES_PER_AUDIO_BLOCK 512
#endif

#ifndef PI
#define PI M_PI
#endif

#ifndef TWO_PI
#define TWO_PI (M_PI * 2)
#endif

#ifndef HALF_PI
#define HALF_PI (M_PI / 2)
#endif

namespace klangwellen {
    class KlangWellen {
    public:
        static const uint8_t      BITS_PER_SAMPLE_16                    = 16;
        static const uint8_t      BITS_PER_SAMPLE_24                    = 24;
        static const uint8_t      BITS_PER_SAMPLE_32                    = 32;
        static const uint8_t      BITS_PER_SAMPLE_8                     = 8;
        static constexpr float    DEFAULT_ATTACK                        = 0.005f;
        static const int          DEFAULT_AUDIOBLOCK_SIZE               = 1024;
        static const int          DEFAULT_AUDIO_DEVICE                  = -1;
        static const uint8_t      DEFAULT_BITS_PER_SAMPLE               = BITS_PER_SAMPLE_16;
        static constexpr float    DEFAULT_DECAY                         = 0.01f;
        static constexpr float    DEFAULT_FILTER_BANDWIDTH              = 100.0f;
        static constexpr float    DEFAULT_FILTER_FREQUENCY              = 1000.0f;
        static constexpr float    DEFAULT_RELEASE                       = 0.075f;
        static constexpr uint32_t DEFAULT_SAMPLING_RATE                 = 48000;
        static constexpr uint32_t DEFAULT_INTERPOLATE_AMP_FREQ_DURATION = 5.f / 1000.f * (float)DEFAULT_SAMPLING_RATE;  // KlangWellen::millis_to_samples(5);
        static constexpr float    DEFAULT_SUSTAIN                       = 0.5f;
        static const int          DEFAULT_WAVETABLE_SIZE                = 512;
        static const uint8_t      DISTORTION_HARD_CLIPPING              = 0;
        static const uint8_t      DISTORTION_FOLDBACK                   = 1;
        static const uint8_t      DISTORTION_FOLDBACK_SINGLE            = 2;
        static const uint8_t      DISTORTION_FULL_WAVE_RECTIFICATION    = 3;
        static const uint8_t      DISTORTION_HALF_WAVE_RECTIFICATION    = 4;
        static const uint8_t      DISTORTION_INFINITE_CLIPPING          = 5;
        static const uint8_t      DISTORTION_SOFT_CLIPPING_CUBIC        = 6;
        static const uint8_t      DISTORTION_SOFT_CLIPPING_ARC_TANGENT  = 7;
        static const uint8_t      DISTORTION_BIT_CRUSHING               = 8;
        static const int          EVENT_UNDEFINED                       = -1;
        static const int          EVENT_CHANNEL                         = 0;
        static const int          EVENT_NOTE_ON                         = 0;
        static const int          EVENT_NOTE                            = 1;
        static const int          EVENT_NOTE_OFF                        = 1;
        static const int          EVENT_CONTROLCHANGE                   = 2;
        static const int          EVENT_PITCHBEND                       = 3;
        static const int          EVENT_PROGRAMCHANGE                   = 4;
        static const int          EVENT_VELOCITY                        = 2;
        static const uint8_t      FILTER_MODE_LOW_PASS                  = 0;
        static const uint8_t      FILTER_MODE_HIGH_PASS                 = 1;
        static const uint8_t      FILTER_MODE_BAND_PASS                 = 2;
        static const uint8_t      FILTER_MODE_NOTCH                     = 3;
        static const uint8_t      FILTER_MODE_PEAK                      = 4;
        static const uint8_t      FILTER_MODE_LOWSHELF                  = 5;
        static const uint8_t      FILTER_MODE_HIGHSHELF                 = 6;
        static const uint8_t      FILTER_MODE_BAND_REJECT               = 7;
        static const int          LOOP_INFINITE                         = std::numeric_limits<int>::max();
        static const uint8_t      MONO                                  = 1;
        static const uint8_t      NOISE_WHITE                           = 0;
        static const uint8_t      NOISE_GAUSSIAN_WHITE                  = 1;
        static const uint8_t      NOISE_GAUSSIAN_WHITE2                 = 2;
        static const uint8_t      NOISE_PINK                            = 3;
        static const uint8_t      NOISE_PINK2                           = 4;
        static const uint8_t      NOISE_PINK3                           = 5;
        static const uint8_t      NOISE_SIMPLEX                         = 6;
        static constexpr float    NOTE_WHOLE                            = 0.25f;
        static constexpr float    NOTE_HALF                             = 0.5f;
        static const uint8_t      NOTE_QUARTER                          = 1;
        static const uint8_t      NOTE_EIGHTH                           = 2;
        static const uint8_t      NOTE_SIXTEENTH                        = 4;
        static const uint8_t      NOTE_THIRTYSECOND                     = 8;
        static const int          NO_AUDIO_DEVICE                       = -2;
        static const int          NO_CHANNELS                           = 0;
        static const int          NO_EVENT                              = -1;
        static const int          NO_INPOINT                            = 0;
        static const int          NO_LOOP                               = -2;
        static const int          NO_LOOP_COUNT                         = -1;
        static const int          NO_OUTPOINT                           = -1;
        static const int          NO_POSITION                           = -1;
        static const int          NO_VALUE                              = -1;
        static const uint8_t      PAN_LINEAR                            = 0;
        static const uint8_t      PAN_SINE_LAW                          = 2;
        static const uint8_t      PAN_SQUARE_LAW                        = 1;
        static const uint8_t      SIGNAL_LEFT                           = 0;
        static constexpr float    SIGNAL_MAX                            = 1.0f;
        static constexpr float    SIGNAL_MIN                            = -1.0f;
        static const int          SIGNAL_MONO                           = 1;
        static const int          SIGNAL_PROCESSING_IGNORE_IN_OUTPOINTS = -3;
        static const int          SIGNAL_RIGHT                          = 1;
        static const int          SIGNAL_STEREO                         = 2;
        static const uint8_t      SIG_INT16_BIG_ENDIAN                  = 2;
        static const uint8_t      SIG_INT16_LITTLE_ENDIAN               = 3;
        static const uint8_t      SIG_INT24_3_BIG_ENDIAN                = 4;
        static const uint8_t      SIG_INT24_3_LITTLE_ENDIAN             = 5;
        static const uint8_t      SIG_INT24_4_BIG_ENDIAN                = 6;
        static const uint8_t      SIG_INT24_4_LITTLE_ENDIAN             = 7;
        static const uint8_t      SIG_INT32_BIG_ENDIAN                  = 8;
        static const uint8_t      SIG_INT32_LITTLE_ENDIAN               = 9;
        static const uint8_t      SIG_INT8                              = 0;
        static const uint8_t      SIG_UINT8                             = 1;
        static const uint8_t      STEREO                                = 2;
        static const uint8_t      VERSION_MAJOR                         = 0;
        static const uint8_t      VERSION_MINOR                         = 8;
        static const uint8_t      WAVEFORM_SINE                         = 0;
        static const uint8_t      WAVEFORM_TRIANGLE                     = 1;
        static const uint8_t      WAVEFORM_SAWTOOTH                     = 2;
        static const uint8_t      WAVEFORM_SQUARE                       = 3;
        static const uint8_t      WAVEFORM_NOISE                        = 4;
        static const uint8_t      WAVESHAPE_INTERPOLATE_NONE            = 0;
        static const uint8_t      WAVESHAPE_INTERPOLATE_LINEAR          = 1;
        static const uint8_t      WAVESHAPE_INTERPOLATE_CUBIC           = 2;
        static const uint8_t      WAV_FORMAT_PCM                        = 1;
        static const uint8_t      WAV_FORMAT_IEEE_FLOAT_32BIT           = 3;

        /* --- sound --- */

        static constexpr float MIDI_NOTE_CONVERSION_BASE_FREQUENCY = 440.0;
        static const uint8_t   NOTE_OFFSET                         = 69;
        static float           note_to_frequency(uint8_t pMidiNote) {
            return MIDI_NOTE_CONVERSION_BASE_FREQUENCY * pow(2, ((pMidiNote - NOTE_OFFSET) / 12.0));
        }

        /* --- math --- */

        static uint32_t millis_to_samples(float pMillis, float pSamplingRate);
        static uint32_t millis_to_samples(float pMillis);
        static float    random_normalized();
        static uint32_t x32Seed;
        static uint32_t xorshift32();
        static float    random();

        inline static float clamp(float value,
                                  float minimum,
                                  float maximum) {
            return value > maximum ? maximum : (value < minimum ? minimum : value);
        }

        inline static float clamp(float value) {
            return value > SIGNAL_MAX ? SIGNAL_MAX : (value < SIGNAL_MIN ? SIGNAL_MIN : value);
        }

        inline static uint8_t clamp127(uint8_t value) {
            return value > 127 ? 127 : value;
        }

        template <typename T>
        inline static T clamp(T value, T minimum, T maximum) {
            return value > maximum ? maximum : (value < minimum ? minimum : value);
        }

        inline static float pow(float base, float exponent) {
            return powf(base, exponent);
        }

        inline static float lerp(float v0, float v1, float t) {
            return v0 + t * (v1 - v0);
        }

        inline static float max(float a, float b) {
            return a > b ? a : b;
        }

        inline static float min(float a, float b) {
            return a < b ? a : b;
        }

        inline static float abs(float value) {
            return value > 0 ? value : -value;
        }

        template <typename T>
        inline static T abs(T value) {
            return value > 0 ? value : -value;
        }

        template <typename T>
        inline static int sign(T val) {
            return (T(0) < val) - (val < T(0));
        }

        inline static float mod(float a, float b) {
            return a >= b ? (a - b * int(a / b)) : a;
            // return a >= b ? fmodf(a, b) : a;
            // return input >= ceil ? input % ceil : input;
            // from https://stackoverflow.com/questions/33333363/built-in-mod-vs-custom-mod-function-improve-the-performance-of-modulus-op
        }

        inline static float map(float value,
                                float inputMin,
                                float inputMax,
                                float outputMin,
                                float outputMax) {
            return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
        }

        inline static int16_t map_i16(int16_t value,
                                      int16_t inputMin,
                                      int16_t inputMax,
                                      int16_t outputMin,
                                      int16_t outputMax) {
            return ((value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin);
        }

        inline static float sin(float r) {
            return sinf(r);
        }

        inline static float cos(float r) {
            return cosf(r);
        }

        inline static float sinh(float r) {
            return sinhf(r);
        }

        inline static float cosh(float r) {
            return coshf(r);
        }

        inline static float sqrt(float r) {
            return sqrtf(r);
        }

        inline static float fast_sqrt(float x) {
            return 1.0 / fast_inv_sqrt(x);
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
        inline static float fast_inv_sqrt(float x) {
            float xhalf = 0.5f * x;
            int   i     = *(int*)&x;
            i           = 0x5f3759df - (i >> 1);
            x           = *(float*)&i;
            return x * (1.5f - xhalf * x * x);
        }
#pragma GCC diagnostic pop

        inline static float fast_sin(float x) {
            if (x < -PI) {
                while (x < -PI) {
                    x += TWO_PI;
                }
            } else if (x > PI) {
                while (x > PI) {
                    x -= TWO_PI;
                }
            }
            const float x2          = x * x;
            const float numerator   = -x * (-11511339840l + x2 * (1640635920 + x2 * (-52785432 + x2 * 479249)));
            const float denominator = 11511339840l + x2 * (277920720 + x2 * (3177720 + x2 * 18361));
            return numerator / denominator;
        }

        inline static float fast_sin3(const float X) {
            // Valid on interval [-PI, PI]
            // Sine approximation using Bhaskara I technique discovered in 7th century.
            // https://en.wikipedia.org/wiki/Bh%C4%81skara_I
            const float AbsX        = abs(X);
            const float Numerator   = 16.0f * X * (PI - AbsX);
            const float Denominator = 5.0f * PI * PI - 4.0f * AbsX * (PI - AbsX);
            return Numerator / Denominator;
        }

        inline static float fast_cos(float x) {
            if (x < -PI) {
                while (x < -PI) {
                    x += TWO_PI;
                }
            } else if (x > PI) {
                while (x > PI) {
                    x -= TWO_PI;
                }
            }
            const float x2          = x * x;
            const float numerator   = -(-39251520 + x2 * (18471600 + x2 * (-1075032 + 14615 * x2)));
            const float denominator = 39251520 + x2 * (1154160 + x2 * (16632 + x2 * 127));
            return numerator / denominator;
        }

        inline static float fast_sinh(float x) {
            auto x2          = x * x;
            auto numerator   = -x * (11511339840 + x2 * (1640635920 + x2 * (52785432 + x2 * 479249)));
            auto denominator = -11511339840 + x2 * (277920720 + x2 * (-3177720 + x2 * 18361));
            return numerator / denominator;
        }

        inline static float fast_cosh(float x) {
            auto x2          = x * x;
            auto numerator   = -(39251520 + x2 * (18471600 + x2 * (1075032 + 14615 * x2)));
            auto denominator = -39251520 + x2 * (1154160 + x2 * (-16632 + 127 * x2));
            return numerator / denominator;
        }

        inline static float fast_tan(float x) {
            auto x2          = x * x;
            auto numerator   = x * (-135135 + x2 * (17325 + x2 * (-378 + x2)));
            auto denominator = -135135 + x2 * (62370 + x2 * (-3150 + 28 * x2));
            return numerator / denominator;
        }

        inline static float fast_tanh(float x) {
            auto x2          = x * x;
            auto numerator   = x * (135135 + x2 * (17325 + x2 * (378 + x2)));
            auto denominator = 135135 + x2 * (62370 + x2 * (3150 + 28 * x2));
            return numerator / denominator;
        }

        inline static float exp_j(float x) {
            auto numerator   = 1680 + x * (840 + x * (180 + x * (20 + x)));
            auto denominator = 1680 + x * (-840 + x * (180 + x * (-20 + x)));
            return numerator / denominator;
        }

        inline static float fast_atan(float x) {
            static const float PI_FOURTH = (PI / 4.0);
            return PI_FOURTH * x - x * (fabs(x) - 1) * (0.2447 + 0.0663 * fabs(x));
        }

        inline static float fast_atan2(float x) {
            static const float PI_FOURTH = (PI / 4.0);
            static const float A         = 0.0776509570923569;
            static const float B         = -0.287434475393028;
            static const float C         = (PI_FOURTH - A - B);
            float              xx        = x * x;
            return ((A * xx + B) * xx + C) * x;
        }

        /* --- buffer --- */
        inline static void fill(float* buffer, float value, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer[i] = value;
            }
        }

        /**
         * copies src into dst. dst will be overwritten.
         */
        inline static void copy(float* src, float* dst, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            std::copy(src, src + length, dst);
        }

        /**
         * adds buffer_b to buffer_a. result will be stored in buffer_a, buffer_b will not be changed.
         */
        inline static void add(float* buffer_a, float* buffer_b, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] += buffer_b[i];
            }
        }

        inline static void add(float* buffer_a, float scalar, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] += scalar;
            }
        }

        /**
         * subtracts buffer_b from buffer_a. result will be stored in buffer_a, buffer_b will not be changed.
         */
        inline static void sub(float* buffer_a, float* buffer_b, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] -= buffer_b[i];
            }
        }

        inline static void sub(float* buffer_a, float scalar, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] -= scalar;
            }
        }

        /**
         * multiplies buffer_b with buffer_a. result will be stored in buffer_a, buffer_b will not be changed.
         */
        inline static void mult(float* buffer_a, float* buffer_b, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] *= buffer_b[i];
            }
        }

        inline static void mult(float* buffer_a, float scalar, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] *= scalar;
            }
        }

        /**
         * divides buffer_b from buffer_a. result will be stored in buffer_a, buffer_b will not be changed.
         */
        inline static void div(float* buffer_a, float* buffer_b, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] /= buffer_b[i];
            }
        }

        inline static void div(float* buffer_a, float scalar, uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                buffer_a[i] /= scalar;
            }
        }
    };
}  // namespace klangwellen
