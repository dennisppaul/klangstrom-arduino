/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    octospi.c
 * @brief   This file provides code for the configuration
 *          of the OCTOSPI instances.
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
#include "octospi.h"

/* USER CODE BEGIN 0 */
#include "KlangstromEnvironment.h"
#if defined(KLST_PANDA_STM32)
/* USER CODE END 0 */

OSPI_HandleTypeDef hospi1;

/* OCTOSPI1 init function */
void MX_OCTOSPI1_Init(void) {

    /* USER CODE BEGIN OCTOSPI1_Init 0 */

    /* USER CODE END OCTOSPI1_Init 0 */

    OSPIM_CfgTypeDef        sOspiManagerCfg = {0};
    OSPI_HyperbusCfgTypeDef sHyperBusCfg    = {0};

    /* USER CODE BEGIN OCTOSPI1_Init 1 */

    /* USER CODE END OCTOSPI1_Init 1 */
    hospi1.Instance                   = OCTOSPI1;
    hospi1.Init.FifoThreshold         = 4;
    hospi1.Init.DualQuad              = HAL_OSPI_DUALQUAD_DISABLE;
    hospi1.Init.MemoryType            = HAL_OSPI_MEMTYPE_HYPERBUS;
    hospi1.Init.DeviceSize            = 24;
    hospi1.Init.ChipSelectHighTime    = 2;
    hospi1.Init.FreeRunningClock      = HAL_OSPI_FREERUNCLK_DISABLE;
    hospi1.Init.ClockMode             = HAL_OSPI_CLOCK_MODE_0;
    hospi1.Init.WrapSize              = HAL_OSPI_WRAP_NOT_SUPPORTED;
    hospi1.Init.ClockPrescaler        = 2;
    hospi1.Init.SampleShifting        = HAL_OSPI_SAMPLE_SHIFTING_NONE;
    hospi1.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_ENABLE;
    hospi1.Init.ChipSelectBoundary    = 0;
    hospi1.Init.DelayBlockBypass      = HAL_OSPI_DELAY_BLOCK_USED;
    hospi1.Init.MaxTran               = 23;
    hospi1.Init.Refresh               = 799;
    if (HAL_OSPI_Init(&hospi1) != HAL_OK) {
        Error_Handler();
    }
    sOspiManagerCfg.ClkPort    = 1;
    sOspiManagerCfg.DQSPort    = 1;
    sOspiManagerCfg.NCSPort    = 1;
    sOspiManagerCfg.IOLowPort  = HAL_OSPIM_IOPORT_1_LOW;
    sOspiManagerCfg.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
    if (HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        Error_Handler();
    }
    sHyperBusCfg.RWRecoveryTime   = 4;
    sHyperBusCfg.AccessTime       = 7;
    sHyperBusCfg.WriteZeroLatency = HAL_OSPI_LATENCY_ON_WRITE;
    sHyperBusCfg.LatencyMode      = HAL_OSPI_FIXED_LATENCY;
    if (HAL_OSPI_HyperbusCfg(&hospi1, &sHyperBusCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN OCTOSPI1_Init 2 */

    /* USER CODE END OCTOSPI1_Init 2 */
}

void HAL_OSPI_MspInit(OSPI_HandleTypeDef* ospiHandle) {

    GPIO_InitTypeDef         GPIO_InitStruct     = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (ospiHandle->Instance == OCTOSPI1) {
        /* USER CODE BEGIN OCTOSPI1_MspInit 0 */

        /* USER CODE END OCTOSPI1_MspInit 0 */

        /** Initializes the peripherals clock
  */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_OSPI;
        PeriphClkInitStruct.PLL2.PLL2M           = 2;
        PeriphClkInitStruct.PLL2.PLL2N           = 75;
        PeriphClkInitStruct.PLL2.PLL2P           = 48;
        PeriphClkInitStruct.PLL2.PLL2Q           = 2;
        PeriphClkInitStruct.PLL2.PLL2R           = 2;
        PeriphClkInitStruct.PLL2.PLL2RGE         = RCC_PLL2VCIRANGE_3;
        PeriphClkInitStruct.PLL2.PLL2VCOSEL      = RCC_PLL2VCOWIDE;
        PeriphClkInitStruct.PLL2.PLL2FRACN       = 0;
        PeriphClkInitStruct.OspiClockSelection   = RCC_OSPICLKSOURCE_PLL2;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        /* OCTOSPI1 clock enable */
        __HAL_RCC_OCTOSPIM_CLK_ENABLE();
        __HAL_RCC_OSPI1_CLK_ENABLE();

        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        /**OCTOSPI1 GPIO Configuration
    PF6     ------> OCTOSPIM_P1_IO3
    PF10     ------> OCTOSPIM_P1_CLK
    PC5     ------> OCTOSPIM_P1_DQS
    PE7     ------> OCTOSPIM_P1_IO4
    PE8     ------> OCTOSPIM_P1_IO5
    PE9     ------> OCTOSPIM_P1_IO6
    PE10     ------> OCTOSPIM_P1_IO7
    PB12     ------> OCTOSPIM_P1_IO0
    PB13     ------> OCTOSPIM_P1_IO2
    PD12     ------> OCTOSPIM_P1_IO1
    PG6     ------> OCTOSPIM_P1_NCS
    */
        GPIO_InitStruct.Pin       = _EXT_MEMORY_A3_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
        HAL_GPIO_Init(_EXT_MEMORY_A3_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _EXT_MEMORY_CLK_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
        HAL_GPIO_Init(_EXT_MEMORY_CLK_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _EXT_MEMORY_DQS_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
        HAL_GPIO_Init(_EXT_MEMORY_DQS_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _EXT_MEMORY_A4_Pin | _EXT_MEMORY_A5_Pin | _EXT_MEMORY_A6_Pin | _EXT_MEMORY_A7_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _EXT_MEMORY_A0_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_OCTOSPIM_P1;
        HAL_GPIO_Init(_EXT_MEMORY_A0_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _EXT_MEMORY_A2_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_OCTOSPIM_P1;
        HAL_GPIO_Init(_EXT_MEMORY_A2_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _EXT_MEMORY_A1_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
        HAL_GPIO_Init(_EXT_MEMORY_A1_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _EXT_MEMORY_CHIP_SELECT_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OCTOSPIM_P1;
        HAL_GPIO_Init(_EXT_MEMORY_CHIP_SELECT_GPIO_Port, &GPIO_InitStruct);

        /* OCTOSPI1 interrupt Init */
        HAL_NVIC_SetPriority(OCTOSPI1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(OCTOSPI1_IRQn);
        /* USER CODE BEGIN OCTOSPI1_MspInit 1 */

        /* USER CODE END OCTOSPI1_MspInit 1 */
    }
}

void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef* ospiHandle) {

    if (ospiHandle->Instance == OCTOSPI1) {
        /* USER CODE BEGIN OCTOSPI1_MspDeInit 0 */

        /* USER CODE END OCTOSPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_OCTOSPIM_CLK_DISABLE();
        __HAL_RCC_OSPI1_CLK_DISABLE();

        /**OCTOSPI1 GPIO Configuration
    PF6     ------> OCTOSPIM_P1_IO3
    PF10     ------> OCTOSPIM_P1_CLK
    PC5     ------> OCTOSPIM_P1_DQS
    PE7     ------> OCTOSPIM_P1_IO4
    PE8     ------> OCTOSPIM_P1_IO5
    PE9     ------> OCTOSPIM_P1_IO6
    PE10     ------> OCTOSPIM_P1_IO7
    PB12     ------> OCTOSPIM_P1_IO0
    PB13     ------> OCTOSPIM_P1_IO2
    PD12     ------> OCTOSPIM_P1_IO1
    PG6     ------> OCTOSPIM_P1_NCS
    */
        HAL_GPIO_DeInit(GPIOF, _EXT_MEMORY_A3_Pin | _EXT_MEMORY_CLK_Pin);

        HAL_GPIO_DeInit(_EXT_MEMORY_DQS_GPIO_Port, _EXT_MEMORY_DQS_Pin);

        HAL_GPIO_DeInit(GPIOE, _EXT_MEMORY_A4_Pin | _EXT_MEMORY_A5_Pin | _EXT_MEMORY_A6_Pin | _EXT_MEMORY_A7_Pin);

        HAL_GPIO_DeInit(GPIOB, _EXT_MEMORY_A0_Pin | _EXT_MEMORY_A2_Pin);

        HAL_GPIO_DeInit(_EXT_MEMORY_A1_GPIO_Port, _EXT_MEMORY_A1_Pin);

        HAL_GPIO_DeInit(_EXT_MEMORY_CHIP_SELECT_GPIO_Port, _EXT_MEMORY_CHIP_SELECT_Pin);

        /* OCTOSPI1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(OCTOSPI1_IRQn);
        /* USER CODE BEGIN OCTOSPI1_MspDeInit 1 */

        /* USER CODE END OCTOSPI1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
#endif // defined(ARDUINO_KLST_PANDA)
/* USER CODE END 1 */
