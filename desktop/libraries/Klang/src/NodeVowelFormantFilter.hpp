/*
 * Klang – a node+text-based synthesizer library
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
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

#ifndef _NODEVOWELFORMANT_FILTER_HPP_
#define _NODEVOWELFORMANT_FILTER_HPP_

#include <stdio.h>
#include "KlangMath.hpp"
#include "NodeKernelBlock.hpp"

namespace klang {
    class NodeVowelFormantFilter : public NodeKernelBlock {
        /*
         * Public source code by alex@smartelectronix.com
         * Simple example of implementation of formant filter
         * Vowel can be 0,1,2,3,4 <=> A,E,I,O,U
         * Good for spectral rich input like saw or square
         */
    public:
        const static uint8_t VOWEL_A = 0;
        const static uint8_t VOWEL_E = 1;
        const static uint8_t VOWEL_I = 2;
        const static uint8_t VOWEL_O = 3;
        const static uint8_t VOWEL_U = 4;
        const static uint8_t NUM_OF_VOWELS = 5;

        NodeVowelFormantFilter() {
            set_vowel(VOWEL_A);
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE *pPayLoad) {}

        float process(float pSignal) {
            double mSignal = (mCoeff[0] * pSignal +
                              mCoeff[1] * memory[0] +
                              mCoeff[2] * memory[1] +
                              mCoeff[3] * memory[2] +
                              mCoeff[4] * memory[3] +
                              mCoeff[5] * memory[4] +
                              mCoeff[6] * memory[5] +
                              mCoeff[7] * memory[6] +
                              mCoeff[8] * memory[7] +
                              mCoeff[9] * memory[8] +
                              mCoeff[10] * memory[9]);

            memory[9] = memory[8];
            memory[8] = memory[7];
            memory[7] = memory[6];
            memory[6] = memory[5];
            memory[5] = memory[4];
            memory[4] = memory[3];
            memory[3] = memory[2];
            memory[2] = memory[1];
            memory[1] = memory[0];
            memory[0] = mSignal;
            return (float) mSignal;
        }

        void lerp_vowel(int pVowelA, int pVowelB, float pLerp) {
            float b = KlangMath::clamp(pLerp, 0.0f, 1.0f);
            float a = 1.0f - b;
            for (uint8_t i = 0; i < NUM_OF_COEFF; i++) {
                mCoeff[i] = coeff[pVowelA][i] * a + coeff[pVowelB][i] * b;
            }
        }

        void set_vowel(int pVowel) {
            for (uint8_t i = 0; i < NUM_OF_COEFF; i++) {
                mCoeff[i] = coeff[pVowel][i];
            }
//            memcpy(mCoeff, coeff[pVowel], NUM_OF_COEFF * sizeof(float));
        }

    protected:
        void kernel(SIGNAL_TYPE *s) {
            for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                s[i] = process(s[i]);
            }
        }

    private:
        static const int NUM_OF_COEFF = 11;
        static constexpr double coeff[NUM_OF_VOWELS][NUM_OF_COEFF] = {
                {8.11044e-06,
                        8.943665402, -36.83889529, 92.01697887, -154.337906,  181.6233289,
                        -151.8651235, 89.09614114, -35.10298511, 8.388101016, -0.923313471  ///A
                },
                {4.36215e-06,
                        8.90438318,  -36.55179099, 91.05750846, -152.422234,  179.1170248,  ///E
                        -149.6496211, 87.78352223, -34.60687431, 8.282228154, -0.914150747
                },
                {3.33819e-06,
                        8.893102966, -36.49532826, 90.96543286, -152.4545478, 179.4835618,
                        -150.315433,  88.43409371, -34.98612086, 8.407803364, -0.932568035  ///I
                },
                {1.13572e-06,
                        8.994734087, -37.2084849,  93.22900521, -156.6929844, 184.596544,   ///O
                        -154.3755513, 90.49663749, -35.58964535, 8.478996281, -0.929252233
                },
                {4.09431e-07,
                        8.997322763, -37.20218544, 93.11385476, -156.2530937, 183.7080141,  ///U
                        -153.2631681, 89.59539726, -35.12454591, 8.338655623, -0.910251753
                }
        };

        double memory[NUM_OF_COEFF - 1];
        double mCoeff[NUM_OF_COEFF];
    };
} // namespace klang
#endif //_NODEVOWELFORMANT_FILTER_HPP_
