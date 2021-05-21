#include "pins_arduino.h"
#include "Arduino.h"

#include "KlangstromDefinesArduino.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterfaceArduino.h"

extern void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeftRX, SIGNAL_TYPE* pInputRight);

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
SIGNAL_TYPE mLeftTX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
SIGNAL_TYPE mRightTX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
SIGNAL_TYPE mLeftRX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
SIGNAL_TYPE mRightRX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
WEAK void KLST_fill_buffer(uint32_t *pTXBuffer, uint32_t *pRXBuffer, uint16_t pBufferLength) {
  /* unpack receive buffer to sample */
  if (KLST_audio_input_enabled()) {
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
      const uint32_t p = pRXBuffer[i];
      const int16_t mLeftInt  = (p & M_MASK_LEFT);
      const int16_t mRightInt = (p & M_MASK_RIGHT) >> M_NUM_OF_BITS;
      mLeftRX[i] = mLeftInt / M_INT_SCALE;
      mRightRX[i] = mRightInt / M_INT_SCALE;
    }
  }
  /* calculate next audio block */
  audioblock(mLeftTX, mRightTX, mLeftRX, mRightRX);
  /* pack sample for transmit buffer */
  for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
    int16_t mLeftInt = (int16_t) (mLeftTX[i] * M_INT_SCALE);
    int16_t mRightInt = (int16_t) (mRightTX[i] * M_INT_SCALE);
    pTXBuffer[i] = ((uint32_t) (uint16_t) mLeftInt) << 0 | ((uint32_t) (uint16_t) mRightInt) << M_NUM_OF_BITS;
      // //@todo this assumes a 16bit sample depth!
      // int16_t mL = (int16_t) (mLeftTX[i] * 32767.0f);
      // int16_t mR = (int16_t) (mRightTX[i] * 32767.0f);
      // pTXBuffer[i] = ((uint32_t) (uint16_t) mL) << 0 | ((uint32_t) (uint16_t) mR) << 16;
  }
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

  KLST_shutdown();
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

unsigned long KLST_get_U_ID(uint8_t pOffset) {
    static const uint32_t UID_ADDR = KLST_U_ID_address();
    return *((unsigned long *) UID_ADDR + pOffset * 0x04);
}

bool KLST_check_UID(const uint32_t pUID[]) {
    return  pUID[0] == KLST_get_U_ID(0) &&
            pUID[1] == KLST_get_U_ID(1) &&
            pUID[2] == KLST_get_U_ID(2);
}

int16_t klangstrom::ID() {
  for (uint8_t i = 0; i < KLST_NUM_OF_U_ID; ++i) {
      if (KLST_check_UID(KLST_U_ID[i])) {
          return i;
      }
  }
  return KLST_NO_ID;
}
