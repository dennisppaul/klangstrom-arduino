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
#include "stm32h7xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AUDIOCODEC_SAI1_MCLK_A_Pin GPIO_PIN_2
#define AUDIOCODEC_SAI1_MCLK_A_GPIO_Port GPIOE
#define AUDIOCODEC_SAI1_SD_B_Pin GPIO_PIN_3
#define AUDIOCODEC_SAI1_SD_B_GPIO_Port GPIOE
#define AUDIOCODEC_SAI1_FS_A_Pin GPIO_PIN_4
#define AUDIOCODEC_SAI1_FS_A_GPIO_Port GPIOE
#define AUDIOCODEC_SAI1_SCK_A_Pin GPIO_PIN_5
#define AUDIOCODEC_SAI1_SCK_A_GPIO_Port GPIOE
#define AUDIOCODEC_SAI1_SD_A_Pin GPIO_PIN_6
#define AUDIOCODEC_SAI1_SD_A_GPIO_Port GPIOE
#define GPIO_15_Pin GPIO_PIN_13
#define GPIO_15_GPIO_Port GPIOC
#define GPIO_14_Pin GPIO_PIN_14
#define GPIO_14_GPIO_Port GPIOC
#define GPIO_13_Pin GPIO_PIN_15
#define GPIO_13_GPIO_Port GPIOC
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOH
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOH
#define GPIO_12_Pin GPIO_PIN_0
#define GPIO_12_GPIO_Port GPIOC
#define SDCARD_CARD_DETECT_Pin GPIO_PIN_1
#define SDCARD_CARD_DETECT_GPIO_Port GPIOC
#define SDCARD_SPI_MISO_Pin GPIO_PIN_2
#define SDCARD_SPI_MISO_GPIO_Port GPIOC
#define SDCARD_SPI_MOSI_Pin GPIO_PIN_3
#define SDCARD_SPI_MOSI_GPIO_Port GPIOC
#define ENCODER_00_A_Pin GPIO_PIN_0
#define ENCODER_00_A_GPIO_Port GPIOA
#define ENCODER_00_B_Pin GPIO_PIN_1
#define ENCODER_00_B_GPIO_Port GPIOA
#define LED_15_Pin GPIO_PIN_2
#define LED_15_GPIO_Port GPIOA
#define ENCODER_00_BUTTON_Pin GPIO_PIN_3
#define ENCODER_00_BUTTON_GPIO_Port GPIOA
#define DAC_00_Pin GPIO_PIN_4
#define DAC_00_GPIO_Port GPIOA
#define DAC_01_Pin GPIO_PIN_5
#define DAC_01_GPIO_Port GPIOA
#define LED_14_Pin GPIO_PIN_6
#define LED_14_GPIO_Port GPIOA
#define LED_13_Pin GPIO_PIN_7
#define LED_13_GPIO_Port GPIOA
#define ADC_00_Pin GPIO_PIN_4
#define ADC_00_GPIO_Port GPIOC
#define ADC_01_Pin GPIO_PIN_5
#define ADC_01_GPIO_Port GPIOC
#define LED_12_Pin GPIO_PIN_0
#define LED_12_GPIO_Port GPIOB
#define LED_11_Pin GPIO_PIN_1
#define LED_11_GPIO_Port GPIOB
#define GPIO_11_Pin GPIO_PIN_2
#define GPIO_11_GPIO_Port GPIOB
#define SERIAL_00_RX_Pin GPIO_PIN_7
#define SERIAL_00_RX_GPIO_Port GPIOE
#define SERIAL_00_TX_Pin GPIO_PIN_8
#define SERIAL_00_TX_GPIO_Port GPIOE
#define GPIO_10_Pin GPIO_PIN_9
#define GPIO_10_GPIO_Port GPIOE
#define GPIO_09_Pin GPIO_PIN_10
#define GPIO_09_GPIO_Port GPIOE
#define SPI_USR_CS_Pin GPIO_PIN_11
#define SPI_USR_CS_GPIO_Port GPIOE
#define SPI_USR_SCK_Pin GPIO_PIN_12
#define SPI_USR_SCK_GPIO_Port GPIOE
#define SPI_USR_MISO_Pin GPIO_PIN_13
#define SPI_USR_MISO_GPIO_Port GPIOE
#define SPI_USR_MOSI_Pin GPIO_PIN_14
#define SPI_USR_MOSI_GPIO_Port GPIOE
#define GPIO_08_Pin GPIO_PIN_15
#define GPIO_08_GPIO_Port GPIOE
#define I2C_USR_SCL_Pin GPIO_PIN_10
#define I2C_USR_SCL_GPIO_Port GPIOB
#define I2C_USR_SDA_Pin GPIO_PIN_11
#define I2C_USR_SDA_GPIO_Port GPIOB
#define SDCARD_SPI_CS_Pin GPIO_PIN_12
#define SDCARD_SPI_CS_GPIO_Port GPIOB
#define SDCARD_SPI_SCK_Pin GPIO_PIN_13
#define SDCARD_SPI_SCK_GPIO_Port GPIOB
#define USB_HOST__Pin GPIO_PIN_14
#define USB_HOST__GPIO_Port GPIOB
#define USB_HOST_B15_Pin GPIO_PIN_15
#define USB_HOST_B15_GPIO_Port GPIOB
#define GPIO_07_Pin GPIO_PIN_8
#define GPIO_07_GPIO_Port GPIOD
#define GPIO_06_Pin GPIO_PIN_9
#define GPIO_06_GPIO_Port GPIOD
#define GPIO_05_Pin GPIO_PIN_10
#define GPIO_05_GPIO_Port GPIOD
#define GPIO_04_Pin GPIO_PIN_11
#define GPIO_04_GPIO_Port GPIOD
#define LED_10_Pin GPIO_PIN_12
#define LED_10_GPIO_Port GPIOD
#define LED_09_Pin GPIO_PIN_13
#define LED_09_GPIO_Port GPIOD
#define LED_08_Pin GPIO_PIN_14
#define LED_08_GPIO_Port GPIOD
#define LED_07_Pin GPIO_PIN_15
#define LED_07_GPIO_Port GPIOD
#define LED_06_Pin GPIO_PIN_6
#define LED_06_GPIO_Port GPIOC
#define LED_05_Pin GPIO_PIN_7
#define LED_05_GPIO_Port GPIOC
#define LED_04_Pin GPIO_PIN_8
#define LED_04_GPIO_Port GPIOC
#define LED_03_Pin GPIO_PIN_9
#define LED_03_GPIO_Port GPIOC
#define ENCODER_01_A_Pin GPIO_PIN_8
#define ENCODER_01_A_GPIO_Port GPIOA
#define ENCODER_01_B_Pin GPIO_PIN_9
#define ENCODER_01_B_GPIO_Port GPIOA
#define ENCODER_01_BUTTON_Pin GPIO_PIN_10
#define ENCODER_01_BUTTON_GPIO_Port GPIOA
#define USB_DEVICE__Pin GPIO_PIN_11
#define USB_DEVICE__GPIO_Port GPIOA
#define USB_DEVICE_A12_Pin GPIO_PIN_12
#define USB_DEVICE_A12_GPIO_Port GPIOA
#define SWD_SWDIO_Pin GPIO_PIN_13
#define SWD_SWDIO_GPIO_Port GPIOA
#define SWD_SWCLK_Pin GPIO_PIN_14
#define SWD_SWCLK_GPIO_Port GPIOA
#define SPI_DISPLAY_CS_Pin GPIO_PIN_15
#define SPI_DISPLAY_CS_GPIO_Port GPIOA
#define SPI_DISPLAY_SCK_Pin GPIO_PIN_10
#define SPI_DISPLAY_SCK_GPIO_Port GPIOC
#define SPI_DISPLAY_MISO_Pin GPIO_PIN_11
#define SPI_DISPLAY_MISO_GPIO_Port GPIOC
#define SPI_DISPLAY_MOSI_Pin GPIO_PIN_12
#define SPI_DISPLAY_MOSI_GPIO_Port GPIOC
#define SPI_DISPLAY_REGISTER_SELECT_Pin GPIO_PIN_0
#define SPI_DISPLAY_REGISTER_SELECT_GPIO_Port GPIOD
#define SPI_DISPLAY_RESET_Pin GPIO_PIN_1
#define SPI_DISPLAY_RESET_GPIO_Port GPIOD
#define GPIO_03_Pin GPIO_PIN_2
#define GPIO_03_GPIO_Port GPIOD
#define GPIO_02_Pin GPIO_PIN_3
#define GPIO_02_GPIO_Port GPIOD
#define GPIO_01_Pin GPIO_PIN_4
#define GPIO_01_GPIO_Port GPIOD
#define SERIAL_DEBUG_TX_Pin GPIO_PIN_5
#define SERIAL_DEBUG_TX_GPIO_Port GPIOD
#define SERIAL_DEBUG_RX_Pin GPIO_PIN_6
#define SERIAL_DEBUG_RX_GPIO_Port GPIOD
#define GPIO_00_Pin GPIO_PIN_7
#define GPIO_00_GPIO_Port GPIOD
#define SWD_SWO_Pin GPIO_PIN_3
#define SWD_SWO_GPIO_Port GPIOB
#define LED_02_Pin GPIO_PIN_4
#define LED_02_GPIO_Port GPIOB
#define LED_01_Pin GPIO_PIN_5
#define LED_01_GPIO_Port GPIOB
#define AUDIOCODEC_I2C_SCL_Pin GPIO_PIN_6
#define AUDIOCODEC_I2C_SCL_GPIO_Port GPIOB
#define AUDIOCODEC_I2C_SDA_Pin GPIO_PIN_7
#define AUDIOCODEC_I2C_SDA_GPIO_Port GPIOB
#define LED_00_Pin GPIO_PIN_8
#define LED_00_GPIO_Port GPIOB
#define BUTTON_PRG_Pin GPIO_PIN_9
#define BUTTON_PRG_GPIO_Port GPIOB
#define SERIAL_01_RX_Pin GPIO_PIN_0
#define SERIAL_01_RX_GPIO_Port GPIOE
#define SERIAL_01_TX_Pin GPIO_PIN_1
#define SERIAL_01_TX_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
