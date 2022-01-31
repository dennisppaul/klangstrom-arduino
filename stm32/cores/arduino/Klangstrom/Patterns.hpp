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

#ifndef Patterns_hpp
#define Patterns_hpp

#include <stdbool.h>
#include <stdint.h>

namespace strom {

    bool is_bit_set(const uint32_t pNumber, const uint8_t pBit) {
        const uint16_t mMask = 1 << pBit;
        return (mMask & pNumber) >> pBit;
    }

    uint8_t mirror_byte(uint32_t pNumber) {
        return mirror_bits(pNumber, 8);
    }

    uint32_t mirror_bits(uint32_t pNumber, int8_t pNumberOfBits) {
        uint32_t mMirroredNumber = 0;
        for (int8_t i = 0; i < pNumberOfBits; i++) {
            const uint32_t mMask      = 1 << i;
            const uint8_t  mMirrorBit = (pNumberOfBits - i) - 1;
            const bool     mBit       = ((mMask & pNumber) >> i);
            mMirroredNumber += mBit ? 1 << mMirrorBit : 0;
        }
        return mMirroredNumber;
    }

    bool bit_bang(uint32_t pStep, uint32_t pPattern, uint32_t pLength, bool pMirror = true) {
        const uint32_t mStep    = pStep % pLength;
        const uint32_t mPattern = pMirror ? mirror_bits(pPattern, pLength) : pPattern;
        return is_bit_set(mPattern, mStep);
    }

    float pattern(const uint32_t pStep, const float* pPattern, const uint32_t pLength) {
        const uint32_t mStep = pStep % pLength;
        return pPattern[mStep];
    }

    bool bang(uint32_t pStep, uint32_t pLength) {
        return pStep % pLength == 0;
    }

    uint32_t not_bang(int32_t pStep, uint32_t pLength) {
        return pStep % pLength;
    }

}  // namespace strom

#endif  // Patterns_hpp