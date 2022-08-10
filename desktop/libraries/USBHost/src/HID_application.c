/*
 * HID_application.c
 */

#ifdef USE_USBHOST

#include "USBHost.h"
#include "usb_host.h"
#include "usbh_def.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"

/* callback function prototypes -----------------------------------------------*/

#define NUM_OF_SIMULTANEOUS_KEYS 3

WEAK void receive_mouse(uint8_t x, uint8_t y, uint8_t* buttons) {}
WEAK void receive_keyboard(uint8_t* keys) {
  USBH_UsrLog("receive_keyboard");
}

/*-----------------------------------------------------------------------------*/

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {
  USBH_UsrLog("USBH_HID_EventCallback");
	if (USBH_HID_GetDeviceType(phost) == HID_MOUSE) {
	  HID_MOUSE_Info_TypeDef *mMouseInfo = USBH_HID_GetMouseInfo(phost);
    receive_mouse(mMouseInfo->x, mMouseInfo->y, mMouseInfo->buttons);
    USBH_UsrLog("mouse ...... : %i, %i, (%i, %i, %i)", mMouseInfo->x, mMouseInfo->y, mMouseInfo->buttons[0], mMouseInfo->buttons[1], mMouseInfo->buttons[2]);

// 		mouse.button = (mMouseInfo->buttons[0]);
// 		// TODO map to buttons B_L == mMouseInfo->buttons[2] + B_R == mMouseInfo->buttons[1]
// 		mouse.x += unsigned_to_signed(mMouseInfo->x, 256);
// 		mouse.y += unsigned_to_signed(mMouseInfo->y, 256);
//
// 		/* map mouse middle and right mouse button to hardware button */
// 		mHWMouseButtonMiddle = (mMouseInfo->buttons[2]);
// 		mHWMouseButtonRight  = (mMouseInfo->buttons[1]);

		// TODO polling mouse info twice is a nasty hack,
		// but without this mouse events are always one event behind.
		// investigate why this is necessary and why it works?
//		USBH_HID_GetMouseInfo(phost);
	} else if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD) {
    	HID_KEYBD_Info_TypeDef * mInfo = USBH_HID_GetKeybdInfo(phost);
    	for (uint8_t i=0; i<NUM_OF_SIMULTANEOUS_KEYS; i++) {
            USBH_UsrLog("keyboard ... : [%i] %i",  i, mInfo->keys[i]);
    	}
    	// @todo(add special keys)
    	/*
    	  uint8_t lctrl;
        uint8_t lshift;
        uint8_t lalt;
        uint8_t lgui;
        uint8_t rctrl;
        uint8_t rshift;
        uint8_t ralt;
        uint8_t rgui;
    	*/
      if (mInfo->lshift) {
          USBH_UsrLog("keyboard ... : SHIFT");
      }
      receive_keyboard(mInfo->keys);
//       USBH_HID_GetKeybdInfo(phost);
    }
}

#endif // USE_USBHOST
