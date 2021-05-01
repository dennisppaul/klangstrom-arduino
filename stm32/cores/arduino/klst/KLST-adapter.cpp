#include "pins_arduino.h"
#include "Arduino.h"

#include "KLST-adapter.h"
#include "KLST-application.h"


extern void audioblock(float* pOutputLeft, float* pOutputRight, float* pInputLeftRX, float* pInputRight);

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

const uint8_t M_NUM_OF_BITS = 16;
const float M_INT_SCALE = (1 << (M_NUM_OF_BITS-1)) - 1.0; // @todo(see if  is `-1.0` required)
const uint32_t M_MASK_LEFT = (1 << M_NUM_OF_BITS) - 1;
const uint32_t M_MASK_RIGHT = ~(M_MASK_LEFT);

/**
 * fills buffer with new block of samples
 */
WEAK void KLST_fill_buffer(uint32_t *pTXBuffer, uint32_t *pRXBuffer, uint16_t pBufferLength) {
//@todo(reintroduce `SIGNAL_TYPE` aka `float` + `KLANG_SAMPLES_PER_AUDIO_BLOCK` )
  #define KLANG_SAMPLES_PER_AUDIO_BLOCK 512
  /* @todo (check if it is more smart to not instantiate the arrays every time) */
  auto *mLeftTX = new float[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  auto *mRightTX = new float[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  auto *mLeftRX = new float[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  auto *mRightRX = new float[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  /* unpack receive buffer to sample */
  for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
    const uint32_t p = pRXBuffer[i];
    const int16_t mLeftInt  = (p & M_MASK_LEFT);
    const int16_t mRightInt = (p & M_MASK_RIGHT) >> M_NUM_OF_BITS;
    mLeftRX[i] = mLeftInt / M_INT_SCALE;
    mRightRX[i] = mRightInt / M_INT_SCALE;
  }
  /* calculate next audio block */
  audioblock(mLeftTX, mRightTX, mLeftRX, mRightRX);
  /* pack sample to transmit buffer */
  for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
    int16_t mLeftInt = (int16_t) (mLeftTX[i] * M_INT_SCALE);
    int16_t mRightInt = (int16_t) (mRightTX[i] * M_INT_SCALE);
    pTXBuffer[i] = ((uint32_t) (uint16_t) mLeftInt) << 0 | ((uint32_t) (uint16_t) mRightInt) << M_NUM_OF_BITS;

      // //@todo this assumes a 16bit sample depth!
      // int16_t mL = (int16_t) (mLeftTX[i] * 32767.0f);
      // int16_t mR = (int16_t) (mRightTX[i] * 32767.0f);
      // pTXBuffer[i] = ((uint32_t) (uint16_t) mL) << 0 | ((uint32_t) (uint16_t) mR) << 16;
  }
  delete[] mLeftTX;
  delete[] mRightTX;
  delete[] mLeftRX;
  delete[] mRightRX;
}

/*
 * starts the device in DfuSe ( USB Device Firmware Upgrade ) mode.
 * see https://github.com/MichaelTien8901/JmpBldrDemo
 */
void KLST_jump_to_bootloader() {
	uint32_t i = 0;
	void (*SysMemBootJump)(void);

	/* Set the address of the entry point to bootloader */
  volatile uint32_t BootAddr = KLST_boot_address();

	/* Disable all interrupts */
	__disable_irq();

	/* Disable Systick timer */
	SysTick->CTRL = 0;

	/* Set the clock to the default state */
	HAL_RCC_DeInit();

	/* Clear Interrupt Enable Register & Interrupt Pending Register */
	for (i = 0; i < 5; i++) {
		NVIC->ICER[i] = 0xFFFFFFFF;
		NVIC->ICPR[i] = 0xFFFFFFFF;
	}

	/* Re-enable all interrupts */
	__enable_irq();

	/* Set up the jump to booloader address + 4 */
	SysMemBootJump = (void (*)(void)) (*((uint32_t*) ((BootAddr + 4))));

	/* Set the main stack pointer to the bootloader stack */
	__set_MSP(*(uint32_t*) BootAddr);

	/* Call the function to jump to bootloader location */
	SysMemBootJump();
}

#ifdef __cplusplus
}
#endif
