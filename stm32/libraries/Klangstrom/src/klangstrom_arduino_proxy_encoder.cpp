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

#include "KlangstromDefines.hpp"

#if (KLST_ARCH == KLST_ARCH_DESKTOP)

#include "Klangstrom.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterface.h"
#include "KlangstromEvents.h"
#include "klangstrom_arduino_proxy_encoder.h"

using namespace klangstrom;

void KlangstromArduinoProxyEncoder::draw(SDL_Renderer* gRenderer, bool pActive) {
    if (pActive) {
        if (selected) {
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0x88, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &dimensions);
        }
        if (fMovedCounter > 0) {
            fMovedCounter--;
            SDL_Rect r = {
                dimensions.x,
                dimensions.y + ((fMoved > 0) ? 0 : dimensions.h * 3 / 4),
                dimensions.w,
                dimensions.h / 4};
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &r);
        }
        if (button_pressed) {
            SDL_Rect r = {
                dimensions.x,
                dimensions.y + dimensions.h / 4,
                dimensions.w,
                dimensions.h / 2};
            SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderFillRect(gRenderer, &r);
        }
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    } else {
        SDL_SetRenderDrawColor(gRenderer, 0x7F, 0x7F, 0x7F, 0xFF);
    }
    SDL_RenderDrawRect(gRenderer, &dimensions);
}

void KlangstromArduinoProxyEncoder::rotate(int16_t pDelta) {
    ticks += pDelta;
}

void KlangstromArduinoProxyEncoder::populate_event(EventEncoder& e) {
    e.index  = ID - ENCODER_00;
    e.ticks  = ticks;
    e.delta  = ticks - previous_ticks;
    e.button = button_pressed;
}

void KlangstromArduinoProxyEncoder::update(bool pFlagged) {
    selected = pFlagged;
    if (fMovedCounter == 0) {
        fMoved = 0;
    }
    if (ticks != previous_ticks) {
        fMoved        = ticks - previous_ticks;
        fMovedCounter = MOVED_COUNTER;
        EventEncoder e;
        populate_event(e);
        // e.index          = ID - ENCODER_00;
        // e.ticks          = ticks;
        // e.previous_ticks = previous_ticks;
        // e.button         = button_pressed;
        // call event rotated call_encoder_rotated(e.index, e.ticks, e.delta);
        // call event rotated call_encoder_rotated(uint8_t, int16_t, int16_t);
        call_encoder_rotated(e.index, e.ticks, e.delta);
        call_encoder_event_receive(EVENT_ENCODER, &e);
    }
    if (button_pressed != previous_button_pressed) {
        EventEncoder e;
        populate_event(e);
        if (button_pressed) {
            call_encoder_pressed(e.index);
        } else {
            call_encoder_released(e.index);
        }
        call_encoder_event_receive(EVENT_ENCODER, &e);
    }
    previous_button_pressed = button_pressed;
    previous_ticks          = ticks;
}

#endif  // (KLST_ARCH==KLST_ARCH_DESKTOP)
