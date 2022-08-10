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

/**
 *       [ NODE_VCF_MOOG_LHBP  ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| CUTOFF              |
 * IN02--| RESONANCE           |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeVCFMoogLHBP_hpp
#define NodeVCFMoogLHBP_hpp

#include "NodeVCF.hpp"

namespace klang {
    class NodeVCFMoogLHBP : public NodeVCF {
    public:
        const static uint8_t HIGH_PASS = 0;
        const static uint8_t LOW_PASS  = 1;
        const static uint8_t BAND_PASS = 2;

        void set_filter_type(const uint8_t pFilterType) {
            mFilterType = pFilterType;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_CUTOFF_F32:
                    set_cutoff(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_RESONANCE_F32:
                    set_resonance(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_TYPE_I8:
                    set_filter_type(pPayLoad[0]);
                    break;
            }
        }

    protected:
        SIGNAL_TYPE process(SIGNAL_TYPE pInput) {
            /* from https://www.musicdsp.org/en/latest/Filters/25-moog-vcf-variation-1.html */
            // Set coefficients given frequency & resonance [0.0...1.0]
            float t1, t2;  // temporary buffers

            float q = 1.0f - mCutoff;
            float p = mCutoff + 0.8f * mCutoff * q;
            float f = p + p - 1.0f;
            q       = mResonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));

            // Filter (in [-1.0...+1.0])
            float in = _constrain(pInput, -1, 1);  // hard clip

            in -= q * b[4];  // feedback

            t1   = b[1];
            b[1] = (in + b[0]) * p - b[1] * f;

            t2   = b[2];
            b[2] = (b[1] + t1) * p - b[2] * f;

            t1   = b[3];
            b[3] = (b[2] + t2) * p - b[3] * f;

            b[4] = (b[3] + t1) * p - b[4] * f;
            b[4] = b[4] - b[4] * b[4] * b[4] * 0.166667f;  // clipping

            // inelegantly squash denormals
            if (b[4] != b[4]) {
                for (uint8_t i = 0; i < 5; i++) {
                    b[i] = 0;
                }
            }

            b[0] = in;

            switch (mFilterType) {
                case HIGH_PASS:
                    return in - b[4];
                    break;

                case LOW_PASS:
                    return b[4];
                    break;

                case BAND_PASS:
                    return 3.0f * (b[3] - b[4]);
                    break;
            }
            return 0;
        }

    private:
        uint8_t mFilterType = LOW_PASS;
        float   b[5];

        float _constrain(float value, float min, float max) {
            return value > max ? max : (value < min ? min : value);
        }
    };
}  // namespace klang

#endif /* NodeVCFMoogLHBP_hpp */
