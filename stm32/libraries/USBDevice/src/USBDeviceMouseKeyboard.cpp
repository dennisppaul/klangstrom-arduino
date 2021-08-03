#ifdef TINYUSB_MOUSE_KEYBOARD_DEVICE

#include "USBDevice.h"
#include "USBDeviceMouseKeyboard.h"

// Interface index depends on the order in configuration descriptor
enum {
  ITF_KEYBOARD = 0,
  ITF_MOUSE = 1
};

enum  {
  MOUSE_KEYBOARD_NOT_MOUNTED = 0,
  MOUSE_KEYBOARD_MOUNTED = 1,
  MOUSE_KEYBOARD_SUSPENDED = 2,
};

static uint32_t mState = MOUSE_KEYBOARD_NOT_MOUNTED;

void usb_device_mouse_keyboard_init() {
    tusb_init();
}

void usb_device_mouse_keyboard_process() {
    tud_task();
}

uint8_t usb_device_mouse_keyboard_state() {
    return (uint8_t)mState;
}

static bool has_key = false;

void transmit_keyboard_key_pressed(char pKey) {
  if ( tud_hid_n_ready(ITF_KEYBOARD) ) {
    uint8_t keycode[6] = { 0 };
    // keycode[0] = HID_KEY_A;
    keycode[0] = pKey - 93;
    tud_hid_n_keyboard_report(ITF_KEYBOARD, 0, 0, keycode);
    has_key = true;
  }
}

void transmit_keyboard_key_released() {
  if ( tud_hid_n_ready(ITF_KEYBOARD) ) {
    // send empty key report if previously has key pressed
    if (has_key) {
      tud_hid_n_keyboard_report(ITF_KEYBOARD, 0, 0, NULL);
    }
    has_key = false;
  }
}

void transmit_mouse_moved(int8_t x, int8_t y) {
  if ( tud_hid_n_ready(ITF_MOUSE) ) {
    // no button, left/right + up/down, no scroll pan
    tud_hid_n_mouse_report(ITF_MOUSE, 0, 0x00, x, y, 0, 0);
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  mState = MOUSE_KEYBOARD_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  mState = MOUSE_KEYBOARD_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  mState = MOUSE_KEYBOARD_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  mState = MOUSE_KEYBOARD_MOUNTED;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) itf;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  // TODO set LED based on CAPLOCK, NUMLOCK etc...
  (void) itf;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) bufsize;
}

#endif // TINYUSB_MOUSE_KEYBOARD_DEVICE
