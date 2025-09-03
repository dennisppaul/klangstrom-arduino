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

#include "usbd_conf.h"
#include "usb_device.h"
#include "USBDevices.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAL_PCD_MODULE_ENABLED
#error "HAL_PCD_MODULE_ENABLED not defined"
#endif

// note make sure that `RCC_OSCILLATORTYPE_HSI48` is present in `SystemClock_Config()`

extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

void usb_devices_init() {
    MX_USB_DEVICE_Init();
}

/**
      * @brief This function handles USB On The Go HS End Point 1 Out global interrupt.
      */
void OTG_HS_EP1_OUT_IRQHandler(void) {
    /* USER CODE BEGIN OTG_HS_EP1_OUT_IRQn 0 */

    /* USER CODE END OTG_HS_EP1_OUT_IRQn 0 */
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
    /* USER CODE BEGIN OTG_HS_EP1_OUT_IRQn 1 */

    /* USER CODE END OTG_HS_EP1_OUT_IRQn 1 */
}

/**
      * @brief This function handles USB On The Go HS End Point 1 In global interrupt.
      */
void OTG_HS_EP1_IN_IRQHandler(void) {
    /* USER CODE BEGIN OTG_HS_EP1_IN_IRQn 0 */

    /* USER CODE END OTG_HS_EP1_IN_IRQn 0 */
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
    /* USER CODE BEGIN OTG_HS_EP1_IN_IRQn 1 */

    /* USER CODE END OTG_HS_EP1_IN_IRQn 1 */
}

/**
      * @brief This function handles USB On The Go HS global interrupt.
      */
void OTG_HS_IRQHandler(void) {
    /* USER CODE BEGIN OTG_HS_IRQn 0 */
    USBD_UsrLog("OTG_HS_IRQHandler");
    /* USER CODE END OTG_HS_IRQn 0 */
    HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
    /* USER CODE BEGIN OTG_HS_IRQn 1 */

    /* USER CODE END OTG_HS_IRQn 1 */
}

#ifdef __cplusplus
}
#endif
