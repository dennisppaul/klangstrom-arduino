#include "Arduino.h"
#include "klangstrom_arduino.h"
#include "klangstrom_peripherals.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * starts the device in DfuSe ( USB Device Firmware Upgrade ) mode
 */
void KLST_JumpToBootloader(void) {
	uint32_t i = 0;
	void (*SysMemBootJump)(void);

	/* Set the address of the entry point to bootloader */
	volatile uint32_t BootAddr = 0x1FF09800;

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

	/* Jump is done successfully */
	while (1) {
		/* Code should never reach this loop */
	}
}

WEAK void setup() {}
WEAK void loop() {}

#ifdef __cplusplus
}
#endif

WEAK void beat(uint32_t pBeat) {}
WEAK void audioblock(SIGNAL_TYPE* pLeftTX, SIGNAL_TYPE* pRightTX, SIGNAL_TYPE* pLeftRX, SIGNAL_TYPE* pRightRX) {}
WEAK void event_receive(const EVENT_TYPE event, const float* data) {}
WEAK void data_receive(uint8_t* data, uint8_t length) {}

WEAK void KLST_fillBuffer(uint32_t *pTXBuffer, uint32_t *pRXBuffer, uint16_t pBufferLength) {
  auto *mLeftTX = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  auto *mRightTX = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  auto *mLeftRX = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  auto *mRightRX = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
  for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
    // @todo(transfer RX to float)
  }
  audioblock(mLeftTX, mRightTX, mLeftRX, mRightRX);
  for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
      int16_t mL = (int16_t) (mLeftTX[i] * 32767.0f);
      int16_t mR = (int16_t) (mRightTX[i] * 32767.0f);
      pTXBuffer[i] = ((uint32_t) (uint16_t) mL) << 0 | ((uint32_t) (uint16_t) mR) << 16;
  }
  delete[] mLeftTX;
  delete[] mRightTX;
  delete[] mLeftRX;
  delete[] mRightRX;
}

