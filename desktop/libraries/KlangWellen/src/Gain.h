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
    class Gain {
    private:
        float mGain;

    public:
        Gain() : mGain(1.0f) {}

        float get_gain() {
            return mGain;
        }

        void set_gain(float pGain) {
            mGain = pGain;
        }

        float process(float signal) {
            return signal * mGain;
        }

        void process(Signal& signal) {
            signal.left *= mGain;
            signal.right *= mGain;
        }

        void process(float*         signal_buffer_left,
                     float*         signal_buffer_right,
                     const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (int i = 0; i < buffer_length; i++) {
                signal_buffer_left[i] *= mGain;
                signal_buffer_right[i] *= mGain;
            }
        }

        void process(float*         signal_buffer,
                     const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (int i = 0; i < buffer_length; i++) {
                signal_buffer[i] *= mGain;
            }
        }
    };
}  // namespace klangwellen
