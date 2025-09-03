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


#include <stdint.h>
#include <stdbool.h>

#include "usb_host.h"
#include "usbh_conf.h"
#include "USBHost.h"
#include "USBHostCallbacks.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAL_HCD_MODULE_ENABLED
#error "HAL_HCD_MODULE_ENABLED not defined"
#endif

// note make sure that `RCC_OSCILLATORTYPE_HSI48` is present in `SystemClock_Config()`

extern HCD_HandleTypeDef hhcd_USB_OTG_HS;

void usb_host_init() {
    MX_USB_HOST_Init();
}

void usb_host_process() {
    MX_USB_HOST_Process();
}

/* --- members in USE_USBHOST--- */

bool usb_host_keyboard_connected = false; // used in `usbh_hid.c`
bool usb_host_mouse_connected    = false; // used in `usbh_hid.c`
bool usb_host_midi_connected     = false; // used in `usbh_MIDI.c`

/* --- functions in USE_USBHOST--- */

bool usb_host_is_mouse_connected() {
    return usb_host_mouse_connected;
}

bool usb_host_is_keyboard_connected() {
    return usb_host_keyboard_connected;
}

bool usb_host_is_midi_connected() {
    return usb_host_midi_connected;
}


/**
  * @brief This function handles USB On The Go HS global interrupt.
  */
void OTG_HS_IRQHandler(void) {
    /* USER CODE BEGIN OTG_HS_IRQn 0 */

    /* USER CODE END OTG_HS_IRQn 0 */
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
    /* USER CODE BEGIN OTG_HS_IRQn 1 */

    /* USER CODE END OTG_HS_IRQn 1 */
}

#ifdef __cplusplus
}
#endif
