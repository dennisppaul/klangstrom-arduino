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

#ifdef __cplusplus

#include "KlangstromDefines.hpp"

#if (KLST_ARCH == KLST_ARCH_DESKTOP)

#include "KlangstromDisplay_SDL-BSP.h"
#include "klangstrom_arduino_sdl.h"

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

void klangstrom::KlangstromDisplay_SDL_BSP::callback() {
    draw();
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_init() {
    renderer = getSDLRenderer();
    texture  = SDL_CreateTexture(renderer,
                                 //  SDL_PIXELFORMAT_RGBA8888,
                                 SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STREAMING,
                                 mScreenWidth, mScreenHeight);
    registerKlangstromDisplayCallback(this);
    // 	set_position(getSDLRendererWidth() / 2 - width() / 2, getSDLRendererHeight() / 2 -height() / 2);
    set_position(getKlangstromDisplayPosX(), getKlangstromDisplayPosY());
}

void klangstrom::KlangstromDisplay_SDL_BSP::draw() {
    /* lock texture */

    uint8_t *lockedPixels = nullptr;
    int      pitch        = 0;
    SDL_LockTexture(
        texture,
        NULL,
        reinterpret_cast<void **>(&lockedPixels),
        &pitch);
    std::memcpy(lockedPixels, pixels->data(), pixels->size());
    SDL_UnlockTexture(texture);

    /* update texture */

    SDL_UpdateTexture(
        texture,
        NULL,
        pixels->data(),
        mScreenWidth * 4);

    /* draw texture */

    SDL_Rect mSource;
    mSource.x = 0;
    mSource.y = 0;
    mSource.w = mScreenWidth;
    mSource.h = mScreenHeight;
    SDL_Rect mDestination;
    mDestination.x = mPosition.x;
    mDestination.y = mPosition.y;
    mDestination.w = mScreenWidth;
    mDestination.h = mScreenHeight;
    SDL_RenderCopy(renderer, texture, &mSource, &mDestination);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(renderer, &mDestination);
}

/* --- BSP --- */

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_set_pixel(const uint16_t x, const uint16_t y, const uint16_t color) {
    if (x > mScreenWidth || y > mScreenHeight) {
        return;
    }
    uint8_t r, g, b;
    convert_RGB_565i_888i(color, &r, &g, &b);
    const uint32_t offset = (mScreenWidth * 4 * y) + x * 4;
    (*pixels)[offset + 0] = b;
    (*pixels)[offset + 1] = g;
    (*pixels)[offset + 2] = r;
    (*pixels)[offset + 3] = 255;
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_clear_background() {
    /* @note(slightly faster implementation) */
    for (uint32_t i = 0; i < pixels->size(); i += 4) {
        (*pixels)[i + 0] = mColorBackground.b;
        (*pixels)[i + 1] = mColorBackground.g;
        (*pixels)[i + 2] = mColorBackground.r;
        (*pixels)[i + 3] = mColorBackground.a;
    }
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_line_vertical(uint16_t x0, uint16_t y0, uint16_t h, const uint16_t color) {
    // @todo(optimize!)
    BSP_line(x0, y0, x0, y0 + h, color);
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_line_horizontal(uint16_t x0, uint16_t y0, uint16_t w, const uint16_t color) {
    // @todo(optimize!)
    BSP_line(x0, y0, x0 + w, y0, color);
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const uint16_t color) {
    /**
     * @brief "Write a line.  Bresenham's algorithm - thx wikpedia" thx adafruit
     *
     */
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_uint16_t(x0, y0);
        _swap_uint16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_uint16_t(x0, x1);
        _swap_uint16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            BSP_set_pixel(y0, x0, color);
        } else {
            BSP_set_pixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_rect_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t color) {
    for (uint16_t mX = 0; mX < w; mX++) {
        for (uint16_t mY = 0; mY < h; mY++) {
            BSP_set_pixel(mX + x, mY + y, color);
        }
    }
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t color) {
    BSP_line_horizontal(x, y, w - 1, color);
    BSP_line_horizontal(x, y + h - 1, w - 1, color);
    BSP_line_vertical(x, y, h - 1, color);
    BSP_line_vertical(x + w - 1, y, h - 1, color);
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_character(uint16_t x, uint16_t y, char ch, const uint16_t foreground_color, const uint16_t background_color) {
    if (ch < 32 || ch > 126) {
        return;
    }
    if (font != nullptr) {
        uint32_t i, b, j;
        // ILI9341_SetAddressWindow(x, y, x+font->width-1, y+font->height-1);
        for (i = 0; i < font->height; i++) {
            b = font->data[(ch - 32) * font->height + i];
            for (j = 0; j < font->width; j++) {
                if ((b << j) & 0x8000) {
                    BSP_set_pixel(x + j, y + i, foreground_color);
                    // uint8_t data[] = {
                    //     (uint8_t)(color >> 8),
                    //     (uint8_t)(color & 0xFF)};
                    // ILI9341_WriteData(data, sizeof(data));
                } else {
                    BSP_set_pixel(x + j, y + i, background_color);
                    // uint8_t data[] = {
                    //     (uint8_t)(bgcolor >> 8),
                    //     (uint8_t)(bgcolor & 0xFF)};
                    // ILI9341_WriteData(data, sizeof(data));
                }
            }
        }
    }
}

void klangstrom::KlangstromDisplay_SDL_BSP::BSP_block(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
    ILI9341_Select();
    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ILI9341_WriteData((uint8_t *)data, sizeof(uint16_t) * w * h);
    ILI9341_Unselect();
}

void klangstrom::KlangstromDisplay_SDL_BSP::ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    mAddressWindow.x       = min(x0, x1);
    mAddressWindow.y       = min(y0, y1);
    mAddressWindow.w       = abs(x1 - x0);
    mAddressWindow.h       = abs(y1 - y0);
    mAddressWindow.counter = 0;
}

void klangstrom::KlangstromDisplay_SDL_BSP::ILI9341_WriteData(uint8_t *buff, size_t buff_size) {
    for (uint32_t i = 0; i < buff_size; i += 2) {
        static const bool IS_BIG_ENDIAN = true;
        const uint8_t     h             = buff[i + (IS_BIG_ENDIAN ? 0 : 1)];
        const uint8_t     l             = buff[i + (IS_BIG_ENDIAN ? 1 : 0)];
        const uint16_t    c             = pack_88i_16i(h, l);
        if (mAddressWindow.counter <= mAddressWindow.w * mAddressWindow.h) {
            const uint16_t x = mAddressWindow.counter % (mAddressWindow.h + 1);  // +1?
            const uint16_t y = mAddressWindow.counter / (mAddressWindow.w + 1);  // +1?
            BSP_set_pixel(x + mAddressWindow.x, y + mAddressWindow.y, c);
            mAddressWindow.counter++;
        }
    }
}

#endif  // (KLST_ARCH==KLST_ARCH_DESKTOP)
#endif  // __cplusplus
