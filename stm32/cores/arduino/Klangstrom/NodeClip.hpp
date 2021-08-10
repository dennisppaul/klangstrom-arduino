//
//  NodeClip.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_CLIP           ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeClip_hpp
#define NodeClip_hpp

#include "NodeKernel.hpp"

namespace klang {
    class NodeClip : public NodeKernel {
    public:
        void  set_min(float pClipMin) { mClipMin = pClipMin; }
        void  set_max(float pClipMax) { mClipMax = pClipMax; }
        float get_min() { return mClipMin; }
        float get_max() { return mClipMax; }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIN_F32:
                    set_min(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_MAX_F32:
                    set_max(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    protected:
        SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            return KlangMath::clamp(s, mClipMin, mClipMax);
        }

    private:
        float mClipMin = SIGNAL_MIN;
        float mClipMax = SIGNAL_MAX;
    };
}  // namespace klang

#endif /* NodeClip_hpp */
