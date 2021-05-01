#include "stm32h7xx_hal.h"
#include "klangstrom_arduino.h"
#include "klangstrom_peripherals.h"
#include "stm32_def_build.h"
#include "Arduino.h"
#include "AudioCodecWM8731.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------------- */

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

I2C_HandleTypeDef hi2c2;


/* -------------------------------------------------------------------------------- */
/* copied from `main.c`, `stm32h7xx_hal_msp.c`, `stm32h7xx_it.c` */
/* -------------------------------------------------------------------------------- */

/**
    @brief System Clock Configuration
    @retval None
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
    in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 24;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_SPI2
      | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable USB Voltage detector
  */
  HAL_PWREx_EnableUSBVoltageDetector();
}

/**
    @brief I2C2 Initialization Function
    @param None
    @retval None
*/
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10707DBC;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
    @brief I2S2 Initialization Function
    @param None
    @retval None
*/
void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_RX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_44K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.FirstBit = I2S_FIRSTBIT_MSB;
  hi2s2.Init.WSInversion = I2S_WS_INVERSION_DISABLE;
  hi2s2.Init.Data24BitAlignment = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
  hi2s2.Init.MasterKeepIOState = I2S_MASTER_KEEP_IO_STATE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */
  hi2s2.Init.Mode = I2S_MODE_MASTER_FULLDUPLEX; /* fixes bug in CubeMX */
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END I2S2_Init 2 */

}

/**
    Enable DMA controller clock
*/
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

/**
    Initializes the Global MSP.
*/
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
  @brief I2C MSP Initialization
  This function configures the hardware resources used in this example
  @param hi2c: I2C handle pointer
  @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (hi2c->Instance == I2C2)
  {
    /* USER CODE BEGIN I2C2_MspInit 0 */

    /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**I2C2 GPIO Configuration
      PF0     ------> I2C2_SDA
      PF1     ------> I2C2_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
    /* USER CODE BEGIN I2C2_MspInit 1 */

    /* USER CODE END I2C2_MspInit 1 */
  }

}

/**
  @brief I2S MSP Initialization
  This function configures the hardware resources used in this example
  @param hi2s: I2S handle pointer
  @retval None
*/
void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (hi2s->Instance == SPI2)
  {
    /* USER CODE BEGIN SPI2_MspInit 0 */

    /* USER CODE END SPI2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2S2 GPIO Configuration
      PC2_C     ------> I2S2_SDI
      PC3_C     ------> I2S2_SDO
      PB10     ------> I2S2_CK
      PB12     ------> I2S2_WS
      PC6     ------> I2S2_MCK
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2S2 DMA Init */
    /* SPI2_RX Init */
    hdma_spi2_rx.Instance = DMA1_Stream0;
    hdma_spi2_rx.Init.Request = DMA_REQUEST_SPI2_RX;
    hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_spi2_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hi2s, hdmarx, hdma_spi2_rx);

    /* SPI2_TX Init */
    hdma_spi2_tx.Instance = DMA1_Stream1;
    hdma_spi2_tx.Init.Request = DMA_REQUEST_SPI2_TX;
    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi2_tx.Init.Mode = DMA_CIRCULAR;
    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hi2s, hdmatx, hdma_spi2_tx);

    /* USER CODE BEGIN SPI2_MspInit 1 */

    /* USER CODE END SPI2_MspInit 1 */
  }

}

/**
    @brief This function handles DMA1 stream0 global interrupt.
*/
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi2_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
    @brief This function handles DMA1 stream1 global interrupt.
*/
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi2_tx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/* -------------------------------------------------------------------------------- */
/* KLST */
/* -------------------------------------------------------------------------------- */

uint32_t mTXBuffer[I2S_BUFFER_SIZE];
uint32_t mRXBuffer[I2S_BUFFER_SIZE];
uint32_t *mCurrentRXBuffer;

#if I2S_USE_DMA

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
  KLST_fillBuffer(&(mTXBuffer[I2S_BUFFER_SIZE >> 1]),
                  mCurrentRXBuffer,
                  I2S_BUFFER_SIZE >> 1);
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  KLST_fillBuffer(&(mTXBuffer[0]),
                  mCurrentRXBuffer,
                  I2S_BUFFER_SIZE >> 1);
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
  mCurrentRXBuffer = &(mRXBuffer[I2S_BUFFER_SIZE >> 1]);
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
	mCurrentRXBuffer = &(mRXBuffer[0]);
}

void KLST_startDMA(I2S_HandleTypeDef *hi2s) {
  if (HAL_I2S_Transmit_DMA(hi2s, (uint16_t*) mTXBuffer, I2S_BUFFER_SIZE << 1) != HAL_OK) {
    Error_Handler();
  }
  hi2s2.State = HAL_I2S_STATE_READY; // state flag needs to be cleared manually
  if (HAL_I2S_Receive_DMA(hi2s, (uint16_t*) mRXBuffer, I2S_BUFFER_SIZE << 1) != HAL_OK) {
    Error_Handler();
  }
}

#else

#if I2S_BUFFER_SIZE > 16
#warning BUFFER_SIZE > 16 might cause problems.
#endif

#include "klangstrom_ISR.h"

uint16_t mBufferPointer = 0;
bool mScheduleHalfBufferUpdate = false;
bool mScheduleCptlBufferUpdate = false;

#define __I2S_TIMEOUT             100
#define __I2S_DIRECT_BUFFER_SIZE  2

void KLST_I2S_transmit(uint16_t pLeft, uint16_t pRight) {
  uint16_t mBuffer[__I2S_DIRECT_BUFFER_SIZE];
  mBuffer[0] = pLeft;
  mBuffer[1] = pRight;
  HAL_I2S_Transmit(&hi2s2, mBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
}

void KLST_I2S_receive(uint16_t* pLeft, uint16_t* pRight) {
	uint16_t mBuffer[__I2S_DIRECT_BUFFER_SIZE];
	HAL_I2S_Receive(&hi2s2, mBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
	*pLeft = mBuffer[0];
	*pRight = mBuffer[1];
}

WEAK void KLST_audio_interrupt() {
  /* update buffer */
  if (mBufferPointer == I2S_BUFFER_SIZE/2) {
    // void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
    KLST_fillBuffer(&(mTXBuffer[0]),
                    &(mRXBuffer[0]),
                    I2S_BUFFER_SIZE >> 1);
  } else if (mBufferPointer >= I2S_BUFFER_SIZE) {
    // void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
    KLST_fillBuffer(&(mTXBuffer[I2S_BUFFER_SIZE >> 1]),
                    &(mRXBuffer[I2S_BUFFER_SIZE >> 1]),
                    I2S_BUFFER_SIZE >> 1);
    mBufferPointer = 0;
  }
  /* retreive sample from I2S */
  uint16_t mTmpRXBuffer[__I2S_DIRECT_BUFFER_SIZE];
 	HAL_I2S_Receive(&hi2s2, mTmpRXBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
  mRXBuffer[mBufferPointer] = ((uint32_t) (uint16_t) mTmpRXBuffer[0]) << 0 | ((uint32_t) (uint16_t) mTmpRXBuffer[0]) << 16;
  /* send sample to I2S */
  uint16_t *mTmpTXBuffer = (uint16_t*)&mTXBuffer[mBufferPointer];
  HAL_I2S_Transmit(&hi2s2, mTmpTXBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
  mBufferPointer++;
}

#endif

void KLST_init() {
#if I2S_USE_DMA
  MX_DMA_Init();
#endif
  MX_I2S2_Init();
  MX_I2C2_Init();

  WM8731_init();
  WM8731_inputSelect(AUDIO_INPUT_MIC);
  WM8731_volumeInteger(110);

  memset(mTXBuffer, 0, sizeof(mTXBuffer));
  memset(mRXBuffer, 0, sizeof(mRXBuffer));
#if I2S_USE_DMA
  KLST_startDMA(&hi2s2); // - `I2S2_SDO` :: `PC3_C` is not triggered … why?!?!
#else
  KLST_startISR();
#endif
}

#ifdef __cplusplus
}
#endif
