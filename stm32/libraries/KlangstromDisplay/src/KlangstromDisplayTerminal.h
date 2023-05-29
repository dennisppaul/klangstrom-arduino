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

#ifndef _KLANGSTROM_DISPLAY_TERMINAL_H_
#define _KLANGSTROM_DISPLAY_TERMINAL_H_

#include "KlangstromDisplay.h"
#include "KlangstromDisplayFont.h"

namespace klangstrom {
    class KlangstromDisplayTerminal {
    private:
        KlangstromDisplayFont* mFont;
        KlangstromDisplay*     mDisplay;

    public:
        uint16_t x;
        uint16_t y;
        uint8_t  width;
        uint8_t  height;
        uint8_t  cursor_x;
        uint8_t  cursor_y;
        uint8_t  char_width;
        uint8_t  char_height;
        bool     auto_clear;
        Color    foreground_color;
        Color    background_color;

        KlangstromDisplayTerminal(KlangstromDisplay*     pDisplay,
                                  KlangstromDisplayFont* pFont) : mFont(pFont),
                                                                  mDisplay(pDisplay),
                                                                  x(0),
                                                                  y(0),
                                                                  cursor_x(0),
                                                                  cursor_y(0),
                                                                  auto_clear(true) {
            foreground_color.set(255, 255, 255);
            background_color.set(0, 0, 0);
            if (mFont != nullptr && mDisplay != nullptr) {
                char_width  = mFont->width;
                char_height = mFont->height;
                width       = mDisplay->width() / char_width;    // default: 240 / 7  = 34
                height      = mDisplay->height() / char_height;  // default: 320 / 10 = 32
            }
        }

        void println() {
            print(LINE_RETURN);
            print(NEW_LINE);
        }

        void println(const char* s) {
            for (size_t i = 0; i < strlen(s); i++) {
                print(s[i]);
            }
            println();
        }

        void print(const char* s) {
            for (size_t i = 0; i < strlen(s); i++) {
                print(s[i]);
            }
        }

        void print(const char c) {
            if (c == NEW_LINE) {
                cursor_y++;
                wrap_y();
            } else if (c == LINE_RETURN) {
                cursor_x = 0;
            } else {
                if (c == BACKSPACE) {
                    if (cursor_x > 0) {
                        cursor_x--;
                    }
                }
                uint16_t x, y;
                calc_relative_position(x, y);
                mDisplay->textFont(mFont);
                mDisplay->background(background_color.r, background_color.g, background_color.b);
                mDisplay->color(foreground_color.r, foreground_color.g, foreground_color.b);
                if (c == BACKSPACE) {
                    // @TODO is there a more elegant way?
                    mDisplay->character(x, y, ' ');
                } else {
                    mDisplay->character(x, y, c);
                    cursor_x++;
                    if (cursor_x >= width) {
                        cursor_x = 0;
                        cursor_y++;
                        wrap_y();
                    }
                }
            }
        }

        void print_to(uint8_t pCursorX, uint8_t pCursorY, char c) {
            cursor_x = pCursorX;
            cursor_y = pCursorY;
            print(c);
        }

        void clear() {
            mDisplay->color(background_color.r, background_color.g, background_color.b);
            mDisplay->rect(x, y, char_width * width, char_height * height);
            cursor_x = 0;
            cursor_y = 0;
        }

    private:
        const char NEW_LINE    = '\n';
        const char LINE_RETURN = '\r';
        const char BACKSPACE   = '\b';

        void calc_relative_position(uint16_t& pX, uint16_t& pY) {
            pX = x + cursor_x * char_width;
            pY = y + cursor_y * char_height;
        }

        void wrap_y() {
            if (cursor_y >= height) {
                cursor_y = 0;
                if (auto_clear) {
                    clear();
                }
            }
        }
    };
}  // namespace klangstrom

#endif  // _KLANGSTROM_DISPLAY_TERMINAL_H_
