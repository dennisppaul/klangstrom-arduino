/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "KlangstromEnvironment.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)
/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart8;
UART_HandleTypeDef huart9;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef  hdma_uart4_rx;
DMA_HandleTypeDef  hdma_uart4_tx;
DMA_HandleTypeDef  hdma_uart8_rx;
DMA_HandleTypeDef  hdma_uart8_tx;
DMA_HandleTypeDef  hdma_uart9_rx;
DMA_HandleTypeDef  hdma_uart9_tx;

/* UART4 init function */
void MX_UART4_Init(void) {

    /* USER CODE BEGIN UART4_Init 0 */

    /* USER CODE END UART4_Init 0 */

    /* USER CODE BEGIN UART4_Init 1 */

    /* USER CODE END UART4_Init 1 */
    huart4.Instance                    = UART4;
    huart4.Init.BaudRate               = 31250;
    huart4.Init.WordLength             = UART_WORDLENGTH_8B;
    huart4.Init.StopBits               = UART_STOPBITS_1;
    huart4.Init.Parity                 = UART_PARITY_NONE;
    huart4.Init.Mode                   = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart4.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart4.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart4) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN UART4_Init 2 */

    /* USER CODE END UART4_Init 2 */
}
/* UART8 init function */
void MX_UART8_Init(void) {

    /* USER CODE BEGIN UART8_Init 0 */

    /* USER CODE END UART8_Init 0 */

    /* USER CODE BEGIN UART8_Init 1 */

    /* USER CODE END UART8_Init 1 */
    huart8.Instance                    = UART8;
    huart8.Init.BaudRate               = 115200;
    huart8.Init.WordLength             = UART_WORDLENGTH_8B;
    huart8.Init.StopBits               = UART_STOPBITS_1;
    huart8.Init.Parity                 = UART_PARITY_NONE;
    huart8.Init.Mode                   = UART_MODE_TX_RX;
    huart8.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart8.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart8.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart8.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN UART8_Init 2 */

    /* USER CODE END UART8_Init 2 */
}
/* UART9 init function */
void MX_UART9_Init(void) {

    /* USER CODE BEGIN UART9_Init 0 */

    /* USER CODE END UART9_Init 0 */

    /* USER CODE BEGIN UART9_Init 1 */

    /* USER CODE END UART9_Init 1 */
    huart9.Instance                    = UART9;
    huart9.Init.BaudRate               = 115200;
    huart9.Init.WordLength             = UART_WORDLENGTH_8B;
    huart9.Init.StopBits               = UART_STOPBITS_1;
    huart9.Init.Parity                 = UART_PARITY_NONE;
    huart9.Init.Mode                   = UART_MODE_TX_RX;
    huart9.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart9.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart9.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart9.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart9.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart9) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart9, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart9, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart9) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN UART9_Init 2 */

    /* USER CODE END UART9_Init 2 */
}
/* USART2 init function */

void MX_USART2_UART_Init(void) {

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance                    = USART2;
    huart2.Init.BaudRate               = 115200;
    huart2.Init.WordLength             = UART_WORDLENGTH_8B;
    huart2.Init.StopBits               = UART_STOPBITS_1;
    huart2.Init.Parity                 = UART_PARITY_NONE;
    huart2.Init.Mode                   = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */
}
/* USART3 init function */

void MX_USART3_UART_Init(void) {

    /* USER CODE BEGIN USART3_Init 0 */

    /* USER CODE END USART3_Init 0 */

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    huart3.Instance                    = USART3;
    huart3.Init.BaudRate               = 115200;
    huart3.Init.WordLength             = UART_WORDLENGTH_8B;
    huart3.Init.StopBits               = UART_STOPBITS_1;
    huart3.Init.Parity                 = UART_PARITY_NONE;
    huart3.Init.Mode                   = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle) {

    GPIO_InitTypeDef         GPIO_InitStruct     = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (uartHandle->Instance == UART4) {
        /* USER CODE BEGIN UART4_MspInit 0 */

        /* USER CODE END UART4_MspInit 0 */

        /** Initializes the peripherals clock
  */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_UART4;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        /* UART4 clock enable */
        __HAL_RCC_UART4_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**UART4 GPIO Configuration
    PC11     ------> UART4_RX
    PD1     ------> UART4_TX
    */
        GPIO_InitStruct.Pin       = _MIDI_ANALOG_IN_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(_MIDI_ANALOG_IN_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = _MIDI_ANALOG_OUT_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(_MIDI_ANALOG_OUT_GPIO_Port, &GPIO_InitStruct);

        /* UART4 DMA Init */
        /* UART4_RX Init */
        hdma_uart4_rx.Instance                 = DMA1_Stream6;
        hdma_uart4_rx.Init.Request             = DMA_REQUEST_UART4_RX;
        hdma_uart4_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_uart4_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart4_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart4_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart4_rx.Init.Mode                = DMA_NORMAL;
        hdma_uart4_rx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart4_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmarx, hdma_uart4_rx);

        /* UART4_TX Init */
        hdma_uart4_tx.Instance                 = DMA1_Stream7;
        hdma_uart4_tx.Init.Request             = DMA_REQUEST_UART4_TX;
        hdma_uart4_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_uart4_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart4_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart4_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart4_tx.Init.Mode                = DMA_NORMAL;
        hdma_uart4_tx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart4_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmatx, hdma_uart4_tx);

        /* UART4 interrupt Init */
        HAL_NVIC_SetPriority(UART4_IRQn, 13, 0);
        HAL_NVIC_EnableIRQ(UART4_IRQn);
        /* USER CODE BEGIN UART4_MspInit 1 */

        /* USER CODE END UART4_MspInit 1 */
    } else if (uartHandle->Instance == UART8) {
        /* USER CODE BEGIN UART8_MspInit 0 */

        /* USER CODE END UART8_MspInit 0 */

        /** Initializes the peripherals clock
  */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_UART8;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        /* UART8 clock enable */
        __HAL_RCC_UART8_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**UART8 GPIO Configuration
    PE0     ------> UART8_RX
    PE1     ------> UART8_TX
    */
        GPIO_InitStruct.Pin       = _IDC_SERIAL_00_RX_Pin | _IDC_SERIAL_00_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART8;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* UART8 DMA Init */
        /* UART8_RX Init */
        hdma_uart8_rx.Instance                 = DMA1_Stream2;
        hdma_uart8_rx.Init.Request             = DMA_REQUEST_UART8_RX;
        hdma_uart8_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_uart8_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart8_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart8_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart8_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart8_rx.Init.Mode                = DMA_NORMAL;
        hdma_uart8_rx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart8_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart8_rx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmarx, hdma_uart8_rx);

        /* UART8_TX Init */
        hdma_uart8_tx.Instance                 = DMA1_Stream3;
        hdma_uart8_tx.Init.Request             = DMA_REQUEST_UART8_TX;
        hdma_uart8_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_uart8_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart8_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart8_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart8_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart8_tx.Init.Mode                = DMA_NORMAL;
        hdma_uart8_tx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart8_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart8_tx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmatx, hdma_uart8_tx);

        /* UART8 interrupt Init */
        HAL_NVIC_SetPriority(UART8_IRQn, 13, 0);
        HAL_NVIC_EnableIRQ(UART8_IRQn);
        /* USER CODE BEGIN UART8_MspInit 1 */

        /* USER CODE END UART8_MspInit 1 */
    } else if (uartHandle->Instance == UART9) {
        /* USER CODE BEGIN UART9_MspInit 0 */

        /* USER CODE END UART9_MspInit 0 */

        /** Initializes the peripherals clock
  */
        PeriphClkInitStruct.PeriphClockSelection  = RCC_PERIPHCLK_UART9;
        PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16910CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        /* UART9 clock enable */
        __HAL_RCC_UART9_CLK_ENABLE();

        __HAL_RCC_GPIOG_CLK_ENABLE();
        /**UART9 GPIO Configuration
    PG0     ------> UART9_RX
    PG1     ------> UART9_TX
    */
        GPIO_InitStruct.Pin       = _IDC_SERIAL_01_RX_Pin | _IDC_SERIAL_01_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF11_UART9;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        /* UART9 DMA Init */
        /* UART9_RX Init */
        hdma_uart9_rx.Instance                 = DMA1_Stream4;
        hdma_uart9_rx.Init.Request             = DMA_REQUEST_UART9_RX;
        hdma_uart9_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_uart9_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart9_rx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart9_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart9_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart9_rx.Init.Mode                = DMA_NORMAL;
        hdma_uart9_rx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart9_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart9_rx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmarx, hdma_uart9_rx);

        /* UART9_TX Init */
        hdma_uart9_tx.Instance                 = DMA1_Stream5;
        hdma_uart9_tx.Init.Request             = DMA_REQUEST_UART9_TX;
        hdma_uart9_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_uart9_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_uart9_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_uart9_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_uart9_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_uart9_tx.Init.Mode                = DMA_NORMAL;
        hdma_uart9_tx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_uart9_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_uart9_tx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(uartHandle, hdmatx, hdma_uart9_tx);

        /* UART9 interrupt Init */
        HAL_NVIC_SetPriority(UART9_IRQn, 13, 0);
        HAL_NVIC_EnableIRQ(UART9_IRQn);
        /* USER CODE BEGIN UART9_MspInit 1 */

        /* USER CODE END UART9_MspInit 1 */
    } else if (uartHandle->Instance == USART2) {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */

        /** Initializes the peripherals clock
  */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_USART2;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**USART2 GPIO Configuration
    PA3     ------> USART2_RX
    PD5     ------> USART2_TX
    */
        GPIO_InitStruct.Pin       = SERIAL_USR_RX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(SERIAL_USR_RX_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = SERIAL_USR_TX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(SERIAL_USR_TX_GPIO_Port, &GPIO_InitStruct);

        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    } else if (uartHandle->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */

        /** Initializes the peripherals clock
  */
        PeriphClkInitStruct.PeriphClockSelection      = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        /* USART3 clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
        GPIO_InitStruct.Pin       = _SERIAL_DEBUG_TX_Pin | _SERIAL_DEBUG_RX_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle) {

    if (uartHandle->Instance == UART4) {
        /* USER CODE BEGIN UART4_MspDeInit 0 */

        /* USER CODE END UART4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART4_CLK_DISABLE();

        /**UART4 GPIO Configuration
    PC11     ------> UART4_RX
    PD1     ------> UART4_TX
    */
        HAL_GPIO_DeInit(_MIDI_ANALOG_IN_GPIO_Port, _MIDI_ANALOG_IN_Pin);

        HAL_GPIO_DeInit(_MIDI_ANALOG_OUT_GPIO_Port, _MIDI_ANALOG_OUT_Pin);

        /* UART4 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);

        /* UART4 interrupt Deinit */
        HAL_NVIC_DisableIRQ(UART4_IRQn);
        /* USER CODE BEGIN UART4_MspDeInit 1 */

        /* USER CODE END UART4_MspDeInit 1 */
    } else if (uartHandle->Instance == UART8) {
        /* USER CODE BEGIN UART8_MspDeInit 0 */

        /* USER CODE END UART8_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART8_CLK_DISABLE();

        /**UART8 GPIO Configuration
    PE0     ------> UART8_RX
    PE1     ------> UART8_TX
    */
        HAL_GPIO_DeInit(GPIOE, _IDC_SERIAL_00_RX_Pin | _IDC_SERIAL_00_TX_Pin);

        /* UART8 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);

        /* UART8 interrupt Deinit */
        HAL_NVIC_DisableIRQ(UART8_IRQn);
        /* USER CODE BEGIN UART8_MspDeInit 1 */

        /* USER CODE END UART8_MspDeInit 1 */
    } else if (uartHandle->Instance == UART9) {
        /* USER CODE BEGIN UART9_MspDeInit 0 */

        /* USER CODE END UART9_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART9_CLK_DISABLE();

        /**UART9 GPIO Configuration
    PG0     ------> UART9_RX
    PG1     ------> UART9_TX
    */
        HAL_GPIO_DeInit(GPIOG, _IDC_SERIAL_01_RX_Pin | _IDC_SERIAL_01_TX_Pin);

        /* UART9 DMA DeInit */
        HAL_DMA_DeInit(uartHandle->hdmarx);
        HAL_DMA_DeInit(uartHandle->hdmatx);

        /* UART9 interrupt Deinit */
        HAL_NVIC_DisableIRQ(UART9_IRQn);
        /* USER CODE BEGIN UART9_MspDeInit 1 */

        /* USER CODE END UART9_MspDeInit 1 */
    } else if (uartHandle->Instance == USART2) {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
    PA3     ------> USART2_RX
    PD5     ------> USART2_TX
    */
        HAL_GPIO_DeInit(SERIAL_USR_RX_GPIO_Port, SERIAL_USR_RX_Pin);

        HAL_GPIO_DeInit(SERIAL_USR_TX_GPIO_Port, SERIAL_USR_TX_Pin);

        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    } else if (uartHandle->Instance == USART3) {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
        HAL_GPIO_DeInit(GPIOD, _SERIAL_DEBUG_TX_Pin | _SERIAL_DEBUG_RX_Pin);

        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
#endif // defined(ARDUINO_KLST_PANDA)
/* USER CODE END 1 */
