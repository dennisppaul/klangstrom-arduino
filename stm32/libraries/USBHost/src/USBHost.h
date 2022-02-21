#ifndef __USBHOST__H__
#define __USBHOST__H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "Arduino.h"
#include "usb_host.h"
#include "usbh_conf.h"
#include "USBHost_BSP_inc.h"
#include "MIDI_application.h"

void usb_host_init();
void usb_host_process();

#ifdef __cplusplus
}
#endif

#endif /* __USBHOST__H__ */
