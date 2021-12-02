//
//  NodeSoftClip.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_SOFT_CLIP      ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeSoftClip_hpp
#define NodeSoftClip_hpp

#include "KlangMath.hpp"
#include "NodeKernelBlock.hpp"

namespace klang {
    class NodeSoftClip : public NodeKernelBlock {
    public:
        void set_level_in(SIGNAL_TYPE pValue) {
            mLevel_IN = pValue;
        }

        SIGNAL_TYPE get_level_in() {
            return mLevel_IN;
        }

        void set_level_out(SIGNAL_TYPE pValue) {
            mLevel_OUT = pValue;
        }

        SIGNAL_TYPE get_level_out() {
            return mLevel_OUT;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            // @todo( add `KLANG_SET_LEVEL_IN_F32` + `KLANG_SET_LEVEL_OUT_F32` )
            // switch (pCommand) {
            //     case KLANG_SET_LEVEL_IN_F32:
            //         set_level_in(KlangMath::FLOAT_32(pPayLoad));
            //         break;
            //     case KLANG_SET_LEVEL_OUT_F32:
            //         set_level_out(KlangMath::FLOAT_32(pPayLoad));
            //         break;
            // }
        }

    protected:
        void kernel(SIGNAL_TYPE* s) {
            for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                s[i] = klang_math_tan(s[i] * mLevel_IN) * mLevel_OUT;
            }
        }

    private:
        SIGNAL_TYPE mLevel_IN  = 1.0;
        SIGNAL_TYPE mLevel_OUT = 1.0;
    };
}  // namespace klang

#endif /* NodeSoftClip_hpp */
