#ifndef _USB_HOST_BSP_INC_
#define _USB_HOST_BSP_INC_

#ifdef __cplusplus
extern "C" {
#endif

#include "Arduino.h"
#include "usbh_conf.h"

#if defined(KLST_BOARD_KLST_TINY)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#elif defined(KLST_BOARD_KLST_CORE)
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#elif defined(KLST_BOARD_KLST_SHEEP)
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#else
#warning @USBHOST architecture not supported
#endif

#ifdef __cplusplus
}
#endif

#endif /* _USB_HOST_BSP_INC_ */
