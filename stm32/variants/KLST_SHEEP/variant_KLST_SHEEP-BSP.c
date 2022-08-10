/*
 *******************************************************************************
 * Copyright (c) 2020-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#if defined(KLST_BOARD_KLST_SHEEP)
#include "variant_KLST_SHEEP-BSP.h"

#include "pins_arduino.h"

/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */
/* from `main.h`+ `main.c`                                                             */
/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/* Private defines -----------------------------------------------------------*/
#define AUDIOCODEC_SAI1_MCLK_A_Pin            GPIO_PIN_2
#define AUDIOCODEC_SAI1_MCLK_A_GPIO_Port      GPIOE
#define AUDIOCODEC_SAI1_SD_B_Pin              GPIO_PIN_3
#define AUDIOCODEC_SAI1_SD_B_GPIO_Port        GPIOE
#define AUDIOCODEC_SAI1_FS_A_Pin              GPIO_PIN_4
#define AUDIOCODEC_SAI1_FS_A_GPIO_Port        GPIOE
#define AUDIOCODEC_SAI1_SCK_A_Pin             GPIO_PIN_5
#define AUDIOCODEC_SAI1_SCK_A_GPIO_Port       GPIOE
#define AUDIOCODEC_SAI1_SD_A_Pin              GPIO_PIN_6
#define AUDIOCODEC_SAI1_SD_A_GPIO_Port        GPIOE
#define GPIO_15_Pin                           GPIO_PIN_13
#define GPIO_15_GPIO_Port                     GPIOC
#define GPIO_14_Pin                           GPIO_PIN_14
#define GPIO_14_GPIO_Port                     GPIOC
#define GPIO_13_Pin                           GPIO_PIN_15
#define GPIO_13_GPIO_Port                     GPIOC
#define RCC_OSC_IN_Pin                        GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port                  GPIOH
#define RCC_OSC_OUT_Pin                       GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port                 GPIOH
#define GPIO_12_Pin                           GPIO_PIN_0
#define GPIO_12_GPIO_Port                     GPIOC
#define SDCARD_CARD_DETECT_Pin                GPIO_PIN_1
#define SDCARD_CARD_DETECT_GPIO_Port          GPIOC
#define SDCARD_SPI_MISO_Pin                   GPIO_PIN_2
#define SDCARD_SPI_MISO_GPIO_Port             GPIOC
#define SDCARD_SPI_MOSI_Pin                   GPIO_PIN_3
#define SDCARD_SPI_MOSI_GPIO_Port             GPIOC
#define ENCODER_00_A_Pin                      GPIO_PIN_0
#define ENCODER_00_A_GPIO_Port                GPIOA
#define ENCODER_00_B_Pin                      GPIO_PIN_1
#define ENCODER_00_B_GPIO_Port                GPIOA
#define LED_15_Pin                            GPIO_PIN_2
#define LED_15_GPIO_Port                      GPIOA
#define ENCODER_00_BUTTON_Pin                 GPIO_PIN_3
#define ENCODER_00_BUTTON_GPIO_Port           GPIOA
#define DAC_00_Pin                            GPIO_PIN_4
#define DAC_00_GPIO_Port                      GPIOA
#define DAC_01_Pin                            GPIO_PIN_5
#define DAC_01_GPIO_Port                      GPIOA
#define LED_14_Pin                            GPIO_PIN_6
#define LED_14_GPIO_Port                      GPIOA
#define LED_13_Pin                            GPIO_PIN_7
#define LED_13_GPIO_Port                      GPIOA
#define ADC_00_Pin                            GPIO_PIN_4
#define ADC_00_GPIO_Port                      GPIOC
#define ADC_01_Pin                            GPIO_PIN_5
#define ADC_01_GPIO_Port                      GPIOC
#define LED_12_Pin                            GPIO_PIN_0
#define LED_12_GPIO_Port                      GPIOB
#define LED_11_Pin                            GPIO_PIN_1
#define LED_11_GPIO_Port                      GPIOB
#define GPIO_11_Pin                           GPIO_PIN_2
#define GPIO_11_GPIO_Port                     GPIOB
#define SERIAL_00_RX_Pin                      GPIO_PIN_7
#define SERIAL_00_RX_GPIO_Port                GPIOE
#define SERIAL_00_TX_Pin                      GPIO_PIN_8
#define SERIAL_00_TX_GPIO_Port                GPIOE
#define GPIO_10_Pin                           GPIO_PIN_9
#define GPIO_10_GPIO_Port                     GPIOE
#define GPIO_09_Pin                           GPIO_PIN_10
#define GPIO_09_GPIO_Port                     GPIOE
#define SPI_USR_CS_Pin                        GPIO_PIN_11
#define SPI_USR_CS_GPIO_Port                  GPIOE
#define SPI_USR_SCK_Pin                       GPIO_PIN_12
#define SPI_USR_SCK_GPIO_Port                 GPIOE
#define SPI_USR_MISO_Pin                      GPIO_PIN_13
#define SPI_USR_MISO_GPIO_Port                GPIOE
#define SPI_USR_MOSI_Pin                      GPIO_PIN_14
#define SPI_USR_MOSI_GPIO_Port                GPIOE
#define GPIO_08_Pin                           GPIO_PIN_15
#define GPIO_08_GPIO_Port                     GPIOE
#define I2C_USR_SCL_Pin                       GPIO_PIN_10
#define I2C_USR_SCL_GPIO_Port                 GPIOB
#define I2C_USR_SDA_Pin                       GPIO_PIN_11
#define I2C_USR_SDA_GPIO_Port                 GPIOB
#define SDCARD_SPI_CS_Pin                     GPIO_PIN_12
#define SDCARD_SPI_CS_GPIO_Port               GPIOB
#define SDCARD_SPI_SCK_Pin                    GPIO_PIN_13
#define SDCARD_SPI_SCK_GPIO_Port              GPIOB
#define USB_HOST__Pin                         GPIO_PIN_14
#define USB_HOST__GPIO_Port                   GPIOB
#define USB_HOST_B15_Pin                      GPIO_PIN_15
#define USB_HOST_B15_GPIO_Port                GPIOB
#define GPIO_07_Pin                           GPIO_PIN_8
#define GPIO_07_GPIO_Port                     GPIOD
#define GPIO_06_Pin                           GPIO_PIN_9
#define GPIO_06_GPIO_Port                     GPIOD
#define GPIO_05_Pin                           GPIO_PIN_10
#define GPIO_05_GPIO_Port                     GPIOD
#define GPIO_04_Pin                           GPIO_PIN_11
#define GPIO_04_GPIO_Port                     GPIOD
#define LED_10_Pin                            GPIO_PIN_12
#define LED_10_GPIO_Port                      GPIOD
#define LED_09_Pin                            GPIO_PIN_13
#define LED_09_GPIO_Port                      GPIOD
#define LED_08_Pin                            GPIO_PIN_14
#define LED_08_GPIO_Port                      GPIOD
#define LED_07_Pin                            GPIO_PIN_15
#define LED_07_GPIO_Port                      GPIOD
#define LED_06_Pin                            GPIO_PIN_6
#define LED_06_GPIO_Port                      GPIOC
#define LED_05_Pin                            GPIO_PIN_7
#define LED_05_GPIO_Port                      GPIOC
#define LED_04_Pin                            GPIO_PIN_8
#define LED_04_GPIO_Port                      GPIOC
#define LED_03_Pin                            GPIO_PIN_9
#define LED_03_GPIO_Port                      GPIOC
#define ENCODER_01_A_Pin                      GPIO_PIN_8
#define ENCODER_01_A_GPIO_Port                GPIOA
#define ENCODER_01_B_Pin                      GPIO_PIN_9
#define ENCODER_01_B_GPIO_Port                GPIOA
#define ENCODER_01_BUTTON_Pin                 GPIO_PIN_10
#define ENCODER_01_BUTTON_GPIO_Port           GPIOA
#define USB_DEVICE__Pin                       GPIO_PIN_11
#define USB_DEVICE__GPIO_Port                 GPIOA
#define USB_DEVICE_A12_Pin                    GPIO_PIN_12
#define USB_DEVICE_A12_GPIO_Port              GPIOA
#define SWD_SWDIO_Pin                         GPIO_PIN_13
#define SWD_SWDIO_GPIO_Port                   GPIOA
#define SWD_SWCLK_Pin                         GPIO_PIN_14
#define SWD_SWCLK_GPIO_Port                   GPIOA
#define SPI_DISPLAY_CS_Pin                    GPIO_PIN_15
#define SPI_DISPLAY_CS_GPIO_Port              GPIOA
#define SPI_DISPLAY_SCK_Pin                   GPIO_PIN_10
#define SPI_DISPLAY_SCK_GPIO_Port             GPIOC
#define SPI_DISPLAY_MISO_Pin                  GPIO_PIN_11
#define SPI_DISPLAY_MISO_GPIO_Port            GPIOC
#define SPI_DISPLAY_MOSI_Pin                  GPIO_PIN_12
#define SPI_DISPLAY_MOSI_GPIO_Port            GPIOC
#define SPI_DISPLAY_REGISTER_SELECT_Pin       GPIO_PIN_0
#define SPI_DISPLAY_REGISTER_SELECT_GPIO_Port GPIOD
#define SPI_DISPLAY_RESET_Pin                 GPIO_PIN_1
#define SPI_DISPLAY_RESET_GPIO_Port           GPIOD
#define GPIO_03_Pin                           GPIO_PIN_2
#define GPIO_03_GPIO_Port                     GPIOD
#define GPIO_02_Pin                           GPIO_PIN_3
#define GPIO_02_GPIO_Port                     GPIOD
#define GPIO_01_Pin                           GPIO_PIN_4
#define GPIO_01_GPIO_Port                     GPIOD
#define SERIAL_DEBUG_TX_Pin                   GPIO_PIN_5
#define SERIAL_DEBUG_TX_GPIO_Port             GPIOD
#define SERIAL_DEBUG_RX_Pin                   GPIO_PIN_6
#define SERIAL_DEBUG_RX_GPIO_Port             GPIOD
#define GPIO_00_Pin                           GPIO_PIN_7
#define GPIO_00_GPIO_Port                     GPIOD
#define SWD_SWO_Pin                           GPIO_PIN_3
#define SWD_SWO_GPIO_Port                     GPIOB
#define LED_02_Pin                            GPIO_PIN_4
#define LED_02_GPIO_Port                      GPIOB
#define LED_01_Pin                            GPIO_PIN_5
#define LED_01_GPIO_Port                      GPIOB
#define AUDIOCODEC_I2C_SCL_Pin                GPIO_PIN_6
#define AUDIOCODEC_I2C_SCL_GPIO_Port          GPIOB
#define AUDIOCODEC_I2C_SDA_Pin                GPIO_PIN_7
#define AUDIOCODEC_I2C_SDA_GPIO_Port          GPIOB
#define LED_00_Pin                            GPIO_PIN_8
#define LED_00_GPIO_Port                      GPIOB
#define BUTTON_PRG_Pin                        GPIO_PIN_9
#define BUTTON_PRG_GPIO_Port                  GPIOB
#define SERIAL_01_RX_Pin                      GPIO_PIN_0
#define SERIAL_01_RX_GPIO_Port                GPIOE
#define SERIAL_01_TX_Pin                      GPIO_PIN_1
#define SERIAL_01_TX_GPIO_Port                GPIOE

/* Private variables ---------------------------------------------------------*/
DMA_HandleTypeDef hdma_sai1_a;
DMA_HandleTypeDef hdma_sai1_b;
DMA_HandleTypeDef hdma_spi3_tx;
I2C_HandleTypeDef hi2c1;
SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;
SPI_HandleTypeDef hspi3;
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
    }
    /** Macro to configure the PLL clock source
     */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState            = RCC_HSE_ON;
    RCC_OscInitStruct.HSIState            = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM            = 2;
    RCC_OscInitStruct.PLL.PLLN            = 120;
    RCC_OscInitStruct.PLL.PLLP            = 2;
    RCC_OscInitStruct.PLL.PLLQ            = 2;
    RCC_OscInitStruct.PLL.PLLR            = 2;
    RCC_OscInitStruct.PLL.PLLRGE          = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL       = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN        = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
    PeriphCommonClock_Config();
}

/**
 * @brief Peripherals Common Clock Configuration
 * @retval None
 */
void PeriphCommonClock_Config(void) {
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Initializes the peripherals clock
     */
    // PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI4;
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI4 | RCC_PERIPHCLK_USB;  // << `RCC_PERIPHCLK_USB` added from `usbd_conf.c` or `usbh_conf.c`
    PeriphClkInitStruct.PLL2.PLL2M           = 8;
    PeriphClkInitStruct.PLL2.PLL2N           = 123;
    PeriphClkInitStruct.PLL2.PLL2P           = 2;
    PeriphClkInitStruct.PLL2.PLL2Q           = 2;
    PeriphClkInitStruct.PLL2.PLL2R           = 2;
    PeriphClkInitStruct.PLL2.PLL2RGE         = RCC_PLL2VCIRANGE_1;
    PeriphClkInitStruct.PLL2.PLL2VCOSEL      = RCC_PLL2VCOWIDE;
    PeriphClkInitStruct.PLL2.PLL2FRACN       = 0;
    PeriphClkInitStruct.PLL3.PLL3M           = 8;
    PeriphClkInitStruct.PLL3.PLL3N           = 120;
    PeriphClkInitStruct.PLL3.PLL3P           = 2;
    PeriphClkInitStruct.PLL3.PLL3Q           = 2;
    PeriphClkInitStruct.PLL3.PLL3R           = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE         = RCC_PLL3VCIRANGE_1;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL      = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN       = 0;
    PeriphClkInitStruct.Sai1ClockSelection   = RCC_SAI1CLKSOURCE_PLL2;
    PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL3;
    PeriphClkInitStruct.Spi45ClockSelection  = RCC_SPI45CLKSOURCE_PLL3;
    PeriphClkInitStruct.AdcClockSelection    = RCC_ADCCLKSOURCE_PLL3;
    PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_HSI48;  // << from `usbd_conf.c` or `usbh_conf.c`

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_15_Pin | GPIO_14_Pin | GPIO_13_Pin | GPIO_12_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIO_11_GPIO_Port, GPIO_11_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, GPIO_10_Pin | GPIO_09_Pin | GPIO_08_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, GPIO_07_Pin | GPIO_06_Pin | GPIO_05_Pin | GPIO_04_Pin | SPI_DISPLAY_REGISTER_SELECT_Pin | SPI_DISPLAY_RESET_Pin | GPIO_03_Pin | GPIO_02_Pin | GPIO_01_Pin | GPIO_00_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : GPIO_15_Pin GPIO_14_Pin GPIO_13_Pin GPIO_12_Pin */
    GPIO_InitStruct.Pin   = GPIO_15_Pin | GPIO_14_Pin | GPIO_13_Pin | GPIO_12_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : SDCARD_CARD_DETECT_Pin */
    GPIO_InitStruct.Pin  = SDCARD_CARD_DETECT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SDCARD_CARD_DETECT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : GPIO_11_Pin */
    GPIO_InitStruct.Pin   = GPIO_11_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_11_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : GPIO_10_Pin GPIO_09_Pin GPIO_08_Pin */
    GPIO_InitStruct.Pin   = GPIO_10_Pin | GPIO_09_Pin | GPIO_08_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : GPIO_07_Pin GPIO_06_Pin GPIO_05_Pin GPIO_04_Pin
                             SPI_DISPLAY_REGISTER_SELECT_Pin SPI_DISPLAY_RESET_Pin GPIO_03_Pin GPIO_02_Pin
                             GPIO_01_Pin GPIO_00_Pin */
    GPIO_InitStruct.Pin   = GPIO_07_Pin | GPIO_06_Pin | GPIO_05_Pin | GPIO_04_Pin | SPI_DISPLAY_REGISTER_SELECT_Pin | SPI_DISPLAY_RESET_Pin | GPIO_03_Pin | GPIO_02_Pin | GPIO_01_Pin | GPIO_00_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/**
 * Enable DMA controller clock
 */
void MX_DMA_Init(void) {
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();
    // __HAL_RCC_DMA2_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    /* DMA1_Stream1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    // /* DMA2_Stream0_IRQn interrupt configuration */
    // HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
void MX_I2C1_Init(void) {
    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance              = I2C1;
    hi2c1.Init.Timing           = 0x10707DBC;
    hi2c1.Init.OwnAddress1      = 0;
    hi2c1.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2      = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Analogue filter
     */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Digital filter
     */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief SAI1 Initialization Function
 * @param None
 * @retval None
 */
void MX_SAI1_Init(void) {
    /* USER CODE BEGIN SAI1_Init 0 */

    /* USER CODE END SAI1_Init 0 */

    /* USER CODE BEGIN SAI1_Init 1 */

    /* USER CODE END SAI1_Init 1 */
    hsai_BlockA1.Instance            = SAI1_Block_A;
    hsai_BlockA1.Init.AudioMode      = SAI_MODEMASTER_TX;
    hsai_BlockA1.Init.Synchro        = SAI_ASYNCHRONOUS;
    hsai_BlockA1.Init.OutputDrive    = SAI_OUTPUTDRIVE_DISABLE;
    hsai_BlockA1.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
    hsai_BlockA1.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_EMPTY;
    hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
    hsai_BlockA1.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
    hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
    hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
    hsai_BlockA1.Init.TriState       = SAI_OUTPUT_NOTRELEASED;
    if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK) {
        Error_Handler();
    }
    hsai_BlockB1.Instance            = SAI1_Block_B;
    hsai_BlockB1.Init.AudioMode      = SAI_MODESLAVE_RX;
    hsai_BlockB1.Init.Synchro        = SAI_SYNCHRONOUS;
    hsai_BlockB1.Init.OutputDrive    = SAI_OUTPUTDRIVE_DISABLE;
    hsai_BlockB1.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_EMPTY;
    hsai_BlockB1.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
    hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
    hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
    hsai_BlockB1.Init.TriState       = SAI_OUTPUT_NOTRELEASED;
    if (HAL_SAI_InitProtocol(&hsai_BlockB1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SAI1_Init 2 */

    /* USER CODE END SAI1_Init 2 */
}

/**
 * @brief SPI3 Initialization Function
 * @param None
 * @retval None
 */
void MX_SPI3_Init(void) {
    /* USER CODE BEGIN SPI3_Init 0 */

    /* USER CODE END SPI3_Init 0 */

    /* USER CODE BEGIN SPI3_Init 1 */

    /* USER CODE END SPI3_Init 1 */
    /* SPI3 parameter configuration*/
    hspi3.Instance                        = SPI3;
    hspi3.Init.Mode                       = SPI_MODE_MASTER;
    hspi3.Init.Direction                  = SPI_DIRECTION_2LINES_TXONLY;
    hspi3.Init.DataSize                   = SPI_DATASIZE_8BIT;
    hspi3.Init.CLKPolarity                = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase                   = SPI_PHASE_1EDGE;
    hspi3.Init.NSS                        = SPI_NSS_HARD_OUTPUT;
    hspi3.Init.BaudRatePrescaler          = SPI_BAUDRATEPRESCALER_4;
    hspi3.Init.FirstBit                   = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode                     = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation             = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial              = 0x0;
    hspi3.Init.NSSPMode                   = SPI_NSS_PULSE_ENABLE;
    hspi3.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
    hspi3.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
    hspi3.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi3.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi3.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi3.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi3.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi3.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    hspi3.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(&hspi3) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI3_Init 2 */

    /* USER CODE END SPI3_Init 2 */
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM1_Init(void) {
    /* USER CODE BEGIN TIM1_Init 0 */

    /* USER CODE END TIM1_Init 0 */

    TIM_Encoder_InitTypeDef sConfig       = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim1.Instance               = TIM1;
    htim1.Init.Prescaler         = 0;
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim1.Init.Period            = 65535;
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode          = TIM_ENCODERMODE_TI1;
    sConfig.IC1Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC1Filter            = 7;
    sConfig.IC2Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC2Filter            = 7;
    if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger  = TIM_TRGO_RESET;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM2_Init(void) {
    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    TIM_Encoder_InitTypeDef sConfig       = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM2_Init 1 */

    /* USER CODE END TIM2_Init 1 */
    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 0;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 4294967295;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode          = TIM_ENCODERMODE_TI1;
    sConfig.IC1Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC1Filter            = 7;
    sConfig.IC2Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC2Filter            = 7;
    if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM2_Init 2 */

    /* USER CODE END TIM2_Init 2 */
}

/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */
/* from `stm32h7xx_it.c`                                                               */
/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

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
 * @brief This function handles SPI3 global interrupt.
 */
void SPI3_IRQHandler(void) {
    /* USER CODE BEGIN SPI3_IRQn 0 */

    /* USER CODE END SPI3_IRQn 0 */
    HAL_SPI_IRQHandler(&hspi3);
    /* USER CODE BEGIN SPI3_IRQn 1 */

    /* USER CODE END SPI3_IRQn 1 */
}

/**
 * @brief This function handles DMA2 stream0 global interrupt.
 */
void DMA2_Stream0_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

    /* USER CODE END DMA2_Stream0_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_spi3_tx);
    /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

    /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */
/* from `stm32h7xx_hal_msp.c`                                                          */
/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void) {
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* System interrupt init*/

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

/**
 * @brief I2C MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c) {
    GPIO_InitTypeDef         GPIO_InitStruct     = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (hi2c->Instance == I2C1) {
        /* USER CODE BEGIN I2C1_MspInit 0 */

        /* USER CODE END I2C1_MspInit 0 */
        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
        PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin       = AUDIOCODEC_I2C_SCL_Pin | AUDIOCODEC_I2C_SDA_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
        /* USER CODE BEGIN I2C1_MspInit 1 */

        /* USER CODE END I2C1_MspInit 1 */
    } else if (hi2c->Instance == I2C2) {
        /* USER CODE BEGIN I2C2_MspInit 0 */

        /* USER CODE END I2C2_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
        PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_HSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
            Error_Handler();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        GPIO_InitStruct.Pin       = I2C_USR_SCL_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(I2C_USR_SCL_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = I2C_USR_SDA_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        HAL_GPIO_Init(I2C_USR_SDA_GPIO_Port, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();
        /* USER CODE BEGIN I2C2_MspInit 1 */

        /* USER CODE END I2C2_MspInit 1 */
    }
}

/**
 * @brief I2C MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c) {
    if (hi2c->Instance == I2C1) {
        /* USER CODE BEGIN I2C1_MspDeInit 0 */

        /* USER CODE END I2C1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C1_CLK_DISABLE();

        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        HAL_GPIO_DeInit(AUDIOCODEC_I2C_SCL_GPIO_Port, AUDIOCODEC_I2C_SCL_Pin);

        HAL_GPIO_DeInit(AUDIOCODEC_I2C_SDA_GPIO_Port, AUDIOCODEC_I2C_SDA_Pin);

        /* USER CODE BEGIN I2C1_MspDeInit 1 */

        /* USER CODE END I2C1_MspDeInit 1 */
    } else if (hi2c->Instance == I2C2) {
        /* USER CODE BEGIN I2C2_MspDeInit 0 */

        /* USER CODE END I2C2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_I2C2_CLK_DISABLE();

        /**I2C2 GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
        */
        HAL_GPIO_DeInit(I2C_USR_SCL_GPIO_Port, I2C_USR_SCL_Pin);

        HAL_GPIO_DeInit(I2C_USR_SDA_GPIO_Port, I2C_USR_SDA_Pin);

        /* USER CODE BEGIN I2C2_MspDeInit 1 */

        /* USER CODE END I2C2_MspDeInit 1 */
    }
}

/**
 * @brief SPI MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hspi->Instance == SPI2) {
        /* USER CODE BEGIN SPI2_MspInit 0 */

        /* USER CODE END SPI2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI2 GPIO Configuration
        PC2_C     ------> SPI2_MISO
        PC3_C     ------> SPI2_MOSI
        PB12     ------> SPI2_NSS
        PB13     ------> SPI2_SCK
        */
        GPIO_InitStruct.Pin       = SDCARD_SPI_MISO_Pin | SDCARD_SPI_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = SDCARD_SPI_CS_Pin | SDCARD_SPI_SCK_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI2_MspInit 1 */

        /* USER CODE END SPI2_MspInit 1 */
    } else if (hspi->Instance == SPI3) {
        /* USER CODE BEGIN SPI3_MspInit 0 */

        /* USER CODE END SPI3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI3_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**SPI3 GPIO Configuration
        PA15 (JTDI)     ------> SPI3_NSS
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PC12     ------> SPI3_MOSI
        */
        GPIO_InitStruct.Pin       = SPI_DISPLAY_CS_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(SPI_DISPLAY_CS_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = SPI_DISPLAY_SCK_Pin | SPI_DISPLAY_MISO_Pin | SPI_DISPLAY_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* SPI3 DMA Init */
        /* SPI3_TX Init */
        hdma_spi3_tx.Instance                 = DMA2_Stream0;
        hdma_spi3_tx.Init.Request             = DMA_REQUEST_SPI3_TX;
        hdma_spi3_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_spi3_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_spi3_tx.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_spi3_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma_spi3_tx.Init.Mode                = DMA_NORMAL;
        hdma_spi3_tx.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_spi3_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_spi3_tx) != HAL_OK) {
            Error_Handler();
        }

        __HAL_LINKDMA(hspi, hdmatx, hdma_spi3_tx);

        /* SPI3 interrupt Init */
        HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SPI3_IRQn);
        /* USER CODE BEGIN SPI3_MspInit 1 */

        /* USER CODE END SPI3_MspInit 1 */
    } else if (hspi->Instance == SPI4) {
        /* USER CODE BEGIN SPI4_MspInit 0 */

        /* USER CODE END SPI4_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI4_CLK_ENABLE();

        __HAL_RCC_GPIOE_CLK_ENABLE();
        /**SPI4 GPIO Configuration
        PE11     ------> SPI4_NSS
        PE12     ------> SPI4_SCK
        PE13     ------> SPI4_MISO
        PE14     ------> SPI4_MOSI
        */
        GPIO_InitStruct.Pin       = SPI_USR_CS_Pin | SPI_USR_SCK_Pin | SPI_USR_MISO_Pin | SPI_USR_MOSI_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI4_MspInit 1 */

        /* USER CODE END SPI4_MspInit 1 */
    }
}

/**
 * @brief SPI MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) {
    if (hspi->Instance == SPI2) {
        /* USER CODE BEGIN SPI2_MspDeInit 0 */

        /* USER CODE END SPI2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**SPI2 GPIO Configuration
        PC2_C     ------> SPI2_MISO
        PC3_C     ------> SPI2_MOSI
        PB12     ------> SPI2_NSS
        PB13     ------> SPI2_SCK
        */
        HAL_GPIO_DeInit(GPIOC, SDCARD_SPI_MISO_Pin | SDCARD_SPI_MOSI_Pin);

        HAL_GPIO_DeInit(GPIOB, SDCARD_SPI_CS_Pin | SDCARD_SPI_SCK_Pin);

        /* USER CODE BEGIN SPI2_MspDeInit 1 */

        /* USER CODE END SPI2_MspDeInit 1 */
    } else if (hspi->Instance == SPI3) {
        /* USER CODE BEGIN SPI3_MspDeInit 0 */

        /* USER CODE END SPI3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI3_CLK_DISABLE();

        /**SPI3 GPIO Configuration
        PA15 (JTDI)     ------> SPI3_NSS
        PC10     ------> SPI3_SCK
        PC11     ------> SPI3_MISO
        PC12     ------> SPI3_MOSI
        */
        HAL_GPIO_DeInit(SPI_DISPLAY_CS_GPIO_Port, SPI_DISPLAY_CS_Pin);

        HAL_GPIO_DeInit(GPIOC, SPI_DISPLAY_SCK_Pin | SPI_DISPLAY_MISO_Pin | SPI_DISPLAY_MOSI_Pin);

        /* SPI3 DMA DeInit */
        HAL_DMA_DeInit(hspi->hdmatx);

        /* SPI3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(SPI3_IRQn);
        /* USER CODE BEGIN SPI3_MspDeInit 1 */

        /* USER CODE END SPI3_MspDeInit 1 */
    } else if (hspi->Instance == SPI4) {
        /* USER CODE BEGIN SPI4_MspDeInit 0 */

        /* USER CODE END SPI4_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI4_CLK_DISABLE();

        /**SPI4 GPIO Configuration
        PE11     ------> SPI4_NSS
        PE12     ------> SPI4_SCK
        PE13     ------> SPI4_MISO
        PE14     ------> SPI4_MOSI
        */
        HAL_GPIO_DeInit(GPIOE, SPI_USR_CS_Pin | SPI_USR_SCK_Pin | SPI_USR_MISO_Pin | SPI_USR_MOSI_Pin);

        /* USER CODE BEGIN SPI4_MspDeInit 1 */

        /* USER CODE END SPI4_MspDeInit 1 */
    }
}

/**
 * @brief TIM_Encoder MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_encoder: TIM_Encoder handle pointer
 * @retval None
 */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (htim_encoder->Instance == TIM1) {
        /* USER CODE BEGIN TIM1_MspInit 0 */

        /* USER CODE END TIM1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PA8     ------> TIM1_CH1
        PA9     ------> TIM1_CH2
        PA10     ------> TIM1_CH3
        */
        GPIO_InitStruct.Pin       = ENCODER_01_A_Pin | ENCODER_01_B_Pin | ENCODER_01_BUTTON_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM1_MspInit 1 */

        /* USER CODE END TIM1_MspInit 1 */
    } else if (htim_encoder->Instance == TIM2) {
        /* USER CODE BEGIN TIM2_MspInit 0 */

        /* USER CODE END TIM2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM2 GPIO Configuration
        PA0     ------> TIM2_CH1
        PA1     ------> TIM2_CH2
        PA3     ------> TIM2_CH4
        */
        GPIO_InitStruct.Pin       = ENCODER_00_A_Pin | ENCODER_00_B_Pin | ENCODER_00_BUTTON_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM2_MspInit 1 */

        /* USER CODE END TIM2_MspInit 1 */
    }
}

/**
 * @brief TIM_Encoder MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_encoder: TIM_Encoder handle pointer
 * @retval None
 */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_encoder) {
    if (htim_encoder->Instance == TIM1) {
        /* USER CODE BEGIN TIM1_MspDeInit 0 */

        /* USER CODE END TIM1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM1_CLK_DISABLE();

        /**TIM1 GPIO Configuration
        PA8     ------> TIM1_CH1
        PA9     ------> TIM1_CH2
        PA10     ------> TIM1_CH3
        */
        HAL_GPIO_DeInit(GPIOA, ENCODER_01_A_Pin | ENCODER_01_B_Pin | ENCODER_01_BUTTON_Pin);

        /* USER CODE BEGIN TIM1_MspDeInit 1 */

        /* USER CODE END TIM1_MspDeInit 1 */
    } else if (htim_encoder->Instance == TIM2) {
        /* USER CODE BEGIN TIM2_MspDeInit 0 */

        /* USER CODE END TIM2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_TIM2_CLK_DISABLE();

        /**TIM2 GPIO Configuration
        PA0     ------> TIM2_CH1
        PA1     ------> TIM2_CH2
        PA3     ------> TIM2_CH4
        */
        HAL_GPIO_DeInit(GPIOA, ENCODER_00_A_Pin | ENCODER_00_B_Pin | ENCODER_00_BUTTON_Pin);

        /* USER CODE BEGIN TIM2_MspDeInit 1 */

        /* USER CODE END TIM2_MspDeInit 1 */
    }
}

static uint32_t SAI1_client = 0;

void HAL_SAI_MspInit(SAI_HandleTypeDef* hsai) {
    GPIO_InitTypeDef GPIO_InitStruct;
    /* SAI1 */
    if (hsai->Instance == SAI1_Block_A) {
        /* Peripheral clock enable */
        if (SAI1_client == 0) {
            __HAL_RCC_SAI1_CLK_ENABLE();
        }
        SAI1_client++;

        /**SAI1_A_Block_A GPIO Configuration
        PE2     ------> SAI1_MCLK_A
        PE4     ------> SAI1_FS_A
        PE5     ------> SAI1_SCK_A
        PE6     ------> SAI1_SD_A
        */
        GPIO_InitStruct.Pin       = AUDIOCODEC_SAI1_MCLK_A_Pin | AUDIOCODEC_SAI1_FS_A_Pin | AUDIOCODEC_SAI1_SCK_A_Pin | AUDIOCODEC_SAI1_SD_A_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* Peripheral DMA init*/

        hdma_sai1_a.Instance                 = DMA1_Stream0;
        hdma_sai1_a.Init.Request             = DMA_REQUEST_SAI1_A;
        hdma_sai1_a.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_sai1_a.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_sai1_a.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_sai1_a.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_sai1_a.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_sai1_a.Init.Mode                = DMA_CIRCULAR;
        hdma_sai1_a.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_sai1_a.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_sai1_a) != HAL_OK) {
            Error_Handler();
        }

        /* Several peripheral DMA handle pointers point to the same DMA handle.
         Be aware that there is only one channel to perform all the requested DMAs. */
        __HAL_LINKDMA(hsai, hdmarx, hdma_sai1_a);

        __HAL_LINKDMA(hsai, hdmatx, hdma_sai1_a);
    }
    if (hsai->Instance == SAI1_Block_B) {
        /* Peripheral clock enable */
        if (SAI1_client == 0) {
            __HAL_RCC_SAI1_CLK_ENABLE();
        }
        SAI1_client++;

        /**SAI1_B_Block_B GPIO Configuration
        PE3     ------> SAI1_SD_B
        */
        GPIO_InitStruct.Pin       = AUDIOCODEC_SAI1_SD_B_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SAI1;
        HAL_GPIO_Init(AUDIOCODEC_SAI1_SD_B_GPIO_Port, &GPIO_InitStruct);

        /* Peripheral DMA init*/

        hdma_sai1_b.Instance                 = DMA1_Stream1;
        hdma_sai1_b.Init.Request             = DMA_REQUEST_SAI1_B;
        hdma_sai1_b.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma_sai1_b.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_sai1_b.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_sai1_b.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_sai1_b.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_sai1_b.Init.Mode                = DMA_CIRCULAR;
        hdma_sai1_b.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_sai1_b.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_sai1_b) != HAL_OK) {
            Error_Handler();
        }

        /* Several peripheral DMA handle pointers point to the same DMA handle.
         Be aware that there is only one channel to perform all the requested DMAs. */
        __HAL_LINKDMA(hsai, hdmarx, hdma_sai1_b);
        __HAL_LINKDMA(hsai, hdmatx, hdma_sai1_b);
    }
}

void HAL_SAI_MspDeInit(SAI_HandleTypeDef* hsai) {
    /* SAI1 */
    if (hsai->Instance == SAI1_Block_A) {
        SAI1_client--;
        if (SAI1_client == 0) {
            /* Peripheral clock disable */
            __HAL_RCC_SAI1_CLK_DISABLE();
        }

        /**SAI1_A_Block_A GPIO Configuration
        PE2     ------> SAI1_MCLK_A
        PE4     ------> SAI1_FS_A
        PE5     ------> SAI1_SCK_A
        PE6     ------> SAI1_SD_A
        */
        HAL_GPIO_DeInit(GPIOE, AUDIOCODEC_SAI1_MCLK_A_Pin | AUDIOCODEC_SAI1_FS_A_Pin | AUDIOCODEC_SAI1_SCK_A_Pin | AUDIOCODEC_SAI1_SD_A_Pin);

        /* SAI1 DMA Deinit */
        HAL_DMA_DeInit(hsai->hdmarx);
        HAL_DMA_DeInit(hsai->hdmatx);
    }
    if (hsai->Instance == SAI1_Block_B) {
        SAI1_client--;
        if (SAI1_client == 0) {
            /* Peripheral clock disable */
            __HAL_RCC_SAI1_CLK_DISABLE();
        }

        /**SAI1_B_Block_B GPIO Configuration
        PE3     ------> SAI1_SD_B
        */
        HAL_GPIO_DeInit(AUDIOCODEC_SAI1_SD_B_GPIO_Port, AUDIOCODEC_SAI1_SD_B_Pin);

        /* SAI1 DMA Deinit */
        HAL_DMA_DeInit(hsai->hdmarx);
        HAL_DMA_DeInit(hsai->hdmatx);
    }
}

#endif
