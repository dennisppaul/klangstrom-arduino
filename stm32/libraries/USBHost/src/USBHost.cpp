#ifdef USE_USBHOST

#include "Arduino.h" // include needs to be before 'USBHost.h'
#include "KlangstromEvents.h"
#include "USBHost.h"

#ifdef __cplusplus
// extern "C" {
// #endif

extern void mouse_pressed(EVENT_DATA button, int8_t x, int8_t y);
extern void mouse_released(EVENT_DATA button, int8_t x, int8_t y);
extern void mouse_moved(const bool* buttons, int8_t x, int8_t y);
extern void key_pressed(uint8_t key);
extern void key_released(uint8_t key);
extern void event_receive(EVENT_TYPE event, const void* data);

void usb_host_callback_defaults() {
    usb_host_callback_key_pressed(key_pressed);
    usb_host_callback_key_released(key_released);
    usb_host_callback_mouse_pressed(mouse_pressed);
    usb_host_callback_mouse_released(mouse_released);
    usb_host_callback_mouse_moved(mouse_moved);
    usb_host_callback_event_receive(event_receive);
}

// #ifdef __cplusplus
// }
#endif

#endif  // USE_USBHOST
