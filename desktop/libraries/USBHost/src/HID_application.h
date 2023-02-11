#ifndef _HID_APPLICATION_H_
#define _HID_APPLICATION_H_

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#if __has_include("KlangstromEvents.h")
#include "KlangstromEvents.h"
#else
#error "'KlangstromEvents.h' or 'Klangstrom.h' needs to be included in sketch before 'USBHost.h'"
#endif

#include "usbh_core.h"
#include "usbh_MIDI.h"

#endif /* _HID_APPLICATION_H_ */
