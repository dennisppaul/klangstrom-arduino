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

#ifndef klangstrom_arduino_proxy_encoder_h
#define klangstrom_arduino_proxy_encoder_h

#include "SDL_render.h"
#include "klangstrom_arduino_debug.h"

class KlangstromArduinoProxyEncoder {
public:
    SDL_Rect dimensions;
    int16_t  ticks          = 0;
    uint8_t  ID             = 0;
    bool     button_pressed = false;

    void draw(SDL_Renderer *gRenderer, bool pActive);
    void rotate(int16_t pDelta);
    void update(bool pFlagged);
    
private:
    static const uint8_t MOVED_COUNTER           = 15;
    int16_t              previous_ticks          = 0;
    int8_t               fMoved                  = 0;
    bool                 selected                = false;
    bool                 previous_button_pressed = false;
    uint8_t              fMovedCounter           = 0;
};

#endif /* klangstrom_arduino_proxy_encoder_h */
