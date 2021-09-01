#ifndef __CMSIS_DSP_H__
#define __CMSIS_DSP_H__


#ifdef KLST_BOARD_KLST_TINY
  #define ARM_MATH_CM4
#endif

#ifdef KLST_BOARD_KLST_CORE
  #define ARM_MATH_CM7
#endif

#include "arm_math.h"

#endif /* __CMSIS_DSP_H__ */
