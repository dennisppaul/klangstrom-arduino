/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#ifndef _KLANGSTROM_DISPLAY_DRAW_SAMPLER_H_
#define _KLANGSTROM_DISPLAY_DRAW_SAMPLER_H_

#include <vector>

#include "KlangMath.hpp"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayDrawBuffer.h"
#include "NodeSampler.hpp"

using namespace klang;

namespace klangstrom {
    class KlangtromDisplayDrawSampler : public KlangstromDisplayDrawBuffer {
    public:
        void draw_in_out(KlangstromDisplay& g, bool pClearBackground = true) {
            if (fSampler != nullptr) {
                if (pClearBackground) {
                    g.BSP_rect_fill(x + 1, y + 1, width - 2, height - 1, g.get_background_16i());
                }

                // @TODO use doted lines for in/out?
                const uint16_t xIn  = x + (width - 0.5) * fSampler->get_in();
                const uint16_t xOut = x + (width - 0.5) * fSampler->get_out();
                const uint16_t y0   = y + 1;
                const uint16_t h    = height - 2;

                g.BSP_line_vertical(xIn, y0, h, g.get_color_16i());
                g.BSP_line_vertical(xOut, y0, h, g.get_color_16i());
            }
        }

        void draw(KlangstromDisplay& g) {
            draw_frame(g);
            draw_buffer(g);
            draw_in_out(g);
        }

        void update_sample_data(bool pAverageSamples = false) {
            update_buffer(fSampler->get_buffer(), fSampler->get_buffer_size(), pAverageSamples);
        }

        void set_sampler(NodeSamplerI16* pSampler) {
            fSampler = pSampler;
        }

    private:
        NodeSamplerI16* fSampler = nullptr;
    };
}  // namespace klangstrom

#endif  // _KLANGSTROM_DISPLAY_DRAW_SAMPLER_H_
