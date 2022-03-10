#ifdef USBDEVICE_KEYBOARD

#include "usbd_def.h"
#include "usbd_hid.h"

// from https://controllerstech.com/how-to-use-stm32-as-a-keyboard/

__ALIGN_BEGIN uint8_t HID_ReportDesc[HID_REPORT_DESC_SIZE] __ALIGN_END = {
    0x05,        //
    0x01,        // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,  // USAGE (Keyboard)
    0xa1, 0x01,  // COLLECTION (Application)
    0x05, 0x07,  //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,  //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,  //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,  //   LOGICAL_MINIMUM (0)
    0x25, 0x01,  //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,  //   REPORT_SIZE (1)
    0x95, 0x08,  //   REPORT_COUNT (8)
    0x81, 0x02,  //   INPUT (Data,Var,Abs)
    0x95, 0x01,  //   REPORT_COUNT (1)
    0x75, 0x08,  //   REPORT_SIZE (8)
    0x81, 0x03,  //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,  //   REPORT_COUNT (5)
    0x75, 0x01,  //   REPORT_SIZE (1)
    0x05, 0x08,  //   USAGE_PAGE (LEDs)
    0x19, 0x01,  //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,  //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,  //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,  //   REPORT_COUNT (1)
    0x75, 0x03,  //   REPORT_SIZE (3)
    0x91, 0x03,  //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,  //   REPORT_COUNT (6)
    0x75, 0x08,  //   REPORT_SIZE (8)
    0x15, 0x00,  //   LOGICAL_MINIMUM (0)
    0x25, 0x65,  //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,  //   USAGE_PAGE (Keyboard)
    0x19, 0x00,  //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,  //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,  //   INPUT (Data,Ary,Abs)
    0xc0         // END_COLLECTION
};

#endif