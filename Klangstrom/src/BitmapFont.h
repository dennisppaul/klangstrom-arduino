/**
  ******************************************************************************
  * @file    BitmapFont.h
  * @author  MCD Application Team
  * @brief   This file provide definition for fonts to be used for STM32xx-EVAL's
  *          LCD driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FONTS_H
#define FONTS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

typedef struct BitmapFont
{
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
} BitmapFont;

extern BitmapFont Font24;
extern BitmapFont Font20;
extern BitmapFont Font16;
extern BitmapFont Font12;
extern BitmapFont Font8;
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* FONTS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
