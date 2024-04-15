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
 * - [ ] void process(Signal&)
 * - [x] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 */

// @TODO Delay is broken, fix it

#pragma once

#include <stdint.h>
#include <stdio.h>

#include "Signal.h"

namespace klangwellen {

    /**
     * a delay line.
     */
    class Delay {
    public:
        /**
         * @param echo_length in seconds
         * @param sample_rate the sample rate in Hz.
         * @param decay_rate  the decay of the echo, a value between 0 and 1. 1 meaning no decay, 0 means immediate decay
         */
        Delay(float echo_length = 0.5, float decay_rate = 0.75, float wet = 0.8, uint32_t sample_rate = KlangWellen::DEFAULT_SAMPLING_RATE) : fSampleRate(sample_rate) {
            set_decay_rate(decay_rate);
            set_echo_length(echo_length);
            set_wet(wet);
            adaptEchoLength();
        }

        /**
         * @param echo_length new echo buffer length in seconds.
         */
        void set_echo_length(float echo_length) {
            fNewEchoLength = echo_length;
        }

        /**
         * A decay, should be a value between zero and one.
         *
         * @param decay_rate the new decay (preferably between zero and one).
         */
        void set_decay_rate(float decay_rate) {
            fDecayRate = decay_rate;
        }

        float get_decay_rate() {
            return fDecayRate;
        }

        void set_wet(float wet) {
            fWet = KlangWellen::clamp(wet, 0, 1);
        }

        float get_wet() {
            return fWet;
        }

        float process(float signal) {
            adaptEchoLength();

            if (fBufferPosition >= fBufferLength) {
                fBufferPosition = 0;
            }
            const float mDry         = 1.0 - fWet;
            const float mEcho        = fBuffer[fBufferPosition] * fDecayRate;
            signal                   = signal * mDry + mEcho * fWet;
            fBuffer[fBufferPosition] = signal;
            fBufferPosition++;

            return signal;
        }

        void process(float*         signal_buffer,
                     const uint32_t length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < length; i++) {
                signal_buffer[i] = process(signal_buffer[i]);
            }
        }

    private:
        int32_t  fBufferPosition  = 0;
        float    fDecayRate       = 0;
        float    fWet             = 0;
        float*   fBuffer          = nullptr;
        bool     fAllocatedBuffer = false;
        int32_t  fBufferLength    = 0;
        float    fNewEchoLength   = 0;
        uint32_t fSampleRate;

        void adaptEchoLength() {
            if (fNewEchoLength > 0) {
                const uint32_t mNewBufferLength = (uint32_t)(fSampleRate * fNewEchoLength);
                float*         mNewBuffer       = new float[mNewBufferLength]{0};
                if (fBuffer != nullptr) {
                    for (uint32_t i = 0; i < mNewBufferLength; i++) {
                        if (fBufferPosition >= fBufferLength) {
                            fBufferPosition = 0;
                        }
                        mNewBuffer[i] = fBuffer[fBufferPosition];
                        fBufferPosition++;
                    }
                    fBufferPosition = 0;
                }
                if (fAllocatedBuffer && fBuffer != nullptr) {
                    delete[] fBuffer;
                }
                fAllocatedBuffer = true;
                fBuffer          = mNewBuffer;
                fBufferLength    = mNewBufferLength;
            }
            fNewEchoLength = -1;
        }
    };
}  // namespace klangwellen