//
//  KLST_TINY-adapter.cpp
//  Klangstrom – a node+text-based synthesizer library
//
//
//

/**
 * @todo(there is a reoccuring problem with HAL callback functions being used in e.g `libraries/SrcWrapper/stm32/uart.c` and `libraries/SrcWrapper/stm32/timer.c`(?) )
 */

#include "pins_arduino.h"
#include "Arduino.h"
#include "KLST-adapter.h"
#include "KLST-application.h"

/* options */

bool mKLSTOptionEnableBeat = true;
bool mKLSTOptionEnableEncoders = true;
bool mKLSTOptionEnableSerialPorts = true;
bool mKLSTOptionEnableProgrammerButton = true;
bool mKLSTOptionEnableUSBSerialDebug = false;
uint8_t mKLSTAudioLine = KLST_MIC;

/* beat */

HardwareTimer *mKLSTBeatTimer;
uint32_t mKLSTBeatCounter = 0;
uint32_t mKLSTBeatIntervalDuration = 1000000/2;

/* encoders */

static int16_t mKLSTENCODER_00TickCount = 0;
static int16_t mKLSTENCODER_01TickCount = 0;
static int16_t mKLSTENCODER_02TickCount = 0;
static bool mKLSTENCODER_00ButtonState = false;
static bool mKLSTENCODER_01ButtonState = false;
static bool mKLSTENCODER_02ButtonState = false;
static const float mKLST_PRESSED[1] = {1.0f};
static const float mKLST_RELEASED[1] = {0.0f};

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "AudioCodecWM8731.h"

#if !(defined(KLST_BOARD_TYPE) || (KLST_BOARD_TYPE))
#warning("KLST_BOARD_TYPE not defined in `variant_KLST.h`. options are KLST_CORE or KLST_TINY")
#endif

extern I2C_HandleTypeDef    hi2c3;
extern SAI_HandleTypeDef    hsai_BlockA1;
extern SAI_HandleTypeDef    hsai_BlockB1;
extern DMA_HandleTypeDef    hdma_sai1_b;
extern DMA_HandleTypeDef    hdma_sai1_a;
extern TIM_HandleTypeDef 	htim2;
extern TIM_HandleTypeDef 	htim3;
extern TIM_HandleTypeDef 	htim8;

void MX_GPIO_Init();
void MX_DMA_Init();
void MX_I2C3_Init();
void MX_SAI1_Init();
void MX_TIM2_Init();
void MX_TIM3_Init();
void MX_TIM8_Init();

/* ----------------------------------------------------------------------------------------------------------------- */
/* SAI + AUDIO CODEC                                                                                                 */
/* ----------------------------------------------------------------------------------------------------------------- */

/**
 * SAI callback implementation
 */

#define	I2S_BUFFER_SIZE		(KLANG_SAMPLES_PER_AUDIO_BLOCK*2)
uint32_t __attribute__((section (".dmadata"))) dma_TX_buffer[I2S_BUFFER_SIZE];
uint32_t __attribute__((section (".dmadata"))) dma_RX_buffer[I2S_BUFFER_SIZE];
uint32_t *mCurrentRXBuffer;

void KLST_start_audio_codec() {
	memset(dma_TX_buffer, 0, sizeof(dma_TX_buffer));
	memset(dma_RX_buffer, 0, sizeof(dma_RX_buffer));
	if(HAL_OK != HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t*) dma_TX_buffer, I2S_BUFFER_SIZE << 1)) {
// 		KLST_LOG("### ERROR initializing SAI TX");
	}
	if(HAL_OK != HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*) dma_RX_buffer, I2S_BUFFER_SIZE << 1)) {
// 		KLST_LOG("### ERROR initializing SAI RX");
	}
	mCurrentRXBuffer = &(dma_RX_buffer[0]);
}

#define SANITY_TEST             0
#define SANITY_TEST_PASSTHROUGH 0

#if SANITY_TEST
float osc_phi = 0;
float osc_phi_inc = 220.0f / 48000.0f; // generating 440Hz

void FillBuffer(uint32_t *mTXBuffer, uint32_t *mRXBuffer, uint16_t len) {
	for (uint16_t i = 0; i < len; i++) {
#if SANITY_TEST_PASSTHROUGH
		mTXBuffer[i] = mRXBuffer[i];
#else
		float a;
		int16_t y;
		a = (float) sin(osc_phi * 6.2832f) * 0.20f;
		osc_phi += osc_phi_inc;
		osc_phi -= (float) ((uint16_t) osc_phi);

		y = (int16_t) (a * 32767.0f);

		// both channels
		mTXBuffer[i] = ((uint32_t) (uint16_t) y) << 0 | ((uint32_t) (uint16_t) y) << 16;
#endif
	}
}
#endif

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai) {
#if SANITY_TEST
	FillBuffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
	KLST_fill_buffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
#if SANITY_TEST
	FillBuffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
	KLST_fill_buffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
	mCurrentRXBuffer = &(dma_RX_buffer[I2S_BUFFER_SIZE >> 1]);
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
	mCurrentRXBuffer = &(dma_RX_buffer[0]);
}

/**
 * WM8731 I2C implementation
 */
bool WM8731_I2C_write(uint8_t device_address, uint8_t *data, uint8_t length) {
	HAL_StatusTypeDef mResult = HAL_I2C_Master_Transmit(&hi2c3, device_address, data, length, 10);
	if (mResult != HAL_OK) {
// 		KLST_LOG("--- ERROR @ I2C_TX_write: %i", mResult);
    digitalWrite(LED_00, HIGH);
		return false;
	}
	return true;
}

/**
 * WM8731 delay implementation
 */
void WM8731_delay(uint32_t pDelay) {
  delay(pDelay);
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* UARTs                                                                                                             */
/* ----------------------------------------------------------------------------------------------------------------- */

// #define KLST_SERIAL_BUFFER_SIZE	1
// uint8_t mSERIAL_00_BUFFER[KLST_SERIAL_BUFFER_SIZE] = { 0 };
// uint8_t mSERIAL_00_TX_BUFFER[KLST_SERIAL_BUFFER_SIZE] = { 0 };
// uint8_t mSERIAL_01_BUFFER[KLST_SERIAL_BUFFER_SIZE] = { 0 };
// uint8_t mSERIAL_01_TX_BUFFER[KLST_SERIAL_BUFFER_SIZE] = { 0 };
//
// /**
//  * called from `HAL_UART_RxCpltCallback`
//  */
// void KLST_RxCpltCallback(UART_HandleTypeDef *huart) {
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
// 	if (huart == SERIAL_00_Handle) {
// 		data_receive(KLST_SENDER_SERIAL_00, mSERIAL_00_BUFFER, KLST_SERIAL_BUFFER_SIZE);
// 		HAL_UART_Receive_IT(SERIAL_00_Handle, mSERIAL_00_BUFFER, KLST_SERIAL_BUFFER_SIZE);
// 	} else if (huart == SERIAL_01_Handle) {
// 		data_receive(KLST_SENDER_SERIAL_01, mSERIAL_01_BUFFER, KLST_SERIAL_BUFFER_SIZE);
// 		HAL_UART_Receive_IT(SERIAL_01_Handle, mSERIAL_01_BUFFER, KLST_SERIAL_BUFFER_SIZE);
// 	}
// }

/* ----------------------------------------------------------------------------------------------------------------- */
/* BEAT                                                                                                             */
/* ----------------------------------------------------------------------------------------------------------------- */

void KLST_IT_beat_callback() {
	beat(mKLSTBeatCounter++);
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* SETUP                                                                                                             */
/* ----------------------------------------------------------------------------------------------------------------- */

/**
 * called before setup
 * @note(make sure to remove static from functions!)
 */
void KLST_pre_setup() {
	/*
	+  MX_GPIO_Init();
	+  MX_DMA_Init();
	MX_ADC1_Init();
	MX_DAC_Init();
	MX_I2C1_Init();
	+  MX_SAI1_Init();
	MX_SPI2_Init();
	+  MX_TIM2_Init();
	MX_TIM6_Init();
	MX_TIM7_Init();
	MX_TIM10_Init();
	MX_TIM11_Init();
	+  MX_TIM3_Init();
	+  MX_TIM8_Init();
	MX_USB_DEVICE_Init();
	+  MX_I2C3_Init();
	MX_TIM13_Init();
	MX_TIM14_Init();
	MX_TIM1_Init();
	MX_TIM4_Init();
	MX_TIM5_Init();
	MX_TIM9_Init();
	MX_TIM12_Init();
	*/
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C3_Init(); // @todo(check if we could use the built-in libs for this?)
	MX_SAI1_Init();

	/* LEDs */
	pinMode(LED_00, OUTPUT);
	pinMode(LED_01, OUTPUT);
	pinMode(LED_02, OUTPUT);

	/* start UART interrupts */
	// HAL_UART_Receive_IT(SERIAL_00_Handle, mSERIAL_00_BUFFER, KLST_SERIAL_BUFFER_SIZE);
	// HAL_UART_Receive_IT(SERIAL_01_Handle, mSERIAL_01_BUFFER, KLST_SERIAL_BUFFER_SIZE);
	if (mKLSTOptionEnableSerialPorts) {
		SERIAL_00.begin(KLST_UART_BAUD);
		SERIAL_01.begin(KLST_UART_BAUD);
	}

  	/* beat */
	if (mKLSTOptionEnableBeat) {
		mKLSTBeatTimer = new HardwareTimer(TIM5);
		klst::beats_per_minute(120);
		mKLSTBeatTimer->attachInterrupt(KLST_IT_beat_callback);
	}

	/* debug serial via USB */
	// @todo(does it make sense to evaluate it here? or rather in `KLST_post_setup`)
	if (mKLSTOptionEnableUSBSerialDebug) {
		Serial.begin(KLST_UART_BAUD);
		uint32_t mIterationGuard = 0;
		while (!Serial && mIterationGuard < 1000000) {
			// wait for serial port to connect. needed for native USB …
			// but only for a limited number of iterations.
		  mIterationGuard++;
		}
	}
}

/**
 * called after setup
 */
void KLST_post_setup() {
	/*
	* POWER UP SEQUENCE
	* - Switch on power supplies. By default the WM8731 is in Standby Mode, the DAC is digitally muted and the Audio Interface and Outputs are all OFF.
	* - Set all required bits in the Power Down reg          ister (0Ch) to ‘0’; EXCEPT the OUTPD bit, this should be set to ‘1’ (Default).
	* - Set required values in all other registers except 12h (Active).
	* - Set the ‘Active’ bit in register 12h.
	* - The last write of the sequence should be setting OUTPD to ‘0’ (active) in register 0Ch, enabling the DAC signal path, free of any significant power-up noise.
	*/
	//	WM8731_write(WM8731_RESET_REGISTER, 0b00000000);
	WM8731_write(WM8731_POWER_DOWN_CONTROL, 0b00010000);
	WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00000000);
	WM8731_write(WM8731_LINE_IN_LEFT,  0b000011111); // disable simultaneous load, disable mute, (+12db)
	WM8731_write(WM8731_LINE_IN_RIGHT, 0b000011111);
	WM8731_write(WM8731_HEADPHONE_OUT_LEFT,  0b001111001);
	WM8731_write(WM8731_HEADPHONE_OUT_RIGHT, 0b001111001);
	if (mKLSTAudioLine == KLST_MIC) {
		WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00010100); // MIC
	} else if (mKLSTAudioLine == KLST_LINE_IN) {
		WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00010010); // LINE_IN
	}
	WM8731_inputLevel(0x1F);
	WM8731_write(WM8731_DIGITAL_AUDIO_PATH_CONTROL, 0b00111);
	WM8731_write(WM8731_DIGITAL_AUDIO_INTERFACE_FORMAT, 0b00000010);
	WM8731_write(WM8731_SAMPLING_CONTROL, 0b000000010);
	WM8731_write(WM8731_ACTIVE_CONTROL, 0b1);

	WM8731_write(WM8731_POWER_DOWN_CONTROL, 0b00000000);

	KLST_start_audio_codec();

	/* start encoder */
	if (mKLSTOptionEnableEncoders) {
		MX_TIM2_Init();
		MX_TIM3_Init();
		MX_TIM8_Init();

		pinMode(ENCODER_00_BUTTON, INPUT);
  		pinMode(ENCODER_01_BUTTON, INPUT);
  		pinMode(ENCODER_02_BUTTON, INPUT);

		HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
		// @note(`HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);`)
	}

	/* beat */
	if (mKLSTOptionEnableBeat) {
		mKLSTBeatTimer->resume();
	}

  /* programmer button */
  if (mKLSTOptionEnableProgrammerButton) {
    pinMode(BUTTON_PROGRAMMER, INPUT);
    attachInterrupt(BUTTON_PROGRAMMER, KLST_jump_to_bootloader, RISING);
	}
}

uint32_t KLST_boot_address() {
  return 0x1FFF0000; // boot address for KLST_TINY
//   return 0x1FF09800; // boot address for KLST_CORE
}

void KLST_shutdown_toggle_leds(const uint16_t pDelay) {
  delay(pDelay);
  digitalWrite(LED_00, !digitalRead(LED_00));
  digitalWrite(LED_01, !digitalRead(LED_01));
  digitalWrite(LED_02, !digitalRead(LED_02));
}

void KLST_shutdown() {
  /* stop timers */
  HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Stop(&htim8, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);
  mKLSTBeatTimer->pause();
  /* stop audio */
  HAL_SAI_DMAStop(&hsai_BlockB1);
  HAL_SAI_DMAStop(&hsai_BlockA1);
  /* flash LEDs */
  digitalWrite(LED_00, LOW);
  digitalWrite(LED_01, HIGH);
  digitalWrite(LED_02, LOW);
  for (uint8_t i=16; i > 0; i--) {
    KLST_shutdown_toggle_leds(24 + 12 * i);
  }
}

void KLST_handle_encoders() {
	/* rotation */
	const int16_t mEncoder_00TickCount = (int16_t)ENCODER_00_TIMER->CNT;
	if (mKLSTENCODER_00TickCount != mEncoder_00TickCount) {
		const float f[2] = {(float)mEncoder_00TickCount, (float)mKLSTENCODER_00TickCount};
		event_receive(KLST_EVENT_ENCODER_00, f);
		mKLSTENCODER_00TickCount = mEncoder_00TickCount;
	}
	const int16_t mEncoder_01TickCount = (int16_t)ENCODER_01_TIMER->CNT;
	if (mKLSTENCODER_01TickCount != mEncoder_01TickCount) {
		const float f[2] = {(float)mEncoder_01TickCount, (float)mKLSTENCODER_01TickCount};
		event_receive(KLST_EVENT_ENCODER_01, f);
		mKLSTENCODER_01TickCount = mEncoder_01TickCount;
	}
	const int16_t mEncoder_02TickCount = (int16_t)ENCODER_02_TIMER->CNT;
	if (mKLSTENCODER_02TickCount != mEncoder_02TickCount) {
		const float f[2] = {(float)mEncoder_02TickCount, (float)mKLSTENCODER_02TickCount};
		event_receive(KLST_EVENT_ENCODER_02, f);
		mKLSTENCODER_02TickCount = mEncoder_02TickCount;
	}
	/* buttons */
	bool mENCODER_00ButtonState = digitalRead(ENCODER_00_BUTTON);
	if (mKLSTENCODER_00ButtonState != mENCODER_00ButtonState) {
		if (mENCODER_00ButtonState) {
			event_receive(KLST_EVENT_ENCODER_BUTTON_00, mKLST_RELEASED);
		} else {
			event_receive(KLST_EVENT_ENCODER_BUTTON_00, mKLST_PRESSED);
		}
		mKLSTENCODER_00ButtonState = mENCODER_00ButtonState;
	}
	bool mENCODER_01ButtonState = digitalRead(ENCODER_01_BUTTON);
	if (mKLSTENCODER_01ButtonState != mENCODER_01ButtonState) {
		if (mENCODER_01ButtonState) {
			event_receive(KLST_EVENT_ENCODER_BUTTON_01, mKLST_RELEASED);
		} else {
			event_receive(KLST_EVENT_ENCODER_BUTTON_01, mKLST_PRESSED);
		}
		mKLSTENCODER_01ButtonState = mENCODER_01ButtonState;
	}
	bool mENCODER_02ButtonState = digitalRead(ENCODER_02_BUTTON);
	if (mKLSTENCODER_02ButtonState != mENCODER_02ButtonState) {
		if (mENCODER_02ButtonState) {
			event_receive(KLST_EVENT_ENCODER_BUTTON_02, mKLST_RELEASED);
		} else {
			event_receive(KLST_EVENT_ENCODER_BUTTON_02, mKLST_PRESSED);
		}
		mKLSTENCODER_02ButtonState = mENCODER_02ButtonState;
	}
}

void KLST_handleSerialPorts() {
	{
		const uint8_t mLength = SERIAL_00.available();
		if (mLength > 0) {
			uint8_t mData[mLength];
			SERIAL_00.readBytes(mData, mLength);
			data_receive(KLST_SENDER_SERIAL_00, mData, mLength);
		}
	}
	{
		const uint8_t mLength = SERIAL_01.available();
		if (mLength > 0) {
			uint8_t mData[mLength];
			SERIAL_01.readBytes(mData, mLength);
			data_receive(KLST_SENDER_SERIAL_01, mData, mLength);
		}
	}
}

void KLST_loop() {
	if (mKLSTOptionEnableEncoders) {
		KLST_handle_encoders();
	}
	if (mKLSTOptionEnableSerialPorts) {
		KLST_handleSerialPorts();
	}
}

#ifdef __cplusplus
}
#endif

/* ----------------------------------------------------------------------------------------------------------------- */
/* arduino sketch related functions ( i.e callbacks )                                                                */
/* ----------------------------------------------------------------------------------------------------------------- */

#ifdef __cplusplus

/**
 * SERIAL_00 callback
 */
// void serialEvent1() {
// 	klst::led(LED_01, true);
// 	while (Serial1.available()) {
//       	uint8_t mValue = Serial1.read();
// 		data_receive(KLST_SENDER_SERIAL_00, &mValue, 1);
//     }
// }

/**
 * SERIAL_01 callback
 */
// void serialEvent4() {
// 	klst::led(LED_02, true);
// 	while (Serial4.available()) {
//       	uint8_t mValue = Serial4.read();
// 		data_receive(KLST_SENDER_SERIAL_01, &mValue, 1);
//     }
// }

void klst::option(uint8_t pOption, uint8_t pValue) {
	switch (pOption) {
		case KLST_OPTION_AUDIO_INPUT:
			mKLSTAudioLine = pValue;
			break;
		case KLST_OPTION_ENCODERS:
			mKLSTOptionEnableEncoders = pValue;
			break;
		case KLST_OPTION_SERIAL_PORTS:
			mKLSTOptionEnableSerialPorts = pValue;
			break;
		case KLST_OPTION_BEAT:
			mKLSTOptionEnableBeat = pValue;
			break;
		case KLST_OPTION_PROGRAMMMER_BUTTON:
			mKLSTOptionEnableProgrammerButton = pValue;
			break;
	}
}

void klst::led(uint8_t pLED, bool pState) {
	// @todo(maybe replace with `KLST_LED_00`)
	switch (pLED) {
		case LED_00:
			digitalWrite(LED_00, pState ? HIGH : LOW);
			break;
		case LED_01:
			digitalWrite(LED_01, pState ? HIGH : LOW);
			break;
		case LED_02:
			digitalWrite(LED_02, pState ? HIGH : LOW);
			break;
	}
}

void klst::led_toggle(uint8_t pLED) {
	// @todo(maybe replace with `KLST_LED_00`)
	switch (pLED) {
		case LED_00:
			digitalWrite(LED_00, !digitalRead(LED_00));
			break;
		case LED_01:
			digitalWrite(LED_01, !digitalRead(LED_01));
			break;
		case LED_02:
			digitalWrite(LED_02, !digitalRead(LED_02));
			break;
	}
}

void klst::beats_per_minute(float pBPM) {
	if (pBPM == 0) { return; }
	klst::beats_per_minute_ms((uint32_t)((60.0 / pBPM) * 1000000));
}

void klst::beats_per_minute_ms(uint32_t pMicroSeconds) {
	mKLSTBeatIntervalDuration = pMicroSeconds;
	mKLSTBeatTimer->setOverflow(mKLSTBeatIntervalDuration, MICROSEC_FORMAT);
}

bool klst::button_state(uint8_t pButton) {
	switch (pButton) {
		case KLST_BUTTON_ENCODER_00:
			return !mKLSTENCODER_00ButtonState;
		case KLST_BUTTON_ENCODER_01:
			return !mKLSTENCODER_01ButtonState;
		case KLST_BUTTON_ENCODER_02:
			return !mKLSTENCODER_02ButtonState;
		case KLST_BUTTON_PROGRAMMER:
			return !digitalRead(BUTTON_PROGRAMMER);
	}
	return false;
}

bool klst::pin_state(uint8_t pButton) {
	return !digitalRead(pButton);
}

void data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength) {
  switch(pSender) {
    case KLST_RECEIVER_SERIAL_00:
      SERIAL_00.write(pData, pDataLength);
      break;
    case KLST_RECEIVER_SERIAL_01:
      SERIAL_01.write(pData, pDataLength);
      break;
  }
}


#endif // __cplusplus

/* ------------------------------------------------------------------------------------ */
//
// #if 0
//
// #if I2S_BUFFER_SIZE > 16
// #warning BUFFER_SIZE > 16 might cause problems.
// #endif
//
// #include "klangstrom_ISR.h"
//
// uint16_t mBufferPointer = 0;
// bool mScheduleHalfBufferUpdate = false;
// bool mScheduleCptlBufferUpdate = false;
//
// #define __I2S_TIMEOUT             100
// #define __I2S_DIRECT_BUFFER_SIZE  2
//
// void KLST_I2S_transmit(uint16_t pLeft, uint16_t pRight) {
//   uint16_t mBuffer[__I2S_DIRECT_BUFFER_SIZE];
//   mBuffer[0] = pLeft;
//   mBuffer[1] = pRight;
//   HAL_I2S_Transmit(&hi2s2, mBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
// }
//
// void KLST_I2S_receive(uint16_t* pLeft, uint16_t* pRight) {
// 	uint16_t mBuffer[__I2S_DIRECT_BUFFER_SIZE];
// 	HAL_I2S_Receive(&hi2s2, mBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
// 	*pLeft = mBuffer[0];
// 	*pRight = mBuffer[1];
// }
//
// WEAK void KLST_audio_interrupt() {
//   /* update buffer */
//   if (mBufferPointer == I2S_BUFFER_SIZE/2) {
//     // void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
//     KLST_fill_buffer(&(mTXBuffer[0]),
//                     &(mRXBuffer[0]),
//                     I2S_BUFFER_SIZE >> 1);
//   } else if (mBufferPointer >= I2S_BUFFER_SIZE) {
//     // void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
//     KLST_fill_buffer(&(mTXBuffer[I2S_BUFFER_SIZE >> 1]),
//                     &(mRXBuffer[I2S_BUFFER_SIZE >> 1]),
//                     I2S_BUFFER_SIZE >> 1);
//     mBufferPointer = 0;
//   }
//   /* retreive sample from I2S */
//   uint16_t mTmpRXBuffer[__I2S_DIRECT_BUFFER_SIZE];
//  	HAL_I2S_Receive(&hi2s2, mTmpRXBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
//   mRXBuffer[mBufferPointer] = ((uint32_t) (uint16_t) mTmpRXBuffer[0]) << 0 | ((uint32_t) (uint16_t) mTmpRXBuffer[0]) << 16;
//   /* send sample to I2S */
//   uint16_t *mTmpTXBuffer = (uint16_t*)&mTXBuffer[mBufferPointer];
//   HAL_I2S_Transmit(&hi2s2, mTmpTXBuffer, __I2S_DIRECT_BUFFER_SIZE, __I2S_TIMEOUT);
//   mBufferPointer++;
// }
//
// #endif
