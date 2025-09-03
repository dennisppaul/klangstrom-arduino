/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "usb_host.h"
#include "USBHostCallbacks.h"

#ifdef __cplusplus
extern "C" {
#endif

void usb_host_init();
void usb_host_process();
bool usb_host_is_mouse_connected();
bool usb_host_is_keyboard_connected();
bool usb_host_is_midi_connected();

// TODO add `usb_host_transmit/send_midi_*`

#ifdef KSLT_USB_HOST_DECLARE_FUNCTIONS
void OTG_HS_EP1_OUT_IRQHandler(void);
void OTG_HS_EP1_IN_IRQHandler(void);
void OTG_HS_IRQHandler(void);
void MX_USB_HOST_Process(void);
#endif // KSLT_USB_HOST_DECLARE_FUNCTIONS

#ifdef __cplusplus
}
#endif