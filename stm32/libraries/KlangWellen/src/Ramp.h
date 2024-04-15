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
 * - [x] float process()
 * - [ ] float process(float)‌
 * - [ ] void process(Signal&)
 * - [x] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 */

#pragma once

#include <stdint.h>
#include <stdio.h>

#include "KlangWellen.h"
#include "Signal.h"

namespace klangwellen {
    class Ramp {
    public:
        Ramp(uint32_t sample_rate = KlangWellen::DEFAULT_SAMPLING_RATE) : fSampleRate(sample_rate) {
            fStartValue   = 0;
            fEndValue     = 0;
            fCurrentValue = 0;
            fDuration     = 0;
            fIsDone       = true;
        }

        void set_start(float start_value) {
            fStartValue = start_value;
        }

        void set_end(float end_value) {
            fEndValue = end_value;
        }

        /**
         * duration in sec
         */
        void set_duration(float duration) {
            fDuration = duration;
        }

        void set(float start_value, float end_value, float duration) {
            fStartValue = start_value;
            fEndValue   = end_value;
            fDuration   = duration;
        }

        float process() {
            if (!fIsDone) {
                fCurrentValue += fDeltaFraction;
                if (fCurrentValue > fEndValue) {
                    fCurrentValue = fEndValue;
                    fIsDone       = true;
                }
                return fCurrentValue;
            }
            return fEndValue;
        }

        void process(float*         signal_buffer,
                     const uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                signal_buffer[i] = process();
            }
        }

        void start() {
            const float mDelta = fEndValue - fStartValue;
            fDeltaFraction     = compute_delta_fraction(mDelta, fDuration);
            fCurrentValue      = fStartValue;
            fIsDone            = false;
        }

    private:
        uint32_t fSampleRate;
        float    fStartValue;
        float    fEndValue;
        float    fCurrentValue;
        float    fDuration;
        float    fDeltaFraction;
        bool     fIsDone;

        float compute_delta_fraction(float pDelta, float pDuration) {
            return pDuration > 0 ? (pDelta / fSampleRate) / pDuration : pDelta;
        }
    };
}  // namespace klangwellen