/*
 * Klangstrom
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

#ifndef _KLANGSTROM_DISPLAY_DRAW_BUFFER_H_
#define _KLANGSTROM_DISPLAY_DRAW_BUFFER_H_

#include <vector>

#include "KlangMath.hpp"
#include "KlangstromDisplay.h"

namespace klangstrom {
    class KlangstromDisplayDrawBuffer {
    public:
        enum VIEW {
            POINTS,
            LINES,
            LINES_SYMETRIC,
            LINE_STRIP,
        };

        KlangstromDisplayDrawBuffer(uint16_t pBufferSize = 32) : width(pBufferSize * 2),
                                                                 height(pBufferSize / 2),
                                                                 view(LINES),
                                                                 draw_baseline(true),
                                                                 mBuffer(pBufferSize, 0) {
        }

        void update_buffer(float* pBuffer, uint32_t pBufferLength, bool pAverageSamples = false) {
            if (pBufferLength >= mBuffer.size()) {
                const uint16_t mStride = pBufferLength / mBuffer.size();
                // // @TODO either `mBuffer.size()` should be a multiple of `pBufferLength` or
                // // `mStride` should be a float.
                // if (mStride != (float)pBufferLength / mBuffer.size()) {
                //     return;
                // }
                if (pAverageSamples) {
                    for (uint16_t i = 0; i < mBuffer.size(); i++) {
                        float s = 0.0;
                        for (uint16_t j = 0; j < mStride; j++) {
                            s += klang::KlangMath::clamp(pBuffer[j + i * mStride]);
                        }
                        s /= mStride;
                        mBuffer[i] = s;
                    }
                } else {
                    for (uint16_t i = 0; i < mBuffer.size(); i++) {
                        mBuffer[i] = klang::KlangMath::clamp(pBuffer[i * mStride]);
                    }
                }
            }
        }
        // @TODO(this could be made generic for different buffer types (see `NodeSampler`))
        void update_buffer(const int16_t* pBuffer, uint32_t pBufferLength, bool pAverageSamples = false) {
            if (pBufferLength >= mBuffer.size()) {
                const uint16_t mStride = pBufferLength / mBuffer.size();
                // // @TODO either `mBuffer.size()` should be a multiple of `pBufferLength` or
                // // `mStride` should be a float.
                // if (mStride != (float)pBufferLength / mBuffer.size()) {
                //     return;
                // }
                if (pAverageSamples) {
                    for (uint16_t i = 0; i < mBuffer.size(); i++) {
                        float s = 0.0;
                        for (uint16_t j = 0; j < mStride; j++) {
                            s += klang::KlangMath::clamp((float)(pBuffer[j + i * mStride]) / 32768.0);
                        }
                        s /= mStride;
                        mBuffer[i] = s;
                    }
                } else {
                    for (uint16_t i = 0; i < mBuffer.size(); i++) {
                        mBuffer[i] = klang::KlangMath::clamp(pBuffer[i * mStride] / 32768.0);
                    }
                }
            }
        }

        void draw_buffer(KlangstromDisplay& g, bool pClearBackground = true) {
            const uint16_t mPadding    = width / mBuffer.size();
            const uint16_t mHalfHeight = height / 2;
            if (view == LINE_STRIP) {
                g.BSP_rect_fill(x + 1, y + 1, width - 2, height - 1, g.get_background_16i());
            }
            for (uint16_t i = 1; i < mBuffer.size(); i++) {
                const uint16_t x0 = x + i * mPadding;
                const uint16_t y0 = y + mHalfHeight;
                const float    w  = mBuffer[i] * mHalfHeight;
                if (view != LINE_STRIP && pClearBackground) {
                    g.BSP_line(x0, y + 1, x0, y + height - 1, g.get_background_16i());
                }
                if (view != POINTS && draw_baseline) {
                    g.pixel(x0, y0);
                }
                switch (view) {
                    case LINES_SYMETRIC:
                        g.line(x0, y0, x0, y0 - w);
                        g.line(x0, y0, x0, y0 + w);
                        break;
                    case LINES:
                        g.line(x0, y0, x0, y0 + w);
                        break;
                    case POINTS:
                        g.pixel(x0, y0 + w);
                        break;
                    case LINE_STRIP: {
                        const uint16_t x1 = x + (i - 1) * mPadding;
                        const float    w1 = mBuffer[i - 1] * mHalfHeight;
                        g.line(x0, y0 + w, x1, y0 + w1);
                    } break;
                }
            }
        }

        void draw_buffer(KlangstromDisplay* g, bool pClearBackground = true) {
            draw_frame(*g);
        }

        void draw_frame(KlangstromDisplay& g) {
            g.rect(x, y, width, height + 1, false);
            g.line(x, y, x, y + height);
        }

        void draw_frame(KlangstromDisplay* g) {
            draw_frame(*g);
        }

        uint16_t buffer_size() {
            return mBuffer.size();
        }

        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        VIEW     view;
        bool     draw_baseline;

    private:
        std::vector<float> mBuffer;
    };
}  // namespace klangstrom

#endif  // _KLANGSTROM_DISPLAY_DRAW_BUFFER_H_
