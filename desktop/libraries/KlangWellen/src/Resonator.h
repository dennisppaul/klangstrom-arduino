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
 * - [ ] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 */

#pragma once

#include <iostream>
#include <cmath>

#include "KlangWellen.h"
#include "Signal.h"

namespace klangwellen {
    class Resonator {
    public:
        Resonator()
            : m_frequency(440.0f), m_sampleRate(KlangWellen::DEFAULT_SAMPLING_RATE), m_qFactor(1.0f) {
            calculateCoefficients();
        }

        Resonator(float frequency, float sampleRate, float qFactor)
            : m_frequency(frequency), m_sampleRate(sampleRate), m_qFactor(qFactor) {
            calculateCoefficients();
        }

        float process(float input) {
            // IIR filter processing
            float output = a0 * input + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;

            // Update filter states
            x2 = x1;
            x1 = input;
            y2 = y1;
            y1 = output;

            return output;
        }

        void set_frequency(float frequency) {
            m_frequency = frequency;
            calculateCoefficients();
        }

        const float get_frequency() {
            return m_frequency;
        }

        void set_Q(float qFactor) {
            m_qFactor = qFactor;
            calculateCoefficients();
        }

        float get_Q() {
            return m_qFactor;
        }

    private:
        float m_frequency;  // Resonant frequency
        float m_sampleRate; // Sample rate of the audio
        float m_qFactor;    // Quality factor

        // Filter coefficients
        float a0, a1, a2, b1, b2;

        // Filter states
        float x1 = 0, x2 = 0, y1 = 0, y2 = 0;

        void calculateCoefficients() {
            // Calculate the coefficients for the resonator filter
            float omega = 2 * M_PI * m_frequency / m_sampleRate;
            float alpha = sin(omega) / (2 * m_qFactor);

            a0 = 1 + alpha;
            a1 = -2 * cos(omega);
            a2 = 1 - alpha;
            b1 = -2 * cos(omega);
            b2 = 1 - alpha;

            // Normalize the coefficients
            a1 /= a0;
            a2 /= a0;
            b1 /= a0;
            b2 /= a0;
        }
    };
} // namespace klangwellen
