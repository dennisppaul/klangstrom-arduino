#ifndef _USB_DEVICE_MOUSE_KEYBOARD_H_
#define _USB_DEVICE_MOUSE_KEYBOARD_H_

#ifndef TINYUSB_MOUSE_KEYBOARD_DEVICE
#error "@USB Device :: select `Tools > USB support > USB Device Mouse+Keyboard` from menu"
#endif

void usb_device_mouse_keyboard_init();
void usb_device_mouse_keyboard_process();
uint8_t usb_device_mouse_keyboard_state();

void transmit_keyboard_key_pressed(char pKey);
void transmit_keyboard_key_released();
void transmit_mouse_moved(int8_t x, int8_t y);

#endif // _USB_DEVICE_MOUSE_KEYBOARD_H_
