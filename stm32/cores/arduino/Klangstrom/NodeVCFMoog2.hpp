//
//  NodeVCFMoog2.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_VCF_MOOG2      ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| CUTOFF              |
 * IN02--| RESONANCE           |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeVCFMoog2_hpp
#define NodeVCFMoog2_hpp

#include "NodeVCF.hpp"

namespace klang {
    class NodeVCFMoogLP : public NodeVCF {
        /*
         Notes :
         in[x] and out[x] are member variables, init to 0.0 the controls:
         
         fc = cutoff, nearly linear [0,1] -> [0, fs/2]
         res = resonance [0, 4] -> [no resonance, self-oscillation]
         
         Code :
         Tfloat MoogVCF::run(float input, float fc, float res)
         {
         float f = fc * 1.16;
         float fb = res * (1.0 - 0.15 * f * f);
         input -= out4 * fb;
         input *= 0.35013 * (f*f)*(f*f);
         out1 = input + 0.3 * in1 + (1 - f) * out1; // Pole 1
         in1  = input;
         out2 = out1 + 0.3 * in2 + (1 - f) * out2;  // Pole 2
         in2  = out1;
         out3 = out2 + 0.3 * in3 + (1 - f) * out3;  // Pole 3
         in3  = out2;
         out4 = out3 + 0.3 * in4 + (1 - f) * out4;  // Pole 4
         in4  = out3;
         return out4;
         }
         
         from http://www.musicdsp.org/archive.php?classid=3#26
         */
    protected:
        float out1 = 0;
        float out2 = 0;
        float out3 = 0;
        float out4 = 0;
        float in1  = 0;
        float in2  = 0;
        float in3  = 0;
        float in4  = 0;

        SIGNAL_TYPE process(SIGNAL_TYPE input) {
            // fc  = cutoff, nearly linear [0,1] -> [0, fs/2]
            // res = resonance [0, 1] -> [no resonance, self-oscillation]
            const float f  = mCutoff * 1.16f;
            const float fb = (mResonance * 4) * (1.0f - 0.15f * f * f);
            input -= out4 * fb;
            input *= 0.35013f * (f * f) * (f * f);
            out1 = input + 0.3f * in1 + (1.0f - f) * out1;  // Pole 1
            in1  = input;
            out2 = out1 + 0.3f * in2 + (1.0f - f) * out2;  // Pole 2
            in2  = out1;
            out3 = out2 + 0.3f * in3 + (1.0f - f) * out3;  // Pole 3
            in3  = out2;
            out4 = out3 + 0.3f * in4 + (1.0f - f) * out4;  // Pole 4
            in4  = out3;
            //    return out4;
            if (out4 != out4) {
                out1 = 0;
                out2 = 0;
                out3 = 0;
                out4 = 0;
                in1  = 0;
                in2  = 0;
                in3  = 0;
                in4  = 0;
                return 0;
            } else {
                return out4 > SIGNAL_MAX ? SIGNAL_MAX : (out4 < SIGNAL_MIN ? SIGNAL_MIN : out4);
            }
        }
    };
}  // namespace klang
#endif /* NodeVCFMoog2_hpp */
