#ifndef __USBHOST__H__
#define __USBHOST__H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "Arduino.h"
#include "usb_host.h"
#include "usbh_conf.h"
#include "bsp_klst_inc.h"

#define WEAK __attribute__ ((weak))

/* --- callback functions --- */

WEAK void receive_midi_note_on(uint8_t pChannel, uint8_t pNote, uint8_t pVelocity) {}
WEAK void receive_midi_note_off(uint8_t pChannel, uint8_t pNote, uint8_t pVelocity) {}
WEAK void receive_midi_pitch_bend(uint8_t pChannel, int16_t pValue) {}
WEAK void receive_midi_control_change(uint8_t pChannel, uint8_t pController, uint8_t pValue) {}
// WEAK void receive_midi_note_on(uint8_t pNote, uint8_t pVelocity);
// WEAK void receive_midi_note_off(uint8_t pNote, uint8_t pVelocity);
// WEAK void receive_midi_control_change(uint8_t pCC, uint8_t pValue);
// WEAK void receive_midi_pitch_bend(int16_t pValue);
WEAK void receive_mouse(uint8_t x, uint8_t y, uint8_t* buttons);
WEAK void receive_keyboard(uint8_t* keys);

void usb_host_init();
void usb_host_process();

#ifdef __cplusplus
}
#endif

#endif /* __USBHOST__H__ */
