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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SDCARD_SCK_Pin GPIO_PIN_2
#define SDCARD_SCK_GPIO_Port GPIOE
#define GPIO_01_Pin GPIO_PIN_3
#define GPIO_01_GPIO_Port GPIOE
#define SDCARD_CS_Pin GPIO_PIN_4
#define SDCARD_CS_GPIO_Port GPIOE
#define SDCARD_MISO_Pin GPIO_PIN_5
#define SDCARD_MISO_GPIO_Port GPIOE
#define SDCARD_MOSI_Pin GPIO_PIN_6
#define SDCARD_MOSI_GPIO_Port GPIOE
#define GPIO_03_Pin GPIO_PIN_13
#define GPIO_03_GPIO_Port GPIOC
#define I2C_00_SDA_Pin GPIO_PIN_0
#define I2C_00_SDA_GPIO_Port GPIOF
#define I2C_00_SCL_Pin GPIO_PIN_1
#define I2C_00_SCL_GPIO_Port GPIOF
#define GPIO_04_Pin GPIO_PIN_2
#define GPIO_04_GPIO_Port GPIOF
#define GPIO_05_Pin GPIO_PIN_3
#define GPIO_05_GPIO_Port GPIOF
#define GPIO_06_Pin GPIO_PIN_4
#define GPIO_06_GPIO_Port GPIOF
#define ADC_00_Pin GPIO_PIN_5
#define ADC_00_GPIO_Port GPIOF
#define ADC_03_Pin GPIO_PIN_6
#define ADC_03_GPIO_Port GPIOF
#define SPI_00_SCK_Pin GPIO_PIN_7
#define SPI_00_SCK_GPIO_Port GPIOF
#define SPI_00_MISO_Pin GPIO_PIN_8
#define SPI_00_MISO_GPIO_Port GPIOF
#define SPI_00_MOSI_Pin GPIO_PIN_9
#define SPI_00_MOSI_GPIO_Port GPIOF
#define GPIO_07_Pin GPIO_PIN_10
#define GPIO_07_GPIO_Port GPIOF
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOH
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOH
#define GPIO_08_Pin GPIO_PIN_0
#define GPIO_08_GPIO_Port GPIOC
#define GPIO_09_Pin GPIO_PIN_1
#define GPIO_09_GPIO_Port GPIOC
#define ENCODER_02_A_Pin GPIO_PIN_0
#define ENCODER_02_A_GPIO_Port GPIOA
#define ENCODER_02_B_Pin GPIO_PIN_1
#define ENCODER_02_B_GPIO_Port GPIOA
#define USART_02_TX_Pin GPIO_PIN_2
#define USART_02_TX_GPIO_Port GPIOA
#define USART_02_RX_Pin GPIO_PIN_3
#define USART_02_RX_GPIO_Port GPIOA
#define DAC_01_Pin GPIO_PIN_4
#define DAC_01_GPIO_Port GPIOA
#define DAC_00_Pin GPIO_PIN_5
#define DAC_00_GPIO_Port GPIOA
#define SPI_01_MISO_Pin GPIO_PIN_6
#define SPI_01_MISO_GPIO_Port GPIOA
#define SPI_02_MOSI_Pin GPIO_PIN_7
#define SPI_02_MOSI_GPIO_Port GPIOA
#define GPIO_10_Pin GPIO_PIN_4
#define GPIO_10_GPIO_Port GPIOC
#define GPIO_11_Pin GPIO_PIN_5
#define GPIO_11_GPIO_Port GPIOC
#define GPIO_12_Pin GPIO_PIN_0
#define GPIO_12_GPIO_Port GPIOB
#define GPIO_13_Pin GPIO_PIN_1
#define GPIO_13_GPIO_Port GPIOB
#define ADC_01_Pin GPIO_PIN_11
#define ADC_01_GPIO_Port GPIOF
#define ADC_04_Pin GPIO_PIN_12
#define ADC_04_GPIO_Port GPIOF
#define ADC_02_Pin GPIO_PIN_13
#define ADC_02_GPIO_Port GPIOF
#define ADC_05_Pin GPIO_PIN_14
#define ADC_05_GPIO_Port GPIOF
#define I2C_02_SDA_Pin GPIO_PIN_15
#define I2C_02_SDA_GPIO_Port GPIOF
#define GPIO_14_Pin GPIO_PIN_0
#define GPIO_14_GPIO_Port GPIOG
#define GPIO_15_Pin GPIO_PIN_1
#define GPIO_15_GPIO_Port GPIOG
#define GPIO_00_Pin GPIO_PIN_7
#define GPIO_00_GPIO_Port GPIOE
#define ENCODER_01_A_Pin GPIO_PIN_9
#define ENCODER_01_A_GPIO_Port GPIOE
#define ENCODER_01_B_Pin GPIO_PIN_11
#define ENCODER_01_B_GPIO_Port GPIOE
#define AUDIOCODEC_I2S_CK_Pin GPIO_PIN_10
#define AUDIOCODEC_I2S_CK_GPIO_Port GPIOB
#define ENCODER_00_A_Pin GPIO_PIN_10
#define ENCODER_00_A_GPIO_Port GPIOH
#define ENCODER_00_B_Pin GPIO_PIN_11
#define ENCODER_00_B_GPIO_Port GPIOH
#define AUDIOCODEC_I2S_WS_Pin GPIO_PIN_12
#define AUDIOCODEC_I2S_WS_GPIO_Port GPIOB
#define USB_HOST_DM_Pin GPIO_PIN_14
#define USB_HOST_DM_GPIO_Port GPIOB
#define USB_HOST_DP_Pin GPIO_PIN_15
#define USB_HOST_DP_GPIO_Port GPIOB
#define RJ45_UART_TX_Pin GPIO_PIN_8
#define RJ45_UART_TX_GPIO_Port GPIOD
#define RJ45_UART_RX_Pin GPIO_PIN_9
#define RJ45_UART_RX_GPIO_Port GPIOD
#define I2C_02_SCL_Pin GPIO_PIN_12
#define I2C_02_SCL_GPIO_Port GPIOD
#define BUTTON_USER_01_Pin GPIO_PIN_14
#define BUTTON_USER_01_GPIO_Port GPIOD
#define BUTTON_USER_00_Pin GPIO_PIN_15
#define BUTTON_USER_00_GPIO_Port GPIOD
#define BUTTON_PROGRAMMER_Pin GPIO_PIN_2
#define BUTTON_PROGRAMMER_GPIO_Port GPIOG
#define LED_01_Pin GPIO_PIN_3
#define LED_01_GPIO_Port GPIOG
#define LED_02_Pin GPIO_PIN_4
#define LED_02_GPIO_Port GPIOG
#define LED_00_Pin GPIO_PIN_5
#define LED_00_GPIO_Port GPIOG
#define ENCODER_01_BUTTON_Pin GPIO_PIN_6
#define ENCODER_01_BUTTON_GPIO_Port GPIOG
#define ENCODER_02_BUTTON_Pin GPIO_PIN_7
#define ENCODER_02_BUTTON_GPIO_Port GPIOG
#define ENCODER_00_BUTTON_Pin GPIO_PIN_8
#define ENCODER_00_BUTTON_GPIO_Port GPIOG
#define AUDIOCODEC_I2S_MCK_Pin GPIO_PIN_6
#define AUDIOCODEC_I2S_MCK_GPIO_Port GPIOC
#define AUDIOCODEC_I2C_SDA_Pin GPIO_PIN_9
#define AUDIOCODEC_I2C_SDA_GPIO_Port GPIOC
#define AUDIOCODEC_I2C_SCL_Pin GPIO_PIN_8
#define AUDIOCODEC_I2C_SCL_GPIO_Port GPIOA
#define USART_01_TX_Pin GPIO_PIN_9
#define USART_01_TX_GPIO_Port GPIOA
#define USART_01_RX_Pin GPIO_PIN_10
#define USART_01_RX_GPIO_Port GPIOA
#define USB_DEVICE_DM_Pin GPIO_PIN_11
#define USB_DEVICE_DM_GPIO_Port GPIOA
#define USB_DEVICE_DP_Pin GPIO_PIN_12
#define USB_DEVICE_DP_GPIO_Port GPIOA
#define SWD_SWDIO_Pin GPIO_PIN_13
#define SWD_SWDIO_GPIO_Port GPIOA
#define AUDIOCODEC_I2S_SDI_Pin GPIO_PIN_2
#define AUDIOCODEC_I2S_SDI_GPIO_Port GPIOI
#define AUDIOCODEC_I2S_SDO_Pin GPIO_PIN_3
#define AUDIOCODEC_I2S_SDO_GPIO_Port GPIOI
#define SWD_SWCLK_Pin GPIO_PIN_14
#define SWD_SWCLK_GPIO_Port GPIOA
#define RJ45_SPI_SCK_Pin GPIO_PIN_10
#define RJ45_SPI_SCK_GPIO_Port GPIOC
#define RJ45_SPI_MISO_Pin GPIO_PIN_11
#define RJ45_SPI_MISO_GPIO_Port GPIOC
#define RJ45_SPI_MOSI_Pin GPIO_PIN_12
#define RJ45_SPI_MOSI_GPIO_Port GPIOC
#define SPI_01_MOSI_Pin GPIO_PIN_7
#define SPI_01_MOSI_GPIO_Port GPIOD
#define USART_00_RX_Pin GPIO_PIN_9
#define USART_00_RX_GPIO_Port GPIOG
#define SPI_01_SCK_Pin GPIO_PIN_11
#define SPI_01_SCK_GPIO_Port GPIOG
#define SPI_02_MISO_Pin GPIO_PIN_12
#define SPI_02_MISO_GPIO_Port GPIOG
#define SPI_02_SCK_Pin GPIO_PIN_13
#define SPI_02_SCK_GPIO_Port GPIOG
#define USART_00_TX_Pin GPIO_PIN_14
#define USART_00_TX_GPIO_Port GPIOG
#define SWD_SWO_Pin GPIO_PIN_3
#define SWD_SWO_GPIO_Port GPIOB
#define I2C_01_SCL_Pin GPIO_PIN_6
#define I2C_01_SCL_GPIO_Port GPIOB
#define I2C_01_SDA_Pin GPIO_PIN_7
#define I2C_01_SDA_GPIO_Port GPIOB
#define GPIO_02_Pin GPIO_PIN_0
#define GPIO_02_GPIO_Port GPIOE
#define SDCARD_DETECT_SDIO_Pin GPIO_PIN_1
#define SDCARD_DETECT_SDIO_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
