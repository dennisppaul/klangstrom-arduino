/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    ltdc.c
 * @brief   This file provides code for the configuration
 *          of the LTDC instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"

/* USER CODE BEGIN 0 */
#include "KlangstromEnvironment.h"
#if defined(KLST_PANDA_STM32)
/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Init(void) {

    /* USER CODE BEGIN LTDC_Init 0 */

    /* USER CODE END LTDC_Init 0 */

    LTDC_LayerCfgTypeDef pLayerCfg = {0};

    /* USER CODE BEGIN LTDC_Init 1 */

    /* USER CODE END LTDC_Init 1 */
    hltdc.Instance                = LTDC;
    hltdc.Init.HSPolarity         = LTDC_HSPOLARITY_AL;
    hltdc.Init.VSPolarity         = LTDC_VSPOLARITY_AL;
    hltdc.Init.DEPolarity         = LTDC_DEPOLARITY_AL;
    hltdc.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;
    hltdc.Init.HorizontalSync     = 3;
    hltdc.Init.VerticalSync       = 3;
    hltdc.Init.AccumulatedHBP     = 46;
    hltdc.Init.AccumulatedVBP     = 15;
    hltdc.Init.AccumulatedActiveW = 526;
    hltdc.Init.AccumulatedActiveH = 287;
    hltdc.Init.TotalWidth         = 534;
    hltdc.Init.TotalHeigh         = 295;
    hltdc.Init.Backcolor.Blue     = 0;
    hltdc.Init.Backcolor.Green    = 0;
    hltdc.Init.Backcolor.Red      = 0;
    if (HAL_LTDC_Init(&hltdc) != HAL_OK) {
        Error_Handler();
    }
    pLayerCfg.WindowX0        = 0;
    pLayerCfg.WindowX1        = KLST_DISPLAY_WIDTH;
    pLayerCfg.WindowY0        = 0;
    pLayerCfg.WindowY1        = KLST_DISPLAY_HEIGHT;
    pLayerCfg.PixelFormat     = LTDC_PIXEL_FORMAT_ARGB8888;
    pLayerCfg.Alpha           = 255;
    pLayerCfg.Alpha0          = 0;
    pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    pLayerCfg.FBStartAdress   = KLST_DISPLAY_FRAMEBUFFER_ADDRESS;
    pLayerCfg.ImageWidth      = KLST_DISPLAY_WIDTH;
    pLayerCfg.ImageHeight     = KLST_DISPLAY_HEIGHT;
    pLayerCfg.Backcolor.Blue  = 0;
    pLayerCfg.Backcolor.Green = 0;
    pLayerCfg.Backcolor.Red   = 0;
    if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN LTDC_Init 2 */

    /* USER CODE END LTDC_Init 2 */
}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (ltdcHandle->Instance == LTDC) {
        /* USER CODE BEGIN LTDC_MspInit 0 */

        /* USER CODE END LTDC_MspInit 0 */
        /* LTDC clock enable */
        __HAL_RCC_LTDC_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        /**LTDC GPIO Configuration
    PC0     ------> LTDC_R5
    PC1     ------> LTDC_G5
    PA2     ------> LTDC_R1
    PA5     ------> LTDC_R4
    PA6     ------> LTDC_G2
    PA7     ------> LTDC_VSYNC
    PB0     ------> LTDC_G1
    PB1     ------> LTDC_G0
    PE12     ------> LTDC_B4
    PE13     ------> LTDC_DE
    PE15     ------> LTDC_R7
    PB10     ------> LTDC_G4
    PD10     ------> LTDC_B3
    PG7     ------> LTDC_CLK
    PG8     ------> LTDC_G7
    PC6     ------> LTDC_HSYNC
    PC7     ------> LTDC_G6
    PC9     ------> LTDC_G3
    PA15(JTDI)     ------> LTDC_R3
    PC10     ------> LTDC_R2
    PC12     ------> LTDC_R6
    PD0     ------> LTDC_B1
    PD2     ------> LTDC_B2
    PG13     ------> LTDC_R0
    PG14     ------> LTDC_B0
    PB5     ------> LTDC_B5
    PB8     ------> LTDC_B6
    PB9     ------> LTDC_B7
    */
        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_R5_Pin | _DISPLAY_LTDC_G5_Pin | _DISPLAY_LTDC_HSYNC_Pin | _DISPLAY_LTDC_G6_Pin | _DISPLAY_LTDC_R2_Pin | _DISPLAY_LTDC_R6_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_R1_Pin | _DISPLAY_LTDC_R4_Pin | _DISPLAY_LTDC_G2_Pin | _DISPLAY_LTDC_VSYNC_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_G1_Pin | _DISPLAY_LTDC_G0_Pin | _DISPLAY_LTDC_G4_Pin | _DISPLAY_LTDC_B6_Pin | _DISPLAY_LTDC_B7_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_B4_Pin | _DISPLAY_LTDC_DE_Pin | _DISPLAY_LTDC_R7_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_B3_Pin | _DISPLAY_LTDC_B1_Pin | _DISPLAY_LTDC_B2_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_CLK_Pin | _DISPLAY_LTDC_G7_Pin | _DISPLAY_LTDC_R0_Pin | _DISPLAY_LTDC_B0_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_G3_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_LTDC;
        HAL_GPIO_Init(_DISPLAY_LTDC_G3_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_R3_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
        HAL_GPIO_Init(_DISPLAY_LTDC_R3_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _DISPLAY_LTDC_B5_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF3_LTDC;
        HAL_GPIO_Init(_DISPLAY_LTDC_B5_GPIO_Port, &GPIO_InitStruct);

        /* LTDC interrupt Init */
        HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(LTDC_IRQn);
        HAL_NVIC_SetPriority(LTDC_ER_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(LTDC_ER_IRQn);
        /* USER CODE BEGIN LTDC_MspInit 1 */

        /* USER CODE END LTDC_MspInit 1 */
    }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle) {

    if (ltdcHandle->Instance == LTDC) {
        /* USER CODE BEGIN LTDC_MspDeInit 0 */

        /* USER CODE END LTDC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_LTDC_CLK_DISABLE();

        /**LTDC GPIO Configuration
    PC0     ------> LTDC_R5
    PC1     ------> LTDC_G5
    PA2     ------> LTDC_R1
    PA5     ------> LTDC_R4
    PA6     ------> LTDC_G2
    PA7     ------> LTDC_VSYNC
    PB0     ------> LTDC_G1
    PB1     ------> LTDC_G0
    PE12     ------> LTDC_B4
    PE13     ------> LTDC_DE
    PE15     ------> LTDC_R7
    PB10     ------> LTDC_G4
    PD10     ------> LTDC_B3
    PG7     ------> LTDC_CLK
    PG8     ------> LTDC_G7
    PC6     ------> LTDC_HSYNC
    PC7     ------> LTDC_G6
    PC9     ------> LTDC_G3
    PA15(JTDI)     ------> LTDC_R3
    PC10     ------> LTDC_R2
    PC12     ------> LTDC_R6
    PD0     ------> LTDC_B1
    PD2     ------> LTDC_B2
    PG13     ------> LTDC_R0
    PG14     ------> LTDC_B0
    PB5     ------> LTDC_B5
    PB8     ------> LTDC_B6
    PB9     ------> LTDC_B7
    */
        HAL_GPIO_DeInit(GPIOC, _DISPLAY_LTDC_R5_Pin | _DISPLAY_LTDC_G5_Pin | _DISPLAY_LTDC_HSYNC_Pin | _DISPLAY_LTDC_G6_Pin | _DISPLAY_LTDC_G3_Pin | _DISPLAY_LTDC_R2_Pin | _DISPLAY_LTDC_R6_Pin);

        HAL_GPIO_DeInit(GPIOA, _DISPLAY_LTDC_R1_Pin | _DISPLAY_LTDC_R4_Pin | _DISPLAY_LTDC_G2_Pin | _DISPLAY_LTDC_VSYNC_Pin | _DISPLAY_LTDC_R3_Pin);

        HAL_GPIO_DeInit(GPIOB, _DISPLAY_LTDC_G1_Pin | _DISPLAY_LTDC_G0_Pin | _DISPLAY_LTDC_G4_Pin | _DISPLAY_LTDC_B5_Pin | _DISPLAY_LTDC_B6_Pin | _DISPLAY_LTDC_B7_Pin);

        HAL_GPIO_DeInit(GPIOE, _DISPLAY_LTDC_B4_Pin | _DISPLAY_LTDC_DE_Pin | _DISPLAY_LTDC_R7_Pin);

        HAL_GPIO_DeInit(GPIOD, _DISPLAY_LTDC_B3_Pin | _DISPLAY_LTDC_B1_Pin | _DISPLAY_LTDC_B2_Pin);

        HAL_GPIO_DeInit(GPIOG, _DISPLAY_LTDC_CLK_Pin | _DISPLAY_LTDC_G7_Pin | _DISPLAY_LTDC_R0_Pin | _DISPLAY_LTDC_B0_Pin);

        /* LTDC interrupt Deinit */
        HAL_NVIC_DisableIRQ(LTDC_IRQn);
        HAL_NVIC_DisableIRQ(LTDC_ER_IRQn);
        /* USER CODE BEGIN LTDC_MspDeInit 1 */

        /* USER CODE END LTDC_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
#endif // defined(ARDUINO_KLST_PANDA)
/* USER CODE END 1 */
