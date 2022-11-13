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

#ifndef _KLANGSTROMDISPLAY_SDL_BSP_H_
#define _KLANGSTROMDISPLAY_SDL_BSP_H_

#include <SDL.h>
#include <stdio.h>

#include <vector>

#include "KlangstromDisplay.h"
#include "KlangstromCallback.h"

namespace klangstrom {
    class KlangstromDisplay_SDL_BSP : public KlangstromDisplay, public KlangstromCallback {
    public:
        KlangstromDisplay_SDL_BSP() {
            mPosition.x = 0;
            mPosition.y = 0;
        }
        void BSP_init();
        void BSP_set_pixel(const uint16_t x, const uint16_t y, const uint16_t color);
        void BSP_clear_background();
        void BSP_line_vertical(uint16_t x0, uint16_t y0, uint16_t h, const uint16_t color);
        void BSP_line_horizontal(uint16_t x0, uint16_t y0, uint16_t w, const uint16_t color);
        void BSP_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const uint16_t color);
        void BSP_rect_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t color);
        void BSP_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t color);
        void BSP_character(uint16_t x, uint16_t y, char ch, const uint16_t foreground_color, const uint16_t background_color);
        void BSP_block(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);
        
        /* --- plattform specific --- */

		void callback();

        void ILI9341_Select() {}
        void ILI9341_Unselect() {}
        void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
        void ILI9341_WriteData(uint8_t *buff, size_t buff_size);

        void draw();
        void set_position(uint16_t pX, uint16_t pY) {
            mPosition.x = pX;
            mPosition.y = pY;
        }

    private:
        SDL_Texture          *texture;
        SDL_Renderer         *renderer;
        std::vector<uint8_t> *pixels = new std::vector<uint8_t>(mScreenWidth * mScreenHeight * 4, 0);

        struct {
            uint16_t x;
            uint16_t y;
        } mPosition;

        struct {
            uint16_t x;
            uint16_t y;
            uint16_t w;
            uint16_t h;
            uint32_t counter;
        } mAddressWindow;
    };
}  // namespace klangstrom

#endif  // _KLANGSTROMDISPLAY_SDL_BSP_H_
