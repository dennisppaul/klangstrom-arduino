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

#ifndef _KLANGSTROM_DISPLAY_FONT_H_
#define _KLANGSTROM_DISPLAY_FONT_H_

#include <stdint.h>

typedef struct {
    // KlangstromDisplayFont(const uint8_t   pWidth,
    //                       uint8_t         pHeight,
    //                       const uint16_t *pData) : width(pWidth),
    //                                                height(pHeight),
    //                                                data(pData) {}
    const uint8_t   width;
    uint8_t         height;
    const uint16_t *data;
} KlangstromDisplayFont;

#endif  // _KLANGSTROM_DISPLAY_FONT_H_
