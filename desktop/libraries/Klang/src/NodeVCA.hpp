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
 *       [ NODE_VCA            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| AMPLIFICATION       |
 *       +---------------------+
 */

#ifndef NodeVCA_hpp
#define NodeVCA_hpp

#include "NodeVCA.hpp"

namespace klang {
    class NodeVCA : public NodeKernelBlock2 {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL        = 0;
        static const CHANNEL_ID CH_IN_AMPLIFICATION = 1;

        void set_amplification(SIGNAL_TYPE pValue) {
            mAmplification = pValue;
        }

        SIGNAL_TYPE get_amplification() {
            return mAmplification;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_AMPLIFICATION_F32:
                    set_amplification(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    protected:
        void kernel(SIGNAL_TYPE* pOutputSignal,
                    SIGNAL_TYPE* pInputSignal_A,
                    SIGNAL_TYPE* pInputSignal_B) {
            if (pInputSignal_A == nullptr) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pOutputSignal[i] = 0.0;
                }
            } else if (pInputSignal_B == nullptr) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pOutputSignal[i] = pInputSignal_A[i] * mAmplification;
                }
            } else {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pOutputSignal[i] = pInputSignal_A[i] * pInputSignal_B[i];
                }
            }
        }

    private:
        SIGNAL_TYPE mAmplification = 1.0;
    };
}  // namespace klang

#endif /* NodeVCA_hpp */
