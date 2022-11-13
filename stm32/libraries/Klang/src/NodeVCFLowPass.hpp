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
 *       [ NODE_VCF_LP         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| CUTOFF              |
 * IN02--| RESONANCE           |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeVCFLowPass_hpp
#define NodeVCFLowPass_hpp

#include "KlangMath.hpp"
#include "NodeVCF.hpp"

namespace klang {
    class NodeVCFLowPass : public NodeVCF {
        /*
         simple resonant filter posted to musicdsp.org by Paul Kellett http://www.musicdsp.org/archive.php?classid=3#259
         
         // set feedback amount given f and q between 0 and 1
         fb = q + q/(1.0 - f);
         
         // for each sample...
         buf0 = buf0 + f * (in - buf0 + fb * (buf0 - buf1));
         buf1 = buf1 + f * (buf0 - buf1);
         out = buf1;
         
         fixed point version of the filter
         "dave's blog of art and programming" http://www.pawfal.org/dave/blog/2011/09/
         */

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_CUTOFF_F32:
                    set_cutoff(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_RESONANCE_F32:
                    set_resonance(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    protected:
        float       buf0;
        float       buf1;
        SIGNAL_TYPE process(SIGNAL_TYPE input) {
            const float q = mResonance;
            const float f = mCutoff;
            // set feedback amount given f and q between 0 and 1
            const float a  = 1.0 - f;
            const float b  = q / a;
            const float fb = q + b;

            // for each sample...
            const float c = buf0 - buf1;
            const float d = fb * c;
            const float e = input - buf0 + d;
            const float g = f * e;
            buf0          = buf0 + g;
            const float h = buf0 - buf1;
            const float i = f * h;
            buf1          = buf1 + i;
            if (buf1 != buf1) {
                buf0 = 0;
                buf1 = 0;
            }
            return buf1;
        }
    };
}  // namespace klang
#endif /* NodeVCFLowPass_hpp */
