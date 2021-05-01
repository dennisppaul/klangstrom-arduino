#include "stm32f4xx_hal.h"
#include "klangstrom_arduino.h"
#include "klangstrom_peripherals.h"
#include "stm32_def_build.h"
#include "Arduino.h"
#include "AudioCodecWM8731.h"

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------------- */
/* KLST */
/* -------------------------------------------------------------------------------- */

uint32_t mTXBuffer[I2S_BUFFER_SIZE];
uint32_t mRXBuffer[I2S_BUFFER_SIZE];
uint32_t *mCurrentRXBuffer;

#if I2S_USE_DMA

// void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
//   KLST_fillBuffer(&(mTXBuffer[I2S_BUFFER_SIZE >> 1]),
//                   mCurrentRXBuffer,
//                   I2S_BUFFER_SIZE >> 1);
// }
//
// void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
//   KLST_fillBuffer(&(mTXBuffer[0]),
//                   mCurrentRXBuffer,
//                   I2S_BUFFER_SIZE >> 1);
// }
//
// void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
//   mCurrentRXBuffer = &(mRXBuffer[I2S_BUFFER_SIZE >> 1]);
// }
//
// void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
// 	mCurrentRXBuffer = &(mRXBuffer[0]);
// }
//
// void KLST_startDMA(I2S_HandleTypeDef *hi2s) {
//   if (HAL_I2S_Transmit_DMA(hi2s, (uint16_t*) mTXBuffer, I2S_BUFFER_SIZE << 1) != HAL_OK) {
//     Error_Handler();
//   }
//   hi2s2.State = HAL_I2S_STATE_READY; // state flag needs to be cleared manually
//   if (HAL_I2S_Receive_DMA(hi2s, (uint16_t*) mRXBuffer, I2S_BUFFER_SIZE << 1) != HAL_OK) {
//     Error_Handler();
//   }
// }

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
//   MX_I2S2_Init();
  MX_I2C2_Init();

  WM8731_init();
  WM8731_inputSelect(AUDIO_INPUT_MIC);
  WM8731_volumeInteger(110);

  memset(mTXBuffer, 0, sizeof(mTXBuffer));
  memset(mRXBuffer, 0, sizeof(mRXBuffer));
#if I2S_USE_DMA
//   KLST_startDMA(&hi2s2); // - `I2S2_SDO` :: `PC3_C` is not triggered … why?!?!
#else
  KLST_startISR();
#endif
}

#ifdef __cplusplus
}
#endif
