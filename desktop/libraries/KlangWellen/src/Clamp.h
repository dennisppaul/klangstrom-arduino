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
 * - [x] float process(float)‌
 * - [x] void process(Signal&)
 * - [x] void process(float*, uint32_t)
 * - [x] void process(float*, float*, uint32_t)
 */

#pragma once

#include <stdint.h>

#include "Signal.h"

namespace klangwellen {
    class Clamp {
    public:
        Clamp() : fMin(-1.0), fMax(1.0) {}

        float process(float signal) {
            return clamp(signal, fMin, fMax);
        }

        void process(Signal& signal) {
            signal.left  = clamp(signal.left, fMin, fMax);
            signal.right = clamp(signal.right, fMin, fMax);
        }

        void process(float*         signal_buffer,
                     const uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                signal_buffer[i] = clamp(signal_buffer[i], fMin, fMax);
            }
        }

        void process(float*         signal_buffer_left,
                     float*         signal_buffer_right,
                     const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < buffer_length; i++) {
                signal_buffer_left[i]  = clamp(signal_buffer_left[i], fMin, fMax);
                signal_buffer_right[i] = clamp(signal_buffer_right[i], fMin, fMax);
            }
        }

        void set_min(const float min) {
            fMin = min;
        }

        void set_max(const float max) {
            fMax = max;
        }

        float get_min() {
            return fMin;
        }

        float get_max() {
            return fMax;
        }

    private:
        float fMin;
        float fMax;

        float clamp(const float value, const float min, const float max) {
            return value > max ? max : (value < min ? min : value);
        }
    };
}  // namespace klangwellen