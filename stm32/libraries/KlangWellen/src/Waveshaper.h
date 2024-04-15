/*
 * KlangWellen
 *
 * This file is part of the *KlangWellen* library (https://github.com/dennisppaul/klangwellen).
 * Copyright (c) 2023 Dennis P Paul
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
 * - [x] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 */

#pragma once

#include <stdint.h>

#include "KlangWellen.h"
namespace klangwellen {

    class Waveshaper {
    public:
        static const uint8_t SIN                  = 0;
        static const uint8_t ATAN                 = 1;
        static const uint8_t TAN_H                = 2;
        static const uint8_t CUBIC                = 3;
        static const uint8_t HARDCLIP             = 4;
        static const uint8_t NUM_WAVESHAPER_TYPES = 5;

        Waveshaper() : fAmount(1.0),
                       fOutputGain(1.0f),
                       fBias(0.0),
                       fType(ATAN),
                       fOneOverAtanAmount(1.f / (KlangWellen::fast_atan(fAmount))),
                       fOneOverTanhAmount(1.f / KlangWellen::fast_tanh(fAmount)) {
        }

        void set_amount(const float amount) {
            // 0.0 means no effect
            fAmount            = KlangWellen::max(amount, 1.0);
            fOneOverAtanAmount = 1.0 / KlangWellen::fast_atan(fAmount);
            fOneOverTanhAmount = 1.0 / KlangWellen::fast_tanh(fAmount);
        }

        void set_bias(const float bias) {
            fBias = KlangWellen::clamp(bias);
        }

        void set_output_gain(const float gain) {
            fOutputGain = gain;
        }

        void set_type(const uint8_t type) {
            fType = type;
        }

        void process(float* signal_buffer, const uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            switch (fType) {
                case ATAN:
                    for (uint32_t i = 0; i < length; i++) {
                        signal_buffer[i] = ProcessATan(signal_buffer[i]);
                    }
                    break;
                case CUBIC:
                    for (uint32_t i = 0; i < length; i++) {
                        signal_buffer[i] = ProcessCubic(signal_buffer[i]);
                    }
                    break;
                case SIN:
                    for (uint32_t i = 0; i < length; i++) {
                        signal_buffer[i] = ProcessSin(signal_buffer[i]);
                    }
                    break;
                case HARDCLIP:
                    for (uint32_t i = 0; i < length; i++) {
                        signal_buffer[i] = ProcessHardClip(signal_buffer[i]);
                    }
                    break;
                case TAN_H:
                default:
                    for (uint32_t i = 0; i < length; i++) {
                        signal_buffer[i] = ProcessTanh(signal_buffer[i]);
                    }
                    break;
            }
        }

        float process(float sample) {
            switch (fType) {
                case ATAN:
                    return ProcessATan(sample);
                case CUBIC:
                    return ProcessCubic(sample);
                case SIN:
                    return ProcessSin(sample);
                case HARDCLIP:
                    return ProcessHardClip(sample);
                case TAN_H:
                default:
                    return ProcessTanh(sample);
            }
        }

    private:
        float   fAmount;
        float   fOutputGain;
        float   fBias;
        uint8_t fType;
        float   fOneOverAtanAmount;
        float   fOneOverTanhAmount;

        float ProcessHardClip(float signal_buffer) {
            return fOutputGain * KlangWellen::clamp(fAmount * (signal_buffer + fBias));
        }

        float ProcessTanh(float signal_buffer) {
            return fOutputGain * KlangWellen::fast_tanh((signal_buffer + fBias) * fAmount) * fOneOverTanhAmount;
        }

        float ProcessATan(float signal_buffer) {
            return fOutputGain * KlangWellen::fast_atan((signal_buffer + fBias) * fAmount) * fOneOverAtanAmount;
        }

        float ProcessCubic(float signal_buffer) {
            static const float CubicMax = 2.f / 3.f;
            static const float OneThird = 1.f / 3.f;

            float mSample = (signal_buffer + fBias) * fAmount;

            if (KlangWellen::abs(mSample) > 1.f) {
                mSample = KlangWellen::sign(mSample) * CubicMax;
            } else {
                mSample = mSample - (mSample * mSample * mSample * OneThird);
            }

            mSample *= fOutputGain;
            return mSample;
        }

        float ProcessSin(float signal_buffer) {
            float mSample = (signal_buffer + fBias) * fAmount;

            if (KlangWellen::abs(mSample) > HALF_PI) {
                mSample = KlangWellen::sign(mSample);
            } else {
                mSample = KlangWellen::fast_sin3(mSample);
            }

            mSample *= fOutputGain;
            return mSample;
        }
    };
}  // namespace klangwellen
