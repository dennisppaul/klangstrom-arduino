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

#ifndef _KLANGSTROM_DISPLAY_H_
#define _KLANGSTROM_DISPLAY_H_

// @TODO(#featurerequest, "add option to draw transparent text ( aka no background color drawing in char )")

#include <math.h>
#include <stdio.h>

#include "KlangstromDisplayFont.h"
#include "WString.h"

namespace klangstrom {
    class Color {
    public:
        static const uint16_t BLACK   = 0x0000;
        static const uint16_t BLUE    = 0x001F;
        static const uint16_t RED     = 0xF800;
        static const uint16_t GREEN   = 0x07E0;
        static const uint16_t CYAN    = 0x07FF;
        static const uint16_t MAGENTA = 0xF81F;
        static const uint16_t YELLOW  = 0xFFE0;
        static const uint16_t WHITE   = 0xFFFF;

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
        void    set(uint8_t pR, uint8_t pG, uint8_t pB) {
            r = pR;
            g = pG;
            b = pB;
        }
    };

    class KlangstromDisplay {
    
	#ifndef _swap_uint16_t
	#define _swap_uint16_t(a, b) \
		{                        \
			uint16_t t = a;      \
			a          = b;      \
			b          = t;      \
		}
	#endif

    protected:
        Color                  mColorForeground;
        Color                  mColorBackground;
        KlangstromDisplayFont *font;

        const uint16_t mScreenWidth  = 240;
        const uint16_t mScreenHeight = 320;

    public:
        static KlangstromDisplay* create();
    
        KlangstromDisplay() {
            mColorForeground.r = 255;
            mColorForeground.g = 255;
            mColorForeground.b = 255;
            mColorForeground.a = 255;
            mColorBackground.r = 0;
            mColorBackground.g = 0;
            mColorBackground.b = 0;
            mColorBackground.a = 255;
            font               = nullptr;
        }

        /* utilities */

        uint16_t get_color_16i() { return convert_RGB_888i_565i(mColorForeground.r, mColorForeground.g, mColorForeground.b); }
        uint16_t get_background_16i() { return convert_RGB_888i_565i(mColorBackground.r, mColorBackground.g, mColorBackground.b); }

        uint16_t convert_RGB_888i_565i(uint8_t r, uint8_t g, uint8_t b) {
            uint16_t c;
            c = ((uint16_t)r * 31 / 255) << 11;
            c |= (((uint16_t)g * 63 / 255) << 5);
            c |= (((uint16_t)b * 31 / 255));
            return c;
        }

        void convert_RGB_565i_888i(uint16_t c, uint8_t *r, uint8_t *g, uint8_t *b) {
            *r = (uint8_t)(((c >> 11) & 0b0000000000011111) * 255 / 31);
            *g = (uint8_t)(((c >> 5) & 0b0000000000111111) * 255 / 63);
            *b = (uint8_t)(((c)&0b0000000000011111) * 255 / 31);
        }

        void unpack_16i_88i(uint16_t i, uint8_t *h, uint8_t *l) {
            *h = (uint8_t)((i & 0b1111111100000000) >> 8);  // @note(shift not necessary)
            *l = (uint8_t)(i & 0b0000000011111111);
        }

        void unpack_32i_8888i(uint32_t i, uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *a) {
            *r = (uint8_t)((i & 0xFF000000) >> 24);
            *g = (uint8_t)((i & 0x00FF0000) >> 16);
            *b = (uint8_t)((i & 0x0000FF00) >> 8);
            *a = (uint8_t)((i & 0x000000FF) >> 0);
        }

        uint32_t pack_888i_32i(uint8_t r, uint8_t g, uint8_t b) {
            // RGBA
            uint32_t i;
            i = 255;  // default to 0xFF ( aka "alpha 100%" )
            i |= r << 24;
            i |= g << 16;
            i |= b << 8;
            return i;
        }

        uint16_t pack_88i_16i(uint8_t h, uint8_t l) {
            uint16_t i;
            i = l;
            i |= (uint16_t)h << 8;
            return i;
        }

        /* --- platform specific implementation --- */

        virtual void BSP_init()                                                                                                       = 0;
        virtual void BSP_set_pixel(const uint16_t x, const uint16_t y, const uint16_t color)                                          = 0;
        virtual void BSP_clear_background()                                                                                           = 0;
        virtual void BSP_line_vertical(uint16_t x0, uint16_t y0, uint16_t h, const uint16_t color)                                    = 0;
        virtual void BSP_line_horizontal(uint16_t x0, uint16_t y0, uint16_t w, const uint16_t color)                                  = 0;
        virtual void BSP_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const uint16_t color)                               = 0;
        virtual void BSP_rect_fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t color)                              = 0;
        virtual void BSP_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t color)                                   = 0;
        virtual void BSP_character(uint16_t x, uint16_t y, char ch, const uint16_t foreground_color, const uint16_t background_color) = 0;
        virtual void BSP_block(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data)                                  = 0;

        /* --- shared draw methods --- */

        /*
         * NOTES:
         *
         * the drawing library remains very basic. it will only be able to draw basic shapes ( e.g. rectangles,
         * circles, lines, etc. ). the color mode will be RGB. however, there is no guarantee regarding the underlying
         * color model ( e.g 888 or 565 ).
         */

        inline uint16_t width() {
            return mScreenWidth;
        }

        inline uint16_t height() {
            return mScreenHeight;
        }

        void begin() {
            BSP_init();
        }

        void color(uint8_t r, uint8_t g, uint8_t b) {
            // @todo(maybe store color in 565 format)
            mColorForeground.set(r, g, b);
        }

        void color_16i(uint16_t p565Color) {
            uint8_t r, g, b;
            convert_RGB_565i_888i(p565Color, &r, &g, &b);
            color(r, g, b);
        }

        void color_32i(uint32_t rgb) {
            uint8_t r, g, b, a;  // @note(`a` is not used)
            unpack_32i_8888i(rgb << 8, &r, &g, &b, &a);
            color(r, g, b);
        }

        void background(uint8_t r, uint8_t g, uint8_t b) {
            // @todo(maybe store color in 565 format)
            mColorBackground.set(r, g, b);
        }

        void background_16i(uint16_t p565Color) {
            uint8_t r, g, b;
            convert_RGB_565i_888i(p565Color, &r, &g, &b);
            background(r, g, b);
        }

        void background_32i(uint32_t rgb) {
            uint8_t r, g, b, a;  // @note(`a` is not used)
            unpack_32i_8888i(rgb << 8, &r, &g, &b, &a);
            background(r, g, b);
        }

        void clear() {
            BSP_clear_background();
        }

        void pixel(const uint16_t x, const uint16_t y) {
            BSP_set_pixel(x, y, get_color_16i());
        }

        void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
            if (x0 == x1) {
                BSP_line_vertical(x0, fmin(y0, y1), abs(y1 - y0), get_color_16i());
            } else if (y0 == y1) {
                BSP_line_horizontal(fmin(x0, x1), y0, abs(x1 - x0), get_color_16i());
            } else {
                BSP_line(x0, y0, x1, y1, get_color_16i());
            }
        }

        void rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool fill = true) {
            if (fill) {
                BSP_rect_fill(x, y, w, h, get_color_16i());
            } else {
                BSP_rect(x, y, w, h, get_color_16i());
            }
        }

        void text(uint16_t x, uint16_t y, const char *str) {
            if (font) {
                const uint16_t mForegroundColor = get_color_16i();
                const uint16_t mBackgroundColor = get_background_16i();
                while (*str) {
                    if (x + font->width >= width()) {
                        x = 0;
                        y += font->height;
                        if (y + font->height >= height()) {
                            break;
                        }
                        if (*str == ' ') {
                            // skip spaces in the beginning of the new line
                            str++;
                            continue;
                        }
                    }
                    BSP_character(x, y, *str, mForegroundColor, mBackgroundColor);
                    x += font->width;
                    str++;
                }
            }
        }        
        
        void character(uint16_t x, uint16_t y, const char c) {
            if (font) {
                const uint16_t mForegroundColor = get_color_16i();
                const uint16_t mBackgroundColor = get_background_16i();
                if (x + font->width >= width()) {
                    x = 0;
                    y += font->height;
                    if (y + font->height >= height()) {
                        return;
                    }
                }
                BSP_character(x, y, c, mForegroundColor, mBackgroundColor);
                x += font->width;
            }
        }

        void image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
            if ((x >= width()) || (y >= height())) {
                return;
            }
            if ((x + w - 1) >= width()) {
                return;
            }
            if ((y + h - 1) >= height()) {
                return;
            }

            BSP_block(x, y, w, h, data);
        }

        void textFont(KlangstromDisplayFont *pFont) {
            font = pFont;
        }

        // void set_rotation(uint8_t m) {
        //     rotation = m % 4;
        //     switch (rotation) {
        //         case 0:
        //             m       = (MADCTL_MX | MADCTL_BGR);
        //             _width  = ILI9341_TFTWIDTH;
        //             _height = ILI9341_TFTHEIGHT;
        //             break;
        //         case 1:
        //             m       = (MADCTL_MV | MADCTL_BGR);
        //             _width  = ILI9341_TFTHEIGHT;
        //             _height = ILI9341_TFTWIDTH;
        //             break;
        //         case 2:
        //             m       = (MADCTL_MY | MADCTL_BGR);
        //             _width  = ILI9341_TFTWIDTH;
        //             _height = ILI9341_TFTHEIGHT;
        //             break;
        //         case 3:
        //             m       = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
        //             _width  = ILI9341_TFTHEIGHT;
        //             _height = ILI9341_TFTWIDTH;
        //             break;
        //     }
        //     sendCommand(ILI9341_MADCTL, &m, 1);
        // }

        // uint8_t get_rotation(void) const { return rotation; }

        // void fillTriangle(int16_t x0, int16_t y0,
        //                   int16_t x1, int16_t y1,
        //                   int16_t x2, int16_t y2,
        //                   uint16_t color) {
        //     int16_t a, b, y, last;

        //     // Sort coordinates by Y order (y2 >= y1 >= y0)
        //     if (y0 > y1) {
        //         _swap_int16_t(y0, y1);
        //         _swap_int16_t(x0, x1);
        //     }
        //     if (y1 > y2) {
        //         _swap_int16_t(y2, y1);
        //         _swap_int16_t(x2, x1);
        //     }
        //     if (y0 > y1) {
        //         _swap_int16_t(y0, y1);
        //         _swap_int16_t(x0, x1);
        //     }

        //     startWrite();
        //     if (y0 == y2) {  // Handle awkward all-on-same-line case as its own thing
        //         a = b = x0;
        //         if (x1 < a)
        //             a = x1;
        //         else if (x1 > b)
        //             b = x1;
        //         if (x2 < a)
        //             a = x2;
        //         else if (x2 > b)
        //             b = x2;
        //         writeFastHLine(a, y0, b - a + 1, color);
        //         endWrite();
        //         return;
        //     }

        //     int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
        //             dx12 = x2 - x1, dy12 = y2 - y1;
        //     int32_t sa = 0, sb = 0;

        //     // For upper part of triangle, find scanline crossings for segments
        //     // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
        //     // is included here (and second loop will be skipped, avoiding a /0
        //     // error there), otherwise scanline y1 is skipped here and handled
        //     // in the second loop...which also avoids a /0 error here if y0=y1
        //     // (flat-topped triangle).
        //     if (y1 == y2)
        //         last = y1;  // Include y1 scanline
        //     else
        //         last = y1 - 1;  // Skip it

        //     for (y = y0; y <= last; y++) {
        //         a = x0 + sa / dy01;
        //         b = x0 + sb / dy02;
        //         sa += dx01;
        //         sb += dx02;
        //         /* longhand:
        //         a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        //         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        //         */
        //         if (a > b)
        //             _swap_int16_t(a, b);
        //         writeFastHLine(a, y, b - a + 1, color);
        //     }

        //     // For lower part of triangle, find scanline crossings for segments
        //     // 0-2 and 1-2.  This loop is skipped if y1=y2.
        //     sa = (int32_t)dx12 * (y - y1);
        //     sb = (int32_t)dx02 * (y - y0);
        //     for (; y <= y2; y++) {
        //         a = x1 + sa / dy12;
        //         b = x0 + sb / dy02;
        //         sa += dx12;
        //         sb += dx02;
        //         /* longhand:
        //         a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        //         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        //         */
        //         if (a > b)
        //             _swap_int16_t(a, b);
        //         writeFastHLine(a, y, b - a + 1, color);
        //     }
        //     endWrite();
        // }
    };
}  // namespace klangstrom

// extern klangstrom::KlangstromDisplay *DisplayPtr;
// #define Display (*DisplayPtr)

#endif  // _KLANGSTROM_DISPLAY_H_
