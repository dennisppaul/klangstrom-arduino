/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOH
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOH
#define AUDIOCODEC_SAI_MCLK_B_Pin GPIO_PIN_0
#define AUDIOCODEC_SAI_MCLK_B_GPIO_Port GPIOC
#define AUDIOCODEC_SAI_SD_A_Pin GPIO_PIN_1
#define AUDIOCODEC_SAI_SD_A_GPIO_Port GPIOC
#define ENCODER_02_A_Pin GPIO_PIN_0
#define ENCODER_02_A_GPIO_Port GPIOA
#define ENCODER_02_B_Pin GPIO_PIN_1
#define ENCODER_02_B_GPIO_Port GPIOA
#define DAC_00_Pin GPIO_PIN_4
#define DAC_00_GPIO_Port GPIOA
#define DAC_01_Pin GPIO_PIN_5
#define DAC_01_GPIO_Port GPIOA
#define ENCODER_00_A_Pin GPIO_PIN_6
#define ENCODER_00_A_GPIO_Port GPIOA
#define ENCODER_00_B_Pin GPIO_PIN_7
#define ENCODER_00_B_GPIO_Port GPIOA
#define ENCODER_00_BUTTON_Pin GPIO_PIN_4
#define ENCODER_00_BUTTON_GPIO_Port GPIOC
#define BUTTON_PROGRAMMER_Pin GPIO_PIN_5
#define BUTTON_PROGRAMMER_GPIO_Port GPIOC
#define ADC_00_Pin GPIO_PIN_0
#define ADC_00_GPIO_Port GPIOB
#define ADC_01_Pin GPIO_PIN_1
#define ADC_01_GPIO_Port GPIOB
#define AUDIOCODEC_SAI_SCK_B_Pin GPIO_PIN_12
#define AUDIOCODEC_SAI_SCK_B_GPIO_Port GPIOB
#define SPI_SDCARD_SCK_Pin GPIO_PIN_13
#define SPI_SDCARD_SCK_GPIO_Port GPIOB
#define SPI_SDCARD_MISO_Pin GPIO_PIN_14
#define SPI_SDCARD_MISO_GPIO_Port GPIOB
#define SPI_SDCARD_MOSI_Pin GPIO_PIN_15
#define SPI_SDCARD_MOSI_GPIO_Port GPIOB
#define ENCODER_01_A_Pin GPIO_PIN_6
#define ENCODER_01_A_GPIO_Port GPIOC
#define ENCODER_01_B_Pin GPIO_PIN_7
#define ENCODER_01_B_GPIO_Port GPIOC
#define ENCODER_01_BUTTON_Pin GPIO_PIN_8
#define ENCODER_01_BUTTON_GPIO_Port GPIOC
#define AUDIOCODEC_I2C_SDA_Pin GPIO_PIN_9
#define AUDIOCODEC_I2C_SDA_GPIO_Port GPIOC
#define AUDIOCODEC_I2C_SCL_Pin GPIO_PIN_8
#define AUDIOCODEC_I2C_SCL_GPIO_Port GPIOA
#define AUDIOCODEC_SAI_SD_B_Pin GPIO_PIN_9
#define AUDIOCODEC_SAI_SD_B_GPIO_Port GPIOA
#define UART_00_RX_Pin GPIO_PIN_10
#define UART_00_RX_GPIO_Port GPIOA
#define USB_D__Pin GPIO_PIN_11
#define USB_D__GPIO_Port GPIOA
#define USB_D_A12_Pin GPIO_PIN_12
#define USB_D_A12_GPIO_Port GPIOA
#define SWD_SWDIO_Pin GPIO_PIN_13
#define SWD_SWDIO_GPIO_Port GPIOA
#define SWD_SWCLK_Pin GPIO_PIN_14
#define SWD_SWCLK_GPIO_Port GPIOA
#define SWD_SWI_Pin GPIO_PIN_15
#define SWD_SWI_GPIO_Port GPIOA
#define UART_01_TX_Pin GPIO_PIN_10
#define UART_01_TX_GPIO_Port GPIOC
#define UART_01_RX_Pin GPIO_PIN_11
#define UART_01_RX_GPIO_Port GPIOC
#define LED_02_Pin GPIO_PIN_12
#define LED_02_GPIO_Port GPIOC
#define LED_01_Pin GPIO_PIN_2
#define LED_01_GPIO_Port GPIOD
#define SWD_SWO_Pin GPIO_PIN_3
#define SWD_SWO_GPIO_Port GPIOB
#define ENCODER_02_BUTTON_Pin GPIO_PIN_4
#define ENCODER_02_BUTTON_GPIO_Port GPIOB
#define LED_00_Pin GPIO_PIN_5
#define LED_00_GPIO_Port GPIOB
#define UART_00_TX_Pin GPIO_PIN_6
#define UART_00_TX_GPIO_Port GPIOB
#define I2C_00_SDA_Pin GPIO_PIN_7
#define I2C_00_SDA_GPIO_Port GPIOB
#define I2C_00_SCL_Pin GPIO_PIN_8
#define I2C_00_SCL_GPIO_Port GPIOB
#define AUDIOCODEC_SAI_FS_B_Pin GPIO_PIN_9
#define AUDIOCODEC_SAI_FS_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
