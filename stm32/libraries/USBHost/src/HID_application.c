/*
 * HID_application.c
 */

#ifdef USE_USBHOST

#include "USBHost.h"
#include "usb_host.h"
#include "usbh_def.h"
#include "usbh_hid_keybd.h"
#include "usbh_hid_mouse.h"

#define NUM_OF_SIMULTANEOUS_KEYS 6
#define NUM_OF_MOUSE_BUTTONS     3

static uint8_t fPreviousKeys[NUM_OF_SIMULTANEOUS_KEYS]     = {0};
static bool    fPreviousMouseButtons[NUM_OF_MOUSE_BUTTONS] = {false};
static int8_t  fPreviousMouseX                             = 0;
static int8_t  fPreviousMouseY                             = 0;

void handleMouse(HID_MOUSE_Info_TypeDef* mMouseInfo) {
    // TODO is that still an issue?
    // TODO polling mouse info twice is a nasty hack,
    // but without this mouse events are always one event behind.
    // investigate why this is necessary and why it works?
    //		USBH_HID_GetMouseInfo(phost);
    const int8_t x = (int8_t)(mMouseInfo->x);
    const int8_t y = (int8_t)(mMouseInfo->y);
    for (uint8_t i = 0; i < NUM_OF_MOUSE_BUTTONS; i++) {
        if (fPreviousMouseButtons[i] && !mMouseInfo->buttons[i]) {
            usb_host_call_mouse_released(i, x, y);
        } else if (!fPreviousMouseButtons[i] && mMouseInfo->buttons[i]) {
            usb_host_call_mouse_pressed(i, x, y);
        }
        fPreviousMouseButtons[i] = mMouseInfo->buttons[i];
    }
    if (fPreviousMouseX != x || fPreviousMouseY != y) {
        usb_host_call_mouse_moved(fPreviousMouseButtons, x, y);
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
    for (uint8_t i = 0; i < NUM_OF_SIMULTANEOUS_KEYS; i++) {
        if (pPreviousKeys[i] == pKey) {
            pPreviousKeys[i] = 0;
            return true;
        }
    }
    return false;
}

void handleKeyboard(HID_KEYBD_Info_TypeDef* mInfo) {
    /* TODO we are using the interpreted ASCII values, rather than the *raw* integer values, maybe those could be also send as `keyCode` */
    uint8_t mPreviousKeys[NUM_OF_SIMULTANEOUS_KEYS];
    memcpy(mPreviousKeys, fPreviousKeys, NUM_OF_SIMULTANEOUS_KEYS * sizeof(uint8_t));

    /* `key_pressed` :: find keys that were not previously pressed */
    for (uint8_t i = 0; i < NUM_OF_SIMULTANEOUS_KEYS; i++) {
        uint8_t key      = USBH_HID_GetASCIICode(mInfo, i);
        fPreviousKeys[i] = key;  // store key(s) for next pass
        if (key > 0) {
            bool mKeyAlreadyPressed = evaluateKeyAlreadyPressed(mPreviousKeys, key);
            if (!mKeyAlreadyPressed) {
                usb_host_call_key_pressed(key);
            }
        }
    }

    /* `key_released` :: find keys that were previously pressed, but are not anymore */
    for (uint8_t i = 0; i < NUM_OF_SIMULTANEOUS_KEYS; i++) {
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
    usb_host_call_event_receive(EVENT_KEYBOARD, (void*)(&mEventKeyboard));
}

void USBH_HID_EventCallback(USBH_HandleTypeDef* phost) {
    USBH_UsrLog("USBH_HID_EventCallback");
    if (USBH_HID_GetDeviceType(phost) == HID_MOUSE) {
        HID_MOUSE_Info_TypeDef* mInfo = USBH_HID_GetMouseInfo(phost);
        handleMouse(mInfo);
    } else if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD) {
        HID_KEYBD_Info_TypeDef* mInfo = USBH_HID_GetKeybdInfo(phost);
        handleKeyboard(mInfo);
    }
}

#endif  // USE_USBHOST
