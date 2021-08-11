#ifndef _TINYUSB_H_
#define _TINYUSB_H_

#include "Arduino.h"
#include "bsp_klst_inc.h"
#include "tusb.h"
#include "tusb_option.h"

#ifdef TINYUSB_MIDI_DEVICE
#include "USBDeviceMIDI.h"
#endif
#ifdef TINYUSB_MOUSE_KEYBOARD_DEVICE
#include "USBDeviceMouseKeyboard.h"
#endif

#ifdef TUSB_OPT_DEVICE_ENABLED
#endif

#endif /* _TINYUSB_H_ */
