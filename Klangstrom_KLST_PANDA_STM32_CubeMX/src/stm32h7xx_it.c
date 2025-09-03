/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32h7xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "KlangstromEnvironment.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)
#include "KlangstromConfiguration.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef   hdma_adc3;
extern ADC_HandleTypeDef   hadc3;
extern DMA_HandleTypeDef   hdma_dfsdm1_flt0;
extern DMA_HandleTypeDef   hdma_dfsdm1_flt1;
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef  hltdc;
extern OSPI_HandleTypeDef  hospi1;
extern DMA_HandleTypeDef   hdma_sai1_a;
extern DMA_HandleTypeDef   hdma_sai1_b;
extern SAI_HandleTypeDef   hsai_BlockA1;
extern SAI_HandleTypeDef   hsai_BlockB1;
extern SD_HandleTypeDef    hsd2;
extern TIM_HandleTypeDef   htim1;
extern TIM_HandleTypeDef   htim2;
extern TIM_HandleTypeDef   htim7;
extern TIM_HandleTypeDef   htim13;
extern TIM_HandleTypeDef   htim14;
extern DMA_HandleTypeDef   hdma_uart4_rx;
extern DMA_HandleTypeDef   hdma_uart4_tx;
extern DMA_HandleTypeDef   hdma_uart8_rx;
extern DMA_HandleTypeDef   hdma_uart8_tx;
extern DMA_HandleTypeDef   hdma_uart9_rx;
extern DMA_HandleTypeDef   hdma_uart9_tx;
extern UART_HandleTypeDef  huart4;
extern UART_HandleTypeDef  huart8;
extern UART_HandleTypeDef  huart9;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */
    // NOTE this only works if `PendSV_Handler` is locate above `SysTick_Handler`
#if !defined(ARDUINO_KLST_PANDA) && !defined(ARDUINO_KLST_CATERPILLAR)
    /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */
#endif // !defined(ARDUINO_KLST_PANDA)
       /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

    /* USER CODE END DMA1_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_sai1_a);
    /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

    /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

    /* USER CODE END DMA1_Stream1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_sai1_b);
    /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

    /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

    /* USER CODE END DMA1_Stream2_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_uart8_rx);
    /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

    /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

    /* USER CODE END DMA1_Stream3_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_uart8_tx);
    /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

    /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

    /* USER CODE END DMA1_Stream4_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_uart9_rx);
    /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

    /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

    /* USER CODE END DMA1_Stream5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_uart9_tx);
    /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

    /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */

    /* USER CODE END DMA1_Stream6_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_uart4_rx);
    /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

    /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief This function handles TIM1 break interrupt.
  */
void TIM1_BRK_IRQHandler(void) {
    /* USER CODE BEGIN TIM1_BRK_IRQn 0 */

    /* USER CODE END TIM1_BRK_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_BRK_IRQn 1 */

    /* USER CODE END TIM1_BRK_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void) {
    /* USER CODE BEGIN TIM1_UP_IRQn 0 */

    /* USER CODE END TIM1_UP_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_UP_IRQn 1 */

    /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
  * @brief This function handles TIM1 trigger and commutation interrupts.
  */
void TIM1_TRG_COM_IRQHandler(void) {
    /* USER CODE BEGIN TIM1_TRG_COM_IRQn 0 */

    /* USER CODE END TIM1_TRG_COM_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_TRG_COM_IRQn 1 */

    /* USER CODE END TIM1_TRG_COM_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void) {
    /* USER CODE BEGIN TIM1_CC_IRQn 0 */

    /* USER CODE END TIM1_CC_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_CC_IRQn 1 */

    /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void) {
    /* USER CODE BEGIN TIM2_IRQn 0 */

    /* USER CODE END TIM2_IRQn 0 */
    HAL_TIM_IRQHandler(&htim2);
    /* USER CODE BEGIN TIM2_IRQn 1 */

    /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void) {
    /* USER CODE BEGIN EXTI15_10_IRQn 0 */
#ifndef KLST_ENABLE_FIRE_EVENT_FOR_ALL_KEYS
    /* USER CODE END EXTI15_10_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(_DISPLAY_TOUCH_INTERRUPT_Pin);
    HAL_GPIO_EXTI_IRQHandler(_MECH_BUTTON_00_Pin);
    HAL_GPIO_EXTI_IRQHandler(_MECH_BUTTON_01_Pin);
    /* USER CODE BEGIN EXTI15_10_IRQn 1 */
#else
    for (uint16_t i = 0; i < 16; i++) {
        HAL_GPIO_EXTI_IRQHandler((uint16_t) (1 << i));
    }
#endif // KLST_ENABLE_FIRE_EVENT_FOR_ALL_KEYS
       /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void) {
    /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */

    /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
    HAL_TIM_IRQHandler(&htim13);
    /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

    /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}

/**
  * @brief This function handles TIM8 trigger and commutation interrupts and TIM14 global interrupt.
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void) {
    /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 0 */

    /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 0 */
    HAL_TIM_IRQHandler(&htim14);
    /* USER CODE BEGIN TIM8_TRG_COM_TIM14_IRQn 1 */

    /* USER CODE END TIM8_TRG_COM_TIM14_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream7 global interrupt.
  */
void DMA1_Stream7_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream7_IRQn 0 */

    /* USER CODE END DMA1_Stream7_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_uart4_tx);
    /* USER CODE BEGIN DMA1_Stream7_IRQn 1 */

    /* USER CODE END DMA1_Stream7_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void) {
    /* USER CODE BEGIN UART4_IRQn 0 */

    /* USER CODE END UART4_IRQn 0 */
    HAL_UART_IRQHandler(&huart4);
    /* USER CODE BEGIN UART4_IRQn 1 */

    /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void) {
    /* USER CODE BEGIN TIM7_IRQn 0 */

    /* USER CODE END TIM7_IRQn 0 */
    HAL_TIM_IRQHandler(&htim7);
    /* USER CODE BEGIN TIM7_IRQn 1 */

    /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
    // #if !defined(ARDUINO_KLST_PANDA) && !defined(ARDUINO_KLST_CATERPILLAR)
    /* USER CODE END DMA2_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_dfsdm1_flt0);
    /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */
    // #endif // !defined(ARDUINO_KLST_PANDA)
    /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */
    // #if !defined(ARDUINO_KLST_PANDA) && !defined(ARDUINO_KLST_CATERPILLAR)
    /* USER CODE END DMA2_Stream1_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_dfsdm1_flt1);
    /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */
    // #endif // !defined(ARDUINO_KLST_PANDA)
    /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles UART8 global interrupt.
  */
void UART8_IRQHandler(void) {
    /* USER CODE BEGIN UART8_IRQn 0 */

    /* USER CODE END UART8_IRQn 0 */
    HAL_UART_IRQHandler(&huart8);
    /* USER CODE BEGIN UART8_IRQn 1 */

    /* USER CODE END UART8_IRQn 1 */
}

/**
  * @brief This function handles SAI1 global interrupt.
  */
void SAI1_IRQHandler(void) {
    /* USER CODE BEGIN SAI1_IRQn 0 */

    /* USER CODE END SAI1_IRQn 0 */
    HAL_SAI_IRQHandler(&hsai_BlockA1);
    HAL_SAI_IRQHandler(&hsai_BlockB1);
    /* USER CODE BEGIN SAI1_IRQn 1 */

    /* USER CODE END SAI1_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void) {
    /* USER CODE BEGIN LTDC_IRQn 0 */
#if defined(KLST_PANDA_STM32)
    /* USER CODE END LTDC_IRQn 0 */
    HAL_LTDC_IRQHandler(&hltdc);
    /* USER CODE BEGIN LTDC_IRQn 1 */
#endif // ARDUINO_KLST_CATERPILLAR
       /* USER CODE END LTDC_IRQn 1 */
}

/**
  * @brief This function handles LTDC Error global Interrupt.
  */
void LTDC_ER_IRQHandler(void) {
    /* USER CODE BEGIN LTDC_ER_IRQn 0 */
#if defined(KLST_PANDA_STM32)
    /* USER CODE END LTDC_ER_IRQn 0 */
    HAL_LTDC_IRQHandler(&hltdc);
    /* USER CODE BEGIN LTDC_ER_IRQn 1 */
#endif // ARDUINO_KLST_CATERPILLAR
       /* USER CODE END LTDC_ER_IRQn 1 */
}

/**
  * @brief This function handles DMA2D global interrupt.
  */
void DMA2D_IRQHandler(void) {
    /* USER CODE BEGIN DMA2D_IRQn 0 */
#if defined(KLST_PANDA_STM32)
    /* USER CODE END DMA2D_IRQn 0 */
    HAL_DMA2D_IRQHandler(&hdma2d);
    /* USER CODE BEGIN DMA2D_IRQn 1 */
#endif // ARDUINO_KLST_CATERPILLAR
       /* USER CODE END DMA2D_IRQn 1 */
}

/**
  * @brief This function handles OCTOSPI1 global interrupt.
  */
void OCTOSPI1_IRQHandler(void) {
    /* USER CODE BEGIN OCTOSPI1_IRQn 0 */
#if defined(KLST_PANDA_STM32)
    /* USER CODE END OCTOSPI1_IRQn 0 */
    HAL_OSPI_IRQHandler(&hospi1);
    /* USER CODE BEGIN OCTOSPI1_IRQn 1 */
#endif // ARDUINO_KLST_CATERPILLAR
       /* USER CODE END OCTOSPI1_IRQn 1 */
}

/**
  * @brief This function handles SDMMC2 global interrupt.
  */
void SDMMC2_IRQHandler(void) {
    /* USER CODE BEGIN SDMMC2_IRQn 0 */

    /* USER CODE END SDMMC2_IRQn 0 */
    HAL_SD_IRQHandler(&hsd2);
    /* USER CODE BEGIN SDMMC2_IRQn 1 */

    /* USER CODE END SDMMC2_IRQn 1 */
}

/**
  * @brief This function handles ADC3 global interrupt.
  */
void ADC3_IRQHandler(void) {
    /* USER CODE BEGIN ADC3_IRQn 0 */

    /* USER CODE END ADC3_IRQn 0 */
    HAL_ADC_IRQHandler(&hadc3);
    /* USER CODE BEGIN ADC3_IRQn 1 */

    /* USER CODE END ADC3_IRQn 1 */
}

/**
  * @brief This function handles BDMA channel0 global interrupt.
  */
void BDMA_Channel0_IRQHandler(void) {
    /* USER CODE BEGIN BDMA_Channel0_IRQn 0 */

    /* USER CODE END BDMA_Channel0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_adc3);
    /* USER CODE BEGIN BDMA_Channel0_IRQn 1 */

    /* USER CODE END BDMA_Channel0_IRQn 1 */
}

/**
  * @brief This function handles UART9 global interrupt.
  */
void UART9_IRQHandler(void) {
    /* USER CODE BEGIN UART9_IRQn 0 */

    /* USER CODE END UART9_IRQn 0 */
    HAL_UART_IRQHandler(&huart9);
    /* USER CODE BEGIN UART9_IRQn 1 */

    /* USER CODE END UART9_IRQn 1 */
}

/* USER CODE BEGIN 1 */
#endif // defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)
/* USER CODE END 1 */
