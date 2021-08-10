//
//  NodeDistortion.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_DISTORTION     ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| AMPLIFICATION       |
 * IN02--| CLIP                |
 *       |                     |
 *       +---------------------+
 */
// @TODO(implement `AMPLIFICATION` + `CLIP`)

#ifndef NodeDistortion_hpp
#define NodeDistortion_hpp

#include "NodeKernel.hpp"

namespace klang {
    class NodeDistortion : public NodeKernel {
    public:
        enum TYPE {
            CLIP,
            FOLDBACK,
            FOLDBACK_SINGLE
        };

        NodeDistortion() {
            set_clip(1.0);
            set_amplification(1.0);
            set_type(CLIP);
        }

        void set_clip(const float pClip) {
            mClip = pClip;
        }

        float get_clip() {
            return mClip;
        }

        void set_amplification(const float pAmplification) {
            mAmplification = pAmplification;
        }

        float get_amplification() {
            return mAmplification;
        }

        void set_type(const TYPE pDistortionType) {
            mDistortionType = pDistortionType;
        }

        TYPE get_type() {
            return mDistortionType;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_CLIP_F32:
                    set_clip(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_AMPLIFICATION_F32:
                    set_amplification(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_TYPE_I8:
                    set_type(static_cast<TYPE>(pPayLoad[0]));
                    break;
            }
        }

    protected:
        SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            switch (mDistortionType) {
                case CLIP:
                    return limit_clip(s * mAmplification);
                case FOLDBACK:
                    return limit_foldback(s * mAmplification);
                case FOLDBACK_SINGLE:
                    return limit_foldback_single(s * mAmplification);
                default:
                    return 0.0;
            }
        }

    private:
        float mClip;
        float mAmplification;
        TYPE  mDistortionType;

#define LOCK_GUARD 1
        float limit_foldback(float v) {
#if LOCK_GUARD
            const static uint8_t MAX_NUM_OF_ITERATIONS = 16;
            uint8_t              i                     = 0;
#endif
            while (v > mClip || v < -mClip) {
                v = limit_foldback_single(v);
#if LOCK_GUARD
                i++;
                if (i > MAX_NUM_OF_ITERATIONS) {
                    return 0.0;
                }
#endif
            }
            return v;
        }

        inline float limit_foldback_single(const float v) {
            if (v > mClip) {
                const float w = 2 * mClip - v;
                return w;
            } else if (v < -mClip) {
                const float w = -2 * mClip - v;
                return w;
            } else {
                return v;
            }
        }

        inline float limit_clip(const float v) {
            if (v > mClip) {
                return mClip;
            } else if (v < -mClip) {
                return -mClip;
            } else {
                return v;
            }
        }
    };
}  // namespace klang

#endif /* NodeDistortion_hpp */
