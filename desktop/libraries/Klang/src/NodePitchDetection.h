/*
 * Klang – a node+text-based synthesizer library
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General License for more details.
 *
 * You should have received a copy of the GNU General License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _KLANG_NODE_PITCHDETECTION_H_
#define _KLANG_NODE_PITCHDETECTION_H_

#include "KlangNode.hpp"

namespace klang {

    // @TODO(implement node and atom fully)
    // @TODO(add mechanism to partially update buffer in order to collect more samples and thus get better frequency range i.e minimum buffer size of 1024)

    /**
     * detectes the pitch of a signal.
     * <p>
     * it uses the YIN algorithm described in the paper <a
     * href="http://recherche.ircam.fr/equipes/pcm/cheveign/ps/2002_JASA_YIN_proof.pdf">Alain de Cheveigné + Hideki
     * Kawahara:YIN, a fundamental frequency estimator for speech and music</a>.
     * <p>
     * Fair Use Disclaimer: this implementation was taken, with minor modifications, from a project called <a
     * href="https://github.com/JorenSix/TarsosDSP">TarsosDSP</a> by Joren Six released under the GPL-3.0 license. the
     * project seems to be really well written. i therefore think it is fair to use the existing source code and modify it.
     */
    template<uint16_t BUFFER_SIZE> // The size of a buffer. E.g. 1024.
    class NodePitchDetection
//            : Node
    {
    public:
        NodePitchDetection() : fSampleRate(KLANG_AUDIO_RATE), fThreshold(0.2) {}

        /**
         * Create a new pitch detector for a stream with the defined sample rate. Processes the audio in blocks of the
         * defined size.
         *
         * @param audioSampleRate The sample rate of the audio stream. E.g. 44.1 kHz.
         * @param yinThreshold    The parameter that defines which peaks are kept as possible pitch candidates. See the YIN
         *                        paper for more details.
         */
        NodePitchDetection(const float audioSampleRate, const double yinThreshold) : fSampleRate(audioSampleRate),
                                                                                     fThreshold(yinThreshold) {
        }

        /**
         * The main flow of the YIN algorithm. Returns a pitch value in Hz or -1 if no pitch is detected.
         *
         * @return a pitch value in Hz or -1 if no pitch is detected.
         */
        const float *process(const float *audioBuffer) {
            // step 2
            difference(audioBuffer);

            // step 3
            cumulativeMeanNormalizedDifference();

            // step 4
            const int tauEstimate = absoluteThreshold();

            // step 5
            float pitchInHertz;
            if (tauEstimate != -1) {
                const float betterTau = parabolicInterpolation(tauEstimate);

                // step 6
                // TODO Implement optimization for the AUBIO_YIN algorithm.
                // 0.77% => 0.5% error rate,
                // using the data of the YIN paper
                // bestLocalEstimate()

                // conversion to Hz
                pitchInHertz = fSampleRate / betterTau;
            } else {
                // no pitch found
                pitchInHertz = -1;
            }
            fPitch = pitchInHertz;
            return audioBuffer;
        }

        [[nodiscard]] float get_pitch() const {
            return fPitch;
        }

        [[nodiscard]] float get_probability() const {
            return fProbability;
        }

        [[nodiscard]] bool is_pitched() const {
            return fPitched;
        }

    private:
        /*
         * An implementation of the AUBIO_YIN pitch tracking algorithm. See <a href=
         * "http://recherche.ircam.fr/equipes/pcm/cheveign/ps/2002_JASA_YIN_proof.pdf" >the YIN paper.</a> Implementation
         *  based
         * on <a href="http://aubio.org">aubio</a>
         *
         * @author Joren Six
         * @author Paul Brossier
         */

        /**
         * The default YIN threshold value. Should be around 0.10~0.15. See YIN paper for more information.
         */
        constexpr static double DEFAULT_THRESHOLD = 0.20;

        /**
         * The actual YIN threshold.
         */
        const double fThreshold;

        /**
         * The audio sample rate. Most audio has a sample rate of 44.1kHz.
         */
        const float fSampleRate;

        /**
         * The buffer that stores the calculated values. It is exactly half the size of the input buffer.
         */
        const static uint16_t fYINBuffer_length = BUFFER_SIZE / 2;
        float fYINBuffer[fYINBuffer_length]{};
        float fPitch{};
        float fProbability{};
        bool fPitched{};

        /**
         * Implements the difference function as described in step 2 of the YIN paper.
         */
        void difference(const float *audioBuffer) {
            int index, tau;
            float delta;
            for (tau = 0; tau < fYINBuffer_length; tau++) {
                fYINBuffer[tau] = 0;
            }
            for (tau = 1; tau < fYINBuffer_length; tau++) {
                for (index = 0; index < fYINBuffer_length; index++) {
                    delta = audioBuffer[index] - audioBuffer[index + tau];
                    fYINBuffer[tau] += delta * delta;
                }
            }
        }

        /**
         * The cumulative mean normalized difference function as described in step 3 of the YIN paper. <br>
         * <code>
         * yinBuffer[0] == yinBuffer[1] = 1
         * </code>
         */
        void cumulativeMeanNormalizedDifference() {
            int tau;
            fYINBuffer[0] = 1;
            float runningSum = 0;
            for (tau = 1; tau < fYINBuffer_length; tau++) {
                runningSum += fYINBuffer[tau];
                fYINBuffer[tau] *= (float) tau / runningSum;
            }
        }

        /**
         * Implements step 4 of the AUBIO_YIN paper.
         */
        int absoluteThreshold() {
            // Uses another loop construct
            // than the AUBIO implementation
            int tau;
            // first two positions in yinBuffer are always 1
            // So start at the third (index 2)
            for (tau = 2; tau < fYINBuffer_length; tau++) {
                if (fYINBuffer[tau] < fThreshold) {
                    while (tau + 1 < fYINBuffer_length && fYINBuffer[tau + 1] < fYINBuffer[tau]) {
                        tau++;
                    }
                    // found tau, exit loop and return
                    // store the probability
                    // From the YIN paper: The threshold determines the list of
                    // candidates admitted to the set, and can be interpreted as the
                    // proportion of aperiodic power tolerated
                    // within a periodic signal.
                    //
                    // Since we want the periodicity and and not aperiodicity:
                    // periodicity = 1 - aperiodicity
                    fProbability = 1 - fYINBuffer[tau];
                    break;
                }
            }

            // if no pitch found, tau => -1
            if (tau == fYINBuffer_length || fYINBuffer[tau] >= fThreshold) {
                tau = -1;
                fProbability = 0;
                fPitched = false;
            } else {
                fPitched = true;
            }

            return tau;
        }

        /**
         * Implements step 5 of the AUBIO_YIN paper. It refines the estimated tau value using parabolic interpolation. This
         * is needed to detect higher frequencies more precisely. See
         * <a href="http://fizyka.umk.pl/nrbook/c10-2.pdf">(dead link)</a> and for more
         * background <a
         * href="http://sfb649.wiwi.hu-berlin.de/fedc_homepage/xplore/tutorials/xegbohtmlnode62.html">Minimization of a
         * Function: One-dimensional Case </a>
         *
         * @param tauEstimate The estimated tau value.
         * @return A better, more precise tau value.
         */
        float parabolicInterpolation(const int tauEstimate) {
            float betterTau;
            int x0;
            int x2;

            if (tauEstimate < 1) {
                x0 = tauEstimate;
            } else {
                x0 = tauEstimate - 1;
            }
            if (tauEstimate + 1 < fYINBuffer_length) {
                x2 = tauEstimate + 1;
            } else {
                x2 = tauEstimate;
            }
            if (x0 == tauEstimate) {
                if (fYINBuffer[tauEstimate] <= fYINBuffer[x2]) {
                    betterTau = (float) tauEstimate;
                } else {
                    betterTau = (float) x2;
                }
            } else if (x2 == tauEstimate) {
                if (fYINBuffer[tauEstimate] <= fYINBuffer[x0]) {
                    betterTau = (float) tauEstimate;
                } else {
                    betterTau = (float) x0;
                }
            } else {
                float s0, s1, s2;
                s0 = fYINBuffer[x0];
                s1 = fYINBuffer[tauEstimate];
                s2 = fYINBuffer[x2];
                // fixed AUBIO implementation, thanks to Karl Helgason:
                // (2.0f * s1 - s2 - s0) was incorrectly multiplied with -1
                betterTau = (float) tauEstimate + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
            }
            return betterTau;
        }
    };
} // namespace klang


#endif //_KLANG_NODE_PITCHDETECTION_H_
