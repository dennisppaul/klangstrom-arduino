//
//  NodeMap.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_MAP            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          maps a value from an input range to an output range.
 *       )
 */

#ifndef NodeMap_hpp
#define NodeMap_hpp

#include "NodeMap.hpp"

namespace klang {
    class NodeMap : public NodeKernelBlock {
    public:
        void set_in_min(SIGNAL_TYPE pInMin) {
            mInMin = pInMin;
        }
        
        void set_in_max(SIGNAL_TYPE pInMax) {
            mInMax = pInMax;
        }
        
        void set_out_min(SIGNAL_TYPE pOutMin) {
            mOutMin = pOutMin;
        }
        
        void set_out_max(SIGNAL_TYPE pOutMax) {
            mOutMax = pOutMax;
        }
        
        void set_in_min(SIGNAL_TYPE pInMin, SIGNAL_TYPE pInMax, SIGNAL_TYPE pOutMin, SIGNAL_TYPE pOutMax) {
            mInMin = pInMin;
            mInMax = pInMax;
            mOutMin = pOutMin;
            mOutMax = pOutMax;
        }
        
        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_IN_MIN_F32:
                    set_in_min(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_IN_MAX_F32:
                    set_in_max(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_OUT_MIN_F32:
                    set_out_min(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_OUT_MAX_F32:
                    set_out_max(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }
        
    protected:
        void kernel(SIGNAL_TYPE* s) {
            const float r = (mInMax - mInMin) / (mOutMax - mOutMin);
            for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                const float a = s[i] - mInMin;
                const float b = r + mOutMin;
                s[i] = a / b;
            }
        }
        
    private:
        SIGNAL_TYPE mInMin  = 0.0;
        SIGNAL_TYPE mInMax  = 1.0;
        SIGNAL_TYPE mOutMin = 0.0;
        SIGNAL_TYPE mOutMax = 1.0;
    };
}

#endif /* NodeMap_hpp */
