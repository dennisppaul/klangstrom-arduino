#ifndef MIDI_APPLICATION_H_
#define MIDI_APPLICATION_H_

#include "stdio.h"
#include "usbh_core.h"
#include "usbh_MIDI.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

extern USBH_HandleTypeDef hUsbHostFS;
void MIDI_Application(void);

#endif /* MIDI_APPLICATION_H_ */
