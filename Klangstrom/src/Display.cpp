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

#include "System.h"
#include "Console.h"
#include "Display.h"

#ifdef __cplusplus
extern "C" {
#endif

WEAK void display_update_event() {}
WEAK void display_touch_event(TouchEvent* touchevent) {}

Callback_0_VOID          fUpdateCallback = display_update_event;
Callback_1_TOUCHEVENTPTR fTouchCallback  = display_touch_event;

bool fIsDoubleBuffer = false;

bool display_init(const bool double_buffered, const TouchPanelMode touch_panel_mode) {
    fIsDoubleBuffer = double_buffered;
    if (!fIsDoubleBuffer) {
        console_status("display is using single buffer mode");
    }
    return display_init_BSP(touch_panel_mode);
}

void display_set_update_callback(Callback_0_VOID callback) {
    fUpdateCallback = callback;
}

void display_set_touch_callback(Callback_1_TOUCHEVENTPTR callback) {
    fTouchCallback = callback;
}

void display_fire_touch_callback(TouchEvent* touchevent) {
    if (fTouchCallback != nullptr) {
        fTouchCallback(touchevent);
    }
}

void display_fire_update_callback() {
    if (fUpdateCallback != nullptr) {
        fUpdateCallback();
    }
}

bool display_is_double_buffered() {
    return fIsDoubleBuffer;
}

#ifdef __cplusplus
}
#endif
