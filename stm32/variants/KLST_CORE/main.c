/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c3;

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

SPI_HandleTypeDef hspi4;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

// PCD_HandleTypeDef hpcd_USB_OTG_FS;
// HCD_HandleTypeDef hhcd_USB_OTG_HS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
// void MX_USB_OTG_FS_PCD_Init(void);
// void MX_USB_OTG_HS_HCD_Init(void);
void MX_I2S2_Init(void);
void MX_I2C3_Init(void);
void MX_SPI4_Init(void);
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
// int main(void)
// {
//   /* USER CODE BEGIN 1 */
//
//   /* USER CODE END 1 */
//
//   /* MCU Configuration--------------------------------------------------------*/
//
//   /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//   HAL_Init();
//
//   /* USER CODE BEGIN Init */
//
//   /* USER CODE END Init */
//
//   /* Configure the system clock */
//   SystemClock_Config();
//
// /* Configure the peripherals common clocks */
//   PeriphCommonClock_Config();
//
//   /* USER CODE BEGIN SysInit */
//
//   /* USER CODE END SysInit */
//
//   /* Initialize all configured peripherals */
//   MX_GPIO_Init();
//   MX_DMA_Init();
//   MX_USB_OTG_FS_PCD_Init();
//   MX_USB_OTG_HS_HCD_Init();
//   MX_I2S2_Init();
//   MX_I2C3_Init();
//   MX_SPI4_Init();
//   MX_TIM1_Init();
//   MX_TIM2_Init();
//   MX_TIM5_Init();
//   /* USER CODE BEGIN 2 */
//   HAL_GPIO_TogglePin(LED_01_GPIO_Port, LED_01_Pin);
//   /* USER CODE END 2 */
//
//   /* Infinite loop */
//   /* USER CODE BEGIN WHILE */
//   while (1)
//   {
//     /* USER CODE END WHILE */
//
//     /* USER CODE BEGIN 3 */
// 	  HAL_GPIO_TogglePin(LED_00_GPIO_Port, LED_00_Pin);
// 	  HAL_GPIO_TogglePin(LED_01_GPIO_Port, LED_01_Pin);
// 	  HAL_GPIO_TogglePin(LED_02_GPIO_Port, LED_02_Pin);
// 	  HAL_Delay(500);
//   }
//   /* USER CODE END 3 */
// }

/**
  * @brief System Clock Configuration
  * @retval None
  */
__attribute__((weak)) void SystemClock_Config(void) {
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
    /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSI48State     = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 2;
    RCC_OscInitStruct.PLL.PLLN       = 120;
    RCC_OscInitStruct.PLL.PLLP       = 2;
    RCC_OscInitStruct.PLL.PLLQ       = 6;
    RCC_OscInitStruct.PLL.PLLR       = 2;
    RCC_OscInitStruct.PLL.PLLRGE     = RCC_PLL1VCIRANGE_3;
    RCC_OscInitStruct.PLL.PLLVCOSEL  = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN   = 0;
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

    /** Initializes the peripherals clock */
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection     = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.PLL3.PLL3M               = 4;
    PeriphClkInitStruct.PLL3.PLL3N               = 48;
    PeriphClkInitStruct.PLL3.PLL3P               = 4;
    PeriphClkInitStruct.PLL3.PLL3Q               = 4;
    PeriphClkInitStruct.PLL3.PLL3R               = 4;
    PeriphClkInitStruct.PLL3.PLL3RGE             = RCC_PLL3VCIRANGE_2;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL          = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN           = 0;
    PeriphClkInitStruct.UsbClockSelection        = RCC_USBCLKSOURCE_PLL3;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
}

//
// void SystemClock_Config(void)
// {
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//
//   /** Supply configuration update enable
//   */
//   HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
//   /** Configure the main internal regulator output voltage
//   */
//   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
//
//   while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
//   /** Macro to configure the PLL clock source
//   */
//   __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
//   /** Initializes the RCC Oscillators according to the specified parameters
//   * in the RCC_OscInitTypeDef structure.
//   */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//   RCC_OscInitStruct.PLL.PLLM = 2;
//   RCC_OscInitStruct.PLL.PLLN = 120;
//   RCC_OscInitStruct.PLL.PLLP = 2;
//   RCC_OscInitStruct.PLL.PLLQ = 6;
//   RCC_OscInitStruct.PLL.PLLR = 2;
//   RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
//   RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
//   RCC_OscInitStruct.PLL.PLLFRACN = 0;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Initializes the CPU, AHB and APB buses clocks
//   */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
//                               |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
//   RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
//   RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
//
//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//   {
//     Error_Handler();
//   }
//
//   PeriphCommonClock_Config();
// }
//
// /**
//   * @brief Peripherals Common Clock Configuration
//   * @retval None
//   */
// void PeriphCommonClock_Config(void)
// {
// RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

// /** Initializes the peripherals clock
// */
// PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
// PeriphClkInitStruct.PLL3.PLL3M = 4;
// PeriphClkInitStruct.PLL3.PLL3N = 48;
// PeriphClkInitStruct.PLL3.PLL3P = 4;
// PeriphClkInitStruct.PLL3.PLL3Q = 4;
// PeriphClkInitStruct.PLL3.PLL3R = 4;
// PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
// PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
// PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
// PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
// if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
// {
//   Error_Handler();
// }
// }

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C3_Init(void) {
    /* USER CODE BEGIN I2C3_Init 0 */

    /* USER CODE END I2C3_Init 0 */

    /* USER CODE BEGIN I2C3_Init 1 */

    /* USER CODE END I2C3_Init 1 */
    hi2c3.Instance              = I2C3;
    hi2c3.Init.Timing           = 0x307075B1;
    hi2c3.Init.OwnAddress1      = 0;
    hi2c3.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    hi2c3.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    hi2c3.Init.OwnAddress2      = 0;
    hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c3.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    hi2c3.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Analogue filter
  */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }
    /** Configure Digital filter
  */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2C3_Init 2 */

    /* USER CODE END I2C3_Init 2 */
}

/**
  * @brief I2S2 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2S2_Init(void) {
    /* USER CODE BEGIN I2S2_Init 0 */

    /* USER CODE END I2S2_Init 0 */

    /* USER CODE BEGIN I2S2_Init 1 */

    /* USER CODE END I2S2_Init 1 */
    hi2s2.Instance                = SPI2;
    hi2s2.Init.Mode               = I2S_MODE_MASTER_TX;
    hi2s2.Init.Standard           = I2S_STANDARD_PHILIPS;
    hi2s2.Init.DataFormat         = I2S_DATAFORMAT_16B;
    hi2s2.Init.MCLKOutput         = I2S_MCLKOUTPUT_ENABLE;
    hi2s2.Init.AudioFreq          = I2S_AUDIOFREQ_48K;
    hi2s2.Init.CPOL               = I2S_CPOL_LOW;
    hi2s2.Init.FirstBit           = I2S_FIRSTBIT_MSB;
    hi2s2.Init.WSInversion        = I2S_WS_INVERSION_DISABLE;
    hi2s2.Init.Data24BitAlignment = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
    hi2s2.Init.MasterKeepIOState  = I2S_MASTER_KEEP_IO_STATE_DISABLE;
    if (HAL_I2S_Init(&hi2s2) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN I2S2_Init 2 */

    /* USER CODE END I2S2_Init 2 */
}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI4_Init(void) {
    /* USER CODE BEGIN SPI4_Init 0 */

    /* USER CODE END SPI4_Init 0 */

    /* USER CODE BEGIN SPI4_Init 1 */

    /* USER CODE END SPI4_Init 1 */
    /* SPI4 parameter configuration*/
    hspi4.Instance                        = SPI4;
    hspi4.Init.Mode                       = SPI_MODE_MASTER;
    hspi4.Init.Direction                  = SPI_DIRECTION_2LINES;
    hspi4.Init.DataSize                   = SPI_DATASIZE_4BIT;
    hspi4.Init.CLKPolarity                = SPI_POLARITY_LOW;
    hspi4.Init.CLKPhase                   = SPI_PHASE_1EDGE;
    hspi4.Init.NSS                        = SPI_NSS_HARD_OUTPUT;
    hspi4.Init.BaudRatePrescaler          = SPI_BAUDRATEPRESCALER_2;
    hspi4.Init.FirstBit                   = SPI_FIRSTBIT_MSB;
    hspi4.Init.TIMode                     = SPI_TIMODE_DISABLE;
    hspi4.Init.CRCCalculation             = SPI_CRCCALCULATION_DISABLE;
    hspi4.Init.CRCPolynomial              = 0x0;
    hspi4.Init.NSSPMode                   = SPI_NSS_PULSE_ENABLE;
    hspi4.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
    hspi4.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
    hspi4.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi4.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
    hspi4.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
    hspi4.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    hspi4.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    hspi4.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
    hspi4.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
    if (HAL_SPI_Init(&hspi4) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN SPI4_Init 2 */

    /* USER CODE END SPI4_Init 2 */
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
    sConfig.IC1Filter            = 0;
    sConfig.IC2Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC2Filter            = 0;
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
    sConfig.IC1Filter            = 0;
    sConfig.IC2Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC2Filter            = 0;
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

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM5_Init(void) {
    /* USER CODE BEGIN TIM5_Init 0 */

    /* USER CODE END TIM5_Init 0 */

    TIM_Encoder_InitTypeDef sConfig       = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM5_Init 1 */

    /* USER CODE END TIM5_Init 1 */
    htim5.Instance               = TIM5;
    htim5.Init.Prescaler         = 0;
    htim5.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim5.Init.Period            = 4294967295;
    htim5.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sConfig.EncoderMode          = TIM_ENCODERMODE_TI1;
    sConfig.IC1Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC1Filter            = 0;
    sConfig.IC2Polarity          = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection         = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler         = TIM_ICPSC_DIV1;
    sConfig.IC2Filter            = 0;
    if (HAL_TIM_Encoder_Init(&htim5, &sConfig) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM5_Init 2 */

    /* USER CODE END TIM5_Init 2 */
}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
// void MX_USB_OTG_FS_PCD_Init(void)
// {

//   /* USER CODE BEGIN USB_OTG_FS_Init 0 */

//   /* USER CODE END USB_OTG_FS_Init 0 */

//   /* USER CODE BEGIN USB_OTG_FS_Init 1 */

//   /* USER CODE END USB_OTG_FS_Init 1 */
//   hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
//   hpcd_USB_OTG_FS.Init.dev_endpoints = 9;
//   hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
//   hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
//   hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
//   hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
//   hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
//   hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
//   hpcd_USB_OTG_FS.Init.battery_charging_enable = DISABLE;
//   hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
//   hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
//   if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN USB_OTG_FS_Init 2 */

//   /* USER CODE END USB_OTG_FS_Init 2 */

// }

/**
  * @brief USB_OTG_HS Initialization Function
  * @param None
  * @retval None
  */
// void MX_USB_OTG_HS_HCD_Init(void)
// {

//   /* USER CODE BEGIN USB_OTG_HS_Init 0 */

//   /* USER CODE END USB_OTG_HS_Init 0 */

//   /* USER CODE BEGIN USB_OTG_HS_Init 1 */

//   /* USER CODE END USB_OTG_HS_Init 1 */
//   hhcd_USB_OTG_HS.Instance = USB_OTG_HS;
//   hhcd_USB_OTG_HS.Init.Host_channels = 16;
//   hhcd_USB_OTG_HS.Init.speed = HCD_SPEED_FULL;
//   hhcd_USB_OTG_HS.Init.dma_enable = DISABLE;
//   hhcd_USB_OTG_HS.Init.phy_itface = USB_OTG_EMBEDDED_PHY;
//   hhcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
//   hhcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
//   hhcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
//   if (HAL_HCD_Init(&hhcd_USB_OTG_HS) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN USB_OTG_HS_Init 2 */

//   /* USER CODE END USB_OTG_HS_Init 2 */

// }

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) {
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream0_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    /* DMA1_Stream1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
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
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, LED_01_Pin | LED_02_Pin | LED_00_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : GPIO_01_Pin GPIO_00_Pin GPIO_02_Pin SDCARD_DETECT_SDIO_Pin */
    GPIO_InitStruct.Pin  = GPIO_01_Pin | GPIO_00_Pin | GPIO_02_Pin | SDCARD_DETECT_SDIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pins : GPIO_03_Pin GPIO_08_Pin GPIO_09_Pin GPIO_10_Pin
                           GPIO_11_Pin */
    GPIO_InitStruct.Pin  = GPIO_03_Pin | GPIO_08_Pin | GPIO_09_Pin | GPIO_10_Pin | GPIO_11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : I2C_00_SDA_Pin I2C_00_SCL_Pin */
    GPIO_InitStruct.Pin       = I2C_00_SDA_Pin | I2C_00_SCL_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : GPIO_04_Pin GPIO_05_Pin GPIO_06_Pin GPIO_07_Pin */
    GPIO_InitStruct.Pin  = GPIO_04_Pin | GPIO_05_Pin | GPIO_06_Pin | GPIO_07_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : ADC_00_Pin ADC_03_Pin ADC_01_Pin ADC_04_Pin
                           ADC_02_Pin ADC_05_Pin */
    GPIO_InitStruct.Pin  = ADC_00_Pin | ADC_03_Pin | ADC_01_Pin | ADC_04_Pin | ADC_02_Pin | ADC_05_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : SPI_00_SCK_Pin SPI_00_MISO_Pin SPI_00_MOSI_Pin */
    GPIO_InitStruct.Pin       = SPI_00_SCK_Pin | SPI_00_MISO_Pin | SPI_00_MOSI_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI5;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /*Configure GPIO pins : USART_02_TX_Pin USART_02_RX_Pin */
    GPIO_InitStruct.Pin       = USART_02_TX_Pin | USART_02_RX_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : DAC_01_Pin DAC_00_Pin */
    GPIO_InitStruct.Pin  = DAC_01_Pin | DAC_00_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : SPI_01_MISO_Pin */
    GPIO_InitStruct.Pin       = SPI_01_MISO_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI_01_MISO_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : SPI_02_MOSI_Pin */
    GPIO_InitStruct.Pin       = SPI_02_MOSI_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_SPI6;
    HAL_GPIO_Init(SPI_02_MOSI_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : GPIO_12_Pin GPIO_13_Pin */
    GPIO_InitStruct.Pin  = GPIO_12_Pin | GPIO_13_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : I2C_02_SDA_Pin */
    GPIO_InitStruct.Pin       = I2C_02_SDA_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
    HAL_GPIO_Init(I2C_02_SDA_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : GPIO_14_Pin GPIO_15_Pin BUTTON_PROGRAMMER_Pin ENCODER_01_BUTTON_Pin
                           ENCODER_02_BUTTON_Pin ENCODER_00_BUTTON_Pin */
    GPIO_InitStruct.Pin  = GPIO_14_Pin | GPIO_15_Pin | BUTTON_PROGRAMMER_Pin | ENCODER_01_BUTTON_Pin | ENCODER_02_BUTTON_Pin | ENCODER_00_BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pins : RJ45_UART_TX_Pin RJ45_UART_RX_Pin */
    GPIO_InitStruct.Pin       = RJ45_UART_TX_Pin | RJ45_UART_RX_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : I2C_02_SCL_Pin */
    GPIO_InitStruct.Pin       = I2C_02_SCL_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
    HAL_GPIO_Init(I2C_02_SCL_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : BUTTON_USER_01_Pin BUTTON_USER_00_Pin */
    GPIO_InitStruct.Pin  = BUTTON_USER_01_Pin | BUTTON_USER_00_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : LED_01_Pin LED_02_Pin LED_00_Pin */
    GPIO_InitStruct.Pin   = LED_01_Pin | LED_02_Pin | LED_00_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pins : USART_01_TX_Pin USART_01_RX_Pin */
    GPIO_InitStruct.Pin       = USART_01_TX_Pin | USART_01_RX_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : RJ45_SPI_SCK_Pin RJ45_SPI_MISO_Pin RJ45_SPI_MOSI_Pin */
    GPIO_InitStruct.Pin       = RJ45_SPI_SCK_Pin | RJ45_SPI_MISO_Pin | RJ45_SPI_MOSI_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : SPI_01_MOSI_Pin */
    GPIO_InitStruct.Pin       = SPI_01_MOSI_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI_01_MOSI_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : USART_00_RX_Pin USART_00_TX_Pin */
    GPIO_InitStruct.Pin       = USART_00_RX_Pin | USART_00_TX_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART6;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pin : SPI_01_SCK_Pin */
    GPIO_InitStruct.Pin       = SPI_01_SCK_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(SPI_01_SCK_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : SPI_02_MISO_Pin SPI_02_SCK_Pin */
    GPIO_InitStruct.Pin       = SPI_02_MISO_Pin | SPI_02_SCK_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pins : I2C_01_SCL_Pin I2C_01_SDA_Pin */
    GPIO_InitStruct.Pin       = I2C_01_SCL_Pin | I2C_01_SDA_Pin;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
