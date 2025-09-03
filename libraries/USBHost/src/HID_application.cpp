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

#include "usbh_conf.h"
#include "usb_host.h"
#include "USBHost.h"
#include "usbh_def.h"
#include "usbh_hid_keybd.h"
#include "usbh_hid_mouse.h"
#include "USBHostCallbacks.h"
#include "USBHostConstants.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EventMouse {
    int8_t x;
    int8_t y;
    bool   LEFT;
    bool   MIDDLE;
    bool   RIGHT;
} EventMouse;

static uint8_t fPreviousKeys[KEYBOARD_NUM_KEYS]         = {0};
static bool    fPreviousMouseButtons[MOUSE_NUM_BUTTONS] = {false};
static int8_t  fPreviousMouseX                          = 0;
static int8_t  fPreviousMouseY                          = 0;

void handleMouse(HID_MOUSE_Info_TypeDef* mMouseInfo) {
    const int8_t x = (int8_t) (mMouseInfo->x);
    const int8_t y = (int8_t) (mMouseInfo->y);
    for (uint8_t i = 0; i < MOUSE_NUM_BUTTONS; i++) {
        if (fPreviousMouseButtons[i] && !mMouseInfo->buttons[i]) {
            usb_host_call_mouse_released(i, x, y);
        } else if (!fPreviousMouseButtons[i] && mMouseInfo->buttons[i]) {
            usb_host_call_mouse_pressed(i, x, y);
        }
        fPreviousMouseButtons[i] = mMouseInfo->buttons[i];
    }
    if (fPreviousMouseX != x || fPreviousMouseY != y) {
        usb_host_call_mouse_moved(fPreviousMouseButtons, x, y);
        printf("x:%i y:%i\r\n", x, y);
    }
    fPreviousMouseX = x;
    fPreviousMouseY = y;

    EventMouse mEventMouse;
    mEventMouse.x      = x;
    mEventMouse.y      = y;
    mEventMouse.LEFT   = mMouseInfo->buttons[MOUSE_BUTTON_LEFT];
    mEventMouse.RIGHT  = mMouseInfo->buttons[MOUSE_BUTTON_RIGHT];
    mEventMouse.MIDDLE = mMouseInfo->buttons[MOUSE_BUTTON_MIDDLE];
    usb_host_call_event_receive(EVENT_MOUSE, &mEventMouse);
}

bool evaluateKeyAlreadyPressed(uint8_t* pPreviousKeys, uint8_t pKey) {
    for (uint8_t i = 0; i < KEYBOARD_NUM_KEYS; i++) {
        if (pPreviousKeys[i] == pKey) {
            pPreviousKeys[i] = 0;
            return true;
        }
    }
    return false;
}

void handleKeyboard(HID_KEYBD_Info_TypeDef* mInfo) {
    /* TODO we are using the interpreted ASCII values, rather than the *raw* integer values, maybe those could be also send as `keyCode` */
    uint8_t mPreviousKeys[KEYBOARD_NUM_KEYS];
    memcpy(mPreviousKeys, fPreviousKeys, KEYBOARD_NUM_KEYS * sizeof(uint8_t));

    /* `key_pressed` :: find keys that were not previously pressed */
    for (uint8_t i = 0; i < KEYBOARD_NUM_KEYS; i++) {
        uint8_t key      = USBH_HID_GetASCIICode(mInfo, i);
        fPreviousKeys[i] = key; // store key(s) for next pass
        // TODO maybe remove key TAB(9), RETURN(10) and BACKSPACE(13) from this list?
        if (key > 0) {
            bool mKeyAlreadyPressed = evaluateKeyAlreadyPressed(mPreviousKeys, key);
            if (!mKeyAlreadyPressed) {
                usb_host_call_key_pressed(key);
            }
        }
    }

    /* `key_released` :: find keys that were previously pressed, but are not anymore */
    for (uint8_t i = 0; i < KEYBOARD_NUM_KEYS; i++) {
        if (mPreviousKeys[i] > 0) {
            usb_host_call_key_released(mPreviousKeys[i]);
        }
    }

    /* transmit entire keyboard event */
    EventKeyboard mEventKeyboard;
    mEventKeyboard.keys        = fPreviousKeys;
    mEventKeyboard.keycodes    = mInfo->keys;
    mEventKeyboard.SHIFT_LEFT  = mInfo->lshift;
    mEventKeyboard.SHIFT_RIGHT = mInfo->rshift;
    mEventKeyboard.CTRL_LEFT   = mInfo->lctrl;
    mEventKeyboard.CTRL_RIGHT  = mInfo->rctrl;
    mEventKeyboard.ALT_LEFT    = mInfo->lalt;
    mEventKeyboard.ALT_RIGHT   = mInfo->ralt;
    mEventKeyboard.GUI_LEFT    = mInfo->lgui;
    mEventKeyboard.GUI_RIGHT   = mInfo->rgui;
    usb_host_call_event_receive(EVENT_KEYBOARD, (void*) (&mEventKeyboard));
}

void USBH_HID_EventCallback(USBH_HandleTypeDef* phost) {
    if (USBH_HID_GetDeviceType(phost) == HID_MOUSE) {
        HID_MOUSE_Info_TypeDef* mInfo = USBH_HID_GetMouseInfo(phost);
        handleMouse(mInfo);
    } else if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD) {
        HID_KEYBD_Info_TypeDef* mInfo = USBH_HID_GetKeybdInfo(phost);
        handleKeyboard(mInfo);
    }
}

#ifdef __cplusplus
}
#endif
