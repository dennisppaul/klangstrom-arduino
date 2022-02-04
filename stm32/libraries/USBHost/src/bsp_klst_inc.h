#ifndef __STM32_HAL_INC_H__
#define __STM32_HAL_INC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "Arduino.h"
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

#endif /* __STM32_HAL_INC_H__ */
