//
//  LUTSine.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef LUTSine_h
#define LUTSine_h

#include "KlangMath.hpp"

namespace klang {

    class LUTSine {
    public:
        static const uint16_t        WAVETABLE_LENGTH = 2048;
        static const SIGNAL_TYPE     WAVETABLE[WAVETABLE_LENGTH + 1];
        constexpr static const float ALPHA = (WAVETABLE_LENGTH / TWO_PI);

        static SIGNAL_TYPE sin_fast(const float pRad) {
            const uint16_t mIndex = ((uint32_t)(WAVETABLE_LENGTH * pRad / TWO_PI)) % WAVETABLE_LENGTH;
            return WAVETABLE[mIndex];
        }

        static SIGNAL_TYPE cos_fast(const float pRad) {
            return sin_fast(pRad + HALF_PI);
        }
    };
}  // namespace klang

#endif /* LUTSine_h */
