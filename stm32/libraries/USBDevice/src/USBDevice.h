#ifndef _USBDEVICE_H_
#define _USBDEVICE_H_

#ifndef USE_USBDEVICE
#error "+++ ERROR no USB Device type selected! select from `Tools > USB support (if available) > USB Device …` */"
#else

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_device.h"
#include "usbd_conf.h"
#include "usbd_core.h"
#include "usbd_ctlreq.h"
#include "usbd_def.h"
#include "usbd_desc.h"
#include "usbd_ioreq.h"

void usb_device_init();

uint8_t transmit_keyboard_key_pressed(uint8_t pKey);
uint8_t transmit_keyboard_key_released(uint8_t pKey);
void    transmit_mouse_moved(const int8_t x, const int8_t y);
void    transmit_mouse_button(const uint8_t button);
void    transmit_midi_note_off(const uint8_t channel, const uint8_t note);
void    transmit_midi_note_on(const uint8_t channel, const uint8_t note, const uint8_t velocity);

#ifdef __cplusplus
}
#endif

#endif  // USE_USBDEVICE
#endif  /* _USBDEVICE_H_ */
