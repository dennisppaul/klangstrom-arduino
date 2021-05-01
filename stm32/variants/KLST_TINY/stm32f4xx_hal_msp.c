/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */
void Error_Handler() {}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
// void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};
//   if(hadc->Instance==ADC1)
//   {
//   /* USER CODE BEGIN ADC1_MspInit 0 */
//
//   /* USER CODE END ADC1_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_ADC1_CLK_ENABLE();
//
//     __HAL_RCC_GPIOB_CLK_ENABLE();
//     /**ADC1 GPIO Configuration
//     PB0     ------> ADC1_IN8
//     PB1     ------> ADC1_IN9
//     */
//     GPIO_InitStruct.Pin = ADC_00_Pin|ADC_01_Pin;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//   /* USER CODE BEGIN ADC1_MspInit 1 */
//
//   /* USER CODE END ADC1_MspInit 1 */
//   }
//
// }

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
// void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
// {
//   if(hadc->Instance==ADC1)
//   {
//   /* USER CODE BEGIN ADC1_MspDeInit 0 */
//
//   /* USER CODE END ADC1_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_ADC1_CLK_DISABLE();
//
//     /**ADC1 GPIO Configuration
//     PB0     ------> ADC1_IN8
//     PB1     ------> ADC1_IN9
//     */
//     HAL_GPIO_DeInit(GPIOB, ADC_00_Pin|ADC_01_Pin);
//
//   /* USER CODE BEGIN ADC1_MspDeInit 1 */
//
//   /* USER CODE END ADC1_MspDeInit 1 */
//   }
//
// }

/**
* @brief DAC MSP Initialization
* This function configures the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
// void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};
//   if(hdac->Instance==DAC)
//   {
//   /* USER CODE BEGIN DAC_MspInit 0 */
//
//   /* USER CODE END DAC_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_DAC_CLK_ENABLE();
//
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     /**DAC GPIO Configuration
//     PA4     ------> DAC_OUT1
//     PA5     ------> DAC_OUT2
//     */
//     GPIO_InitStruct.Pin = DAC_00_Pin|DAC_01_Pin;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//   /* USER CODE BEGIN DAC_MspInit 1 */
//
//   /* USER CODE END DAC_MspInit 1 */
//   }
//
// }

/**
* @brief DAC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
// void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
// {
//   if(hdac->Instance==DAC)
//   {
//   /* USER CODE BEGIN DAC_MspDeInit 0 */
//
//   /* USER CODE END DAC_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_DAC_CLK_DISABLE();
//
//     /**DAC GPIO Configuration
//     PA4     ------> DAC_OUT1
//     PA5     ------> DAC_OUT2
//     */
//     HAL_GPIO_DeInit(GPIOA, DAC_00_Pin|DAC_01_Pin);
//
//   /* USER CODE BEGIN DAC_MspDeInit 1 */
//
//   /* USER CODE END DAC_MspDeInit 1 */
//   }
//
// }

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL
    */
    GPIO_InitStruct.Pin = I2C_00_SDA_Pin|I2C_00_SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(hi2c->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspInit 0 */

  /* USER CODE END I2C3_MspInit 0 */

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL
    */
    GPIO_InitStruct.Pin = AUDIOCODEC_I2C_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(AUDIOCODEC_I2C_SDA_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AUDIOCODEC_I2C_SCL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(AUDIOCODEC_I2C_SCL_GPIO_Port, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C3_CLK_ENABLE();
  /* USER CODE BEGIN I2C3_MspInit 1 */

  /* USER CODE END I2C3_MspInit 1 */
  }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL
    */
    HAL_GPIO_DeInit(I2C_00_SDA_GPIO_Port, I2C_00_SDA_Pin);

    HAL_GPIO_DeInit(I2C_00_SCL_GPIO_Port, I2C_00_SCL_Pin);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(hi2c->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspDeInit 0 */

  /* USER CODE END I2C3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C3_CLK_DISABLE();

    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL
    */
    HAL_GPIO_DeInit(AUDIOCODEC_I2C_SDA_GPIO_Port, AUDIOCODEC_I2C_SDA_Pin);

    HAL_GPIO_DeInit(AUDIOCODEC_I2C_SCL_GPIO_Port, AUDIOCODEC_I2C_SCL_Pin);

  /* USER CODE BEGIN I2C3_MspDeInit 1 */

  /* USER CODE END I2C3_MspDeInit 1 */
  }

}

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    GPIO_InitStruct.Pin = SPI_00_SCK_Pin|SPI_00_MISO_Pin|SPI_00_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }

}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, SPI_00_SCK_Pin|SPI_00_MISO_Pin|SPI_00_MOSI_Pin);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }

}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
// void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
// {
//   if(htim_base->Instance==TIM1)
//   {
//   /* USER CODE BEGIN TIM1_MspInit 0 */
//
//   /* USER CODE END TIM1_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM1_CLK_ENABLE();
//   /* USER CODE BEGIN TIM1_MspInit 1 */
//
//   /* USER CODE END TIM1_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM4)
//   {
//   /* USER CODE BEGIN TIM4_MspInit 0 */
//
//   /* USER CODE END TIM4_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM4_CLK_ENABLE();
//   /* USER CODE BEGIN TIM4_MspInit 1 */
//
//   /* USER CODE END TIM4_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM5)
//   {
//   /* USER CODE BEGIN TIM5_MspInit 0 */
//
//   /* USER CODE END TIM5_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM5_CLK_ENABLE();
//   /* USER CODE BEGIN TIM5_MspInit 1 */
//
//   /* USER CODE END TIM5_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM6)
//   {
//   /* USER CODE BEGIN TIM6_MspInit 0 */
//
//   /* USER CODE END TIM6_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM6_CLK_ENABLE();
//   /* USER CODE BEGIN TIM6_MspInit 1 */
//
//   /* USER CODE END TIM6_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM7)
//   {
//   /* USER CODE BEGIN TIM7_MspInit 0 */
//
//   /* USER CODE END TIM7_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM7_CLK_ENABLE();
//   /* USER CODE BEGIN TIM7_MspInit 1 */
//
//   /* USER CODE END TIM7_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM9)
//   {
//   /* USER CODE BEGIN TIM9_MspInit 0 */
//
//   /* USER CODE END TIM9_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM9_CLK_ENABLE();
//   /* USER CODE BEGIN TIM9_MspInit 1 */
//
//   /* USER CODE END TIM9_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM10)
//   {
//   /* USER CODE BEGIN TIM10_MspInit 0 */
//
//   /* USER CODE END TIM10_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM10_CLK_ENABLE();
//   /* USER CODE BEGIN TIM10_MspInit 1 */
//
//   /* USER CODE END TIM10_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM11)
//   {
//   /* USER CODE BEGIN TIM11_MspInit 0 */
//
//   /* USER CODE END TIM11_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM11_CLK_ENABLE();
//   /* USER CODE BEGIN TIM11_MspInit 1 */
//
//   /* USER CODE END TIM11_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM12)
//   {
//   /* USER CODE BEGIN TIM12_MspInit 0 */
//
//   /* USER CODE END TIM12_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM12_CLK_ENABLE();
//   /* USER CODE BEGIN TIM12_MspInit 1 */
//
//   /* USER CODE END TIM12_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM13)
//   {
//   /* USER CODE BEGIN TIM13_MspInit 0 */
//
//   /* USER CODE END TIM13_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM13_CLK_ENABLE();
//   /* USER CODE BEGIN TIM13_MspInit 1 */
//
//   /* USER CODE END TIM13_MspInit 1 */
//   }
//   else if(htim_base->Instance==TIM14)
//   {
//   /* USER CODE BEGIN TIM14_MspInit 0 */
//
//   /* USER CODE END TIM14_MspInit 0 */
//     /* Peripheral clock enable */
//     __HAL_RCC_TIM14_CLK_ENABLE();
//   /* USER CODE BEGIN TIM14_MspInit 1 */
//
//   /* USER CODE END TIM14_MspInit 1 */
//   }
//
// }

/**
* @brief TIM_Encoder MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_encoder: TIM_Encoder handle pointer
* @retval None
*/
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim_encoder->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_CH1
    PA1     ------> TIM2_CH2
    */
    GPIO_InitStruct.Pin = ENCODER_02_A_Pin|ENCODER_02_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(htim_encoder->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PA7     ------> TIM3_CH2
    */
    GPIO_InitStruct.Pin = ENCODER_00_A_Pin|ENCODER_00_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
  else if(htim_encoder->Instance==TIM8)
  {
  /* USER CODE BEGIN TIM8_MspInit 0 */

  /* USER CODE END TIM8_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM8_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**TIM8 GPIO Configuration
    PC6     ------> TIM8_CH1
    PC7     ------> TIM8_CH2
    */
    GPIO_InitStruct.Pin = ENCODER_01_A_Pin|ENCODER_01_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM8_MspInit 1 */

  /* USER CODE END TIM8_MspInit 1 */
  }

}

/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
// void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
// {
//   if(htim_base->Instance==TIM1)
//   {
//   /* USER CODE BEGIN TIM1_MspDeInit 0 */
//
//   /* USER CODE END TIM1_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM1_CLK_DISABLE();
//   /* USER CODE BEGIN TIM1_MspDeInit 1 */
//
//   /* USER CODE END TIM1_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM4)
//   {
//   /* USER CODE BEGIN TIM4_MspDeInit 0 */
//
//   /* USER CODE END TIM4_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM4_CLK_DISABLE();
//   /* USER CODE BEGIN TIM4_MspDeInit 1 */
//
//   /* USER CODE END TIM4_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM5)
//   {
//   /* USER CODE BEGIN TIM5_MspDeInit 0 */
//
//   /* USER CODE END TIM5_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM5_CLK_DISABLE();
//   /* USER CODE BEGIN TIM5_MspDeInit 1 */
//
//   /* USER CODE END TIM5_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM6)
//   {
//   /* USER CODE BEGIN TIM6_MspDeInit 0 */
//
//   /* USER CODE END TIM6_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM6_CLK_DISABLE();
//   /* USER CODE BEGIN TIM6_MspDeInit 1 */
//
//   /* USER CODE END TIM6_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM7)
//   {
//   /* USER CODE BEGIN TIM7_MspDeInit 0 */
//
//   /* USER CODE END TIM7_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM7_CLK_DISABLE();
//   /* USER CODE BEGIN TIM7_MspDeInit 1 */
//
//   /* USER CODE END TIM7_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM9)
//   {
//   /* USER CODE BEGIN TIM9_MspDeInit 0 */
//
//   /* USER CODE END TIM9_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM9_CLK_DISABLE();
//   /* USER CODE BEGIN TIM9_MspDeInit 1 */
//
//   /* USER CODE END TIM9_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM10)
//   {
//   /* USER CODE BEGIN TIM10_MspDeInit 0 */
//
//   /* USER CODE END TIM10_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM10_CLK_DISABLE();
//   /* USER CODE BEGIN TIM10_MspDeInit 1 */
//
//   /* USER CODE END TIM10_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM11)
//   {
//   /* USER CODE BEGIN TIM11_MspDeInit 0 */
//
//   /* USER CODE END TIM11_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM11_CLK_DISABLE();
//   /* USER CODE BEGIN TIM11_MspDeInit 1 */
//
//   /* USER CODE END TIM11_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM12)
//   {
//   /* USER CODE BEGIN TIM12_MspDeInit 0 */
//
//   /* USER CODE END TIM12_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM12_CLK_DISABLE();
//   /* USER CODE BEGIN TIM12_MspDeInit 1 */
//
//   /* USER CODE END TIM12_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM13)
//   {
//   /* USER CODE BEGIN TIM13_MspDeInit 0 */
//
//   /* USER CODE END TIM13_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM13_CLK_DISABLE();
//   /* USER CODE BEGIN TIM13_MspDeInit 1 */
//
//   /* USER CODE END TIM13_MspDeInit 1 */
//   }
//   else if(htim_base->Instance==TIM14)
//   {
//   /* USER CODE BEGIN TIM14_MspDeInit 0 */
//
//   /* USER CODE END TIM14_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_TIM14_CLK_DISABLE();
//   /* USER CODE BEGIN TIM14_MspDeInit 1 */
//
//   /* USER CODE END TIM14_MspDeInit 1 */
//   }
//
// }

/**
* @brief TIM_Encoder MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_encoder: TIM_Encoder handle pointer
* @retval None
*/
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_encoder)
{
  if(htim_encoder->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /**TIM2 GPIO Configuration
    PA0-WKUP     ------> TIM2_CH1
    PA1     ------> TIM2_CH2
    */
    HAL_GPIO_DeInit(GPIOA, ENCODER_02_A_Pin|ENCODER_02_B_Pin);

  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(htim_encoder->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PA7     ------> TIM3_CH2
    */
    HAL_GPIO_DeInit(GPIOA, ENCODER_00_A_Pin|ENCODER_00_B_Pin);

  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
  else if(htim_encoder->Instance==TIM8)
  {
  /* USER CODE BEGIN TIM8_MspDeInit 0 */

  /* USER CODE END TIM8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM8_CLK_DISABLE();

    /**TIM8 GPIO Configuration
    PC6     ------> TIM8_CH1
    PC7     ------> TIM8_CH2
    */
    HAL_GPIO_DeInit(GPIOC, ENCODER_01_A_Pin|ENCODER_01_B_Pin);

  /* USER CODE BEGIN TIM8_MspDeInit 1 */

  /* USER CODE END TIM8_MspDeInit 1 */
  }

}

extern DMA_HandleTypeDef hdma_sai1_b;

extern DMA_HandleTypeDef hdma_sai1_a;

static uint32_t SAI1_client =0;

void HAL_SAI_MspInit(SAI_HandleTypeDef* hsai)
{

  GPIO_InitTypeDef GPIO_InitStruct;
/* SAI1 */
    if(hsai->Instance==SAI1_Block_A)
    {
    /* Peripheral clock enable */
    if (SAI1_client == 0)
    {
       __HAL_RCC_SAI1_CLK_ENABLE();
    }
    SAI1_client ++;

    /**SAI1_A_Block_A GPIO Configuration
    PC1     ------> SAI1_SD_A
    */
    GPIO_InitStruct.Pin = AUDIOCODEC_SAI_SD_A_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(AUDIOCODEC_SAI_SD_A_GPIO_Port, &GPIO_InitStruct);

    /* Peripheral DMA init*/

    hdma_sai1_a.Instance = DMA2_Stream1;
    hdma_sai1_a.Init.Channel = DMA_CHANNEL_0;
    hdma_sai1_a.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sai1_a.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sai1_a.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sai1_a.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_sai1_a.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_sai1_a.Init.Mode = DMA_CIRCULAR;
    hdma_sai1_a.Init.Priority = DMA_PRIORITY_LOW;
    hdma_sai1_a.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_sai1_a) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one stream to perform all the requested DMAs. */
    __HAL_LINKDMA(hsai,hdmarx,hdma_sai1_a);

    __HAL_LINKDMA(hsai,hdmatx,hdma_sai1_a);

    }
    if(hsai->Instance==SAI1_Block_B)
    {
      /* Peripheral clock enable */
      if (SAI1_client == 0)
      {
       __HAL_RCC_SAI1_CLK_ENABLE();
      }
    SAI1_client ++;

    /**SAI1_B_Block_B GPIO Configuration
    PC0     ------> SAI1_MCLK_B
    PB12     ------> SAI1_SCK_B
    PA9     ------> SAI1_SD_B
    PB9     ------> SAI1_FS_B
    */
    GPIO_InitStruct.Pin = AUDIOCODEC_SAI_MCLK_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(AUDIOCODEC_SAI_MCLK_B_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AUDIOCODEC_SAI_SCK_B_Pin|AUDIOCODEC_SAI_FS_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = AUDIOCODEC_SAI_SD_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
    HAL_GPIO_Init(AUDIOCODEC_SAI_SD_B_GPIO_Port, &GPIO_InitStruct);

    /* Peripheral DMA init*/

    hdma_sai1_b.Instance = DMA2_Stream4;
    hdma_sai1_b.Init.Channel = DMA_CHANNEL_1;
    hdma_sai1_b.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sai1_b.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sai1_b.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sai1_b.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_sai1_b.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_sai1_b.Init.Mode = DMA_CIRCULAR;
    hdma_sai1_b.Init.Priority = DMA_PRIORITY_LOW;
    hdma_sai1_b.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_sai1_b) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one stream to perform all the requested DMAs. */
    __HAL_LINKDMA(hsai,hdmarx,hdma_sai1_b);
    __HAL_LINKDMA(hsai,hdmatx,hdma_sai1_b);
    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef* hsai)
{
/* SAI1 */
    if(hsai->Instance==SAI1_Block_A)
    {
    SAI1_client --;
    if (SAI1_client == 0)
      {
      /* Peripheral clock disable */
       __HAL_RCC_SAI1_CLK_DISABLE();
      }

    /**SAI1_A_Block_A GPIO Configuration
    PC1     ------> SAI1_SD_A
    */
    HAL_GPIO_DeInit(AUDIOCODEC_SAI_SD_A_GPIO_Port, AUDIOCODEC_SAI_SD_A_Pin);

    HAL_DMA_DeInit(hsai->hdmarx);
    HAL_DMA_DeInit(hsai->hdmatx);
    }
    if(hsai->Instance==SAI1_Block_B)
    {
    SAI1_client --;
      if (SAI1_client == 0)
      {
      /* Peripheral clock disable */
      __HAL_RCC_SAI1_CLK_DISABLE();
      }

    /**SAI1_B_Block_B GPIO Configuration
    PC0     ------> SAI1_MCLK_B
    PB12     ------> SAI1_SCK_B
    PA9     ------> SAI1_SD_B
    PB9     ------> SAI1_FS_B
    */
    HAL_GPIO_DeInit(AUDIOCODEC_SAI_MCLK_B_GPIO_Port, AUDIOCODEC_SAI_MCLK_B_Pin);

    HAL_GPIO_DeInit(GPIOB, AUDIOCODEC_SAI_SCK_B_Pin|AUDIOCODEC_SAI_FS_B_Pin);

    HAL_GPIO_DeInit(AUDIOCODEC_SAI_SD_B_GPIO_Port, AUDIOCODEC_SAI_SD_B_Pin);

    HAL_DMA_DeInit(hsai->hdmarx);
    HAL_DMA_DeInit(hsai->hdmatx);
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
