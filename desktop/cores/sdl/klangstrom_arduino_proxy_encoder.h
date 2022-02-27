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

#include "KlangstromDefines.hpp"
#include "SDL_render.h"
#include "klangstrom_arduino_debug.h"

using namespace klangstrom;

class KlangstromArduinoProxyEncoder {
public:
    SDL_Rect dimensions;
    int16_t  ticks          = 0;
    uint8_t  ID             = 0;
    bool     button_pressed = false;

    void draw(SDL_Renderer *gRenderer, bool pActive) {
        if (pActive) {
            if (selected) {
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x88, 0x00, 0xFF);
                SDL_RenderFillRect(gRenderer, &dimensions);
            }
            if (fMovedCounter > 0) {
                fMovedCounter--;
                SDL_Rect r = {
                    dimensions.x + ((fMoved > 0) ? dimensions.w * 3 / 4 : 0),
                    dimensions.y,
                    dimensions.w / 4,
                    dimensions.h};
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                SDL_RenderFillRect(gRenderer, &r);
            }
            if (button_pressed) {
                SDL_Rect r = {
                    dimensions.x + dimensions.w / 4,
                    dimensions.y,
                    dimensions.w / 2,
                    dimensions.h};
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                SDL_RenderFillRect(gRenderer, &r);
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        } else {
            SDL_SetRenderDrawColor(gRenderer, 0x7F, 0x7F, 0x7F, 0xFF);
        }
        SDL_RenderDrawRect(gRenderer, &dimensions);
    }

    void rotate(int16_t pDelta) {
        ticks += pDelta;
    }

    void update(bool pFlagged) {
        selected = pFlagged;
        if (fMovedCounter == 0) {
            fMoved = 0;
        }
        if (ticks != previous_ticks) {
            /* create event */
            fMoved        = ticks - previous_ticks;
            fMovedCounter = MOVED_COUNTER;
            EventEncoder e;
            e.index          = (float)(ID - ENCODER_00);
            e.ticks          = (float)ticks;
            e.previous_ticks = (float)previous_ticks;
            e.delta          = e.ticks - e.previous_ticks;
            event_receive(EVENT_ENCODER_ROTATED, (float *)(&e));
        }
        if (button_pressed != previous_button_pressed) {
            EventEncoder e;
            e.index          = (float)(ID - ENCODER_00);
            e.ticks          = (float)ticks;
            e.previous_ticks = (float)previous_ticks;
            e.delta          = e.ticks - e.previous_ticks;
            if (button_pressed) {
                event_receive(EVENT_ENCODER_BUTTON_PRESSED, (float *)(&e));
            } else {
                event_receive(EVENT_ENCODER_BUTTON_RELEASED, (float *)(&e));
            }
        }
        previous_button_pressed = button_pressed;
        previous_ticks          = ticks;
    }

private:
    static const uint8_t MOVED_COUNTER           = 15;
    int16_t              previous_ticks          = 0;
    int8_t               fMoved                  = 0;
    bool                 selected                = false;
    bool                 previous_button_pressed = false;
    uint8_t              fMovedCounter           = 0;
};

#endif /* klangstrom_arduino_proxy_encoder_h */
