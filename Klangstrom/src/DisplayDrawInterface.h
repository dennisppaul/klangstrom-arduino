/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
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

#pragma once

#include <stdint.h>

#include "BitmapFont.h"

void     display_clear_BSP(uint32_t color);
void     display_set_pixel_BSP(uint16_t x, uint16_t y, uint32_t color);
void     display_set_pixel_alpha_BSP(uint16_t x, uint16_t y, uint32_t color);
uint32_t display_get_pixel_BSP(uint16_t x, uint16_t y);
void     display_rect_fill_BSP(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color); // BSP
void     display_line_horizontal_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color);
void     display_line_vertical_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color);
void     display_image_BSP(uint32_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void     display_char_BSP(BitmapFont* font, uint16_t x, uint16_t y, uint8_t ascii_char, uint32_t color, uint32_t background_color);
