#include "pins_arduino.h"
#include "Arduino.h"

#include "KlangstromDefines.hpp"
#include "KlangstromDefinesArduino.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterfaceArduino.h"

#if !(defined(KLST_BOARD_TYPE) || (KLST_BOARD_TYPE))
#warning("KLST_BOARD_TYPE not defined! options are KLST_CORE or KLST_TINY")
#endif

#define KLST_DISABLE_INTERRUPTS_IN_AUDIOBLOCK
extern void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeftRX, SIGNAL_TYPE* pInputRight);

using namespace klangstrom;

/* --------------------------------------------------------------------------------------------- */
/* --- internal shared (ISH) functions --------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* options */

bool mKLSTOptionEnableBeat = true;
bool mKLSTOptionEnableEncoders = true;
bool mKLSTOptionEnableSerialPorts = true;
bool mKLSTOptionEnableProgrammerButton = true;
bool mKLSTOptionEnableAudioInput = true;
float mKLSTOptionSerial00BaudRate = KLST_UART_BAUD;
float mKLSTOptionSerial01BaudRate = KLST_UART_BAUD;
float mKLSTOptionSerial02BaudRate = KLST_UART_BAUD;
uint8_t mKLSTAudioLine = KLST_MIC;

/* beat */

HardwareTimer *mKLSTBeatTimer;
uint32_t mKLSTBeatCounter = 0;
uint32_t mKLSTBeatIntervalDuration = 1000000/2;

/* encoders */

static int16_t mKLSTENCODER_00TickCount = 0;
static int16_t mKLSTENCODER_01TickCount = 0;
static int16_t mKLSTENCODER_02TickCount = 0;
static bool mKLSTENCODER_00ButtonState = true;
static bool mKLSTENCODER_01ButtonState = true;
static bool mKLSTENCODER_02ButtonState = true;

/* ----------------------------------------------------------------------------------------------------------------- */
/* OPTIONS                                                                                                             */
/* ----------------------------------------------------------------------------------------------------------------- */

uint32_t KLST_ISH_OPT_audio_line() {
  return mKLSTAudioLine;
}

bool KLST_ISH_OPT_audio_input_enabled() {
  return mKLSTOptionEnableAudioInput;
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* SETUP                                                                                                             */
/* ----------------------------------------------------------------------------------------------------------------- */

void KLST_IT_beat_callback() {
    beat(mKLSTBeatCounter++);
}

/**
 * called before setup
 * @note(make sure to remove static from functions!)
 */
void KLST_ISH_pre_setup() {
    KLST_BSP_init_peripherals();

    /* LEDs */
    pinMode(LED_00, OUTPUT);
    pinMode(LED_01, OUTPUT);
    pinMode(LED_02, OUTPUT);

    /* start UART interrupts */
    if (mKLSTOptionEnableSerialPorts) {
#ifdef HAL_UART_MODULE_ENABLED
        KLST_SERIAL_00.begin(mKLSTOptionSerial00BaudRate);
        KLST_SERIAL_01.begin(mKLSTOptionSerial01BaudRate);
#ifdef KLST_SERIAL_02
        KLST_SERIAL_02.begin(mKLSTOptionSerial02BaudRate);
#endif
#else
#warning @Klangstrom :: serial ports are not enabled in board options. ignore this warning if this was intended.
#endif
    }

    /* beat */
    if (mKLSTOptionEnableBeat) {
        mKLSTBeatTimer = new HardwareTimer(KLST_BEAT_TIMER);
        klangstrom::beats_per_minute(120);
        mKLSTBeatTimer->attachInterrupt(KLST_IT_beat_callback);
    }

#ifdef USE_TINYUSB
    KLST_BSP_configure_TinyUSB();
#endif
}

/**
 * called after setup
 */
void KLST_ISH_post_setup() {

    KLST_BSP_configure_audio_codec();
    KLST_BSP_start_audio_codec();

    /* start encoder */
    if (mKLSTOptionEnableEncoders) {
        KLST_BSP_configure_encoders();
        pinMode(ENCODER_00_BUTTON, INPUT);
        pinMode(ENCODER_01_BUTTON, INPUT);
        pinMode(ENCODER_02_BUTTON, INPUT);

        mKLSTENCODER_00TickCount = (int16_t)ENCODER_00_TIMER->CNT;
        mKLSTENCODER_01TickCount = (int16_t)ENCODER_01_TIMER->CNT;
        mKLSTENCODER_02TickCount = (int16_t)ENCODER_02_TIMER->CNT;
    }

    /* beat */
    if (mKLSTOptionEnableBeat) {
        mKLSTBeatTimer->resume();
    }

    /* programmer button */
    if (mKLSTOptionEnableProgrammerButton) {
        pinMode(BUTTON_PROGRAMMER, INPUT);
        attachInterrupt(BUTTON_PROGRAMMER, KLST_ISH_jump_to_bootloader, RISING);
    }
}

void KLST_ISH_loop() {
    if (mKLSTOptionEnableEncoders) {
        KLST_ISH_handle_encoders();
    }
    if (mKLSTOptionEnableSerialPorts) {
        KLST_ISH_handleSerialPorts();
    }
}

void KLST_ISH_handle_encoders() {
    /* rotation */
    const int16_t mEncoder_00TickCount = (int16_t)ENCODER_00_TIMER->CNT;
    if (mKLSTENCODER_00TickCount != mEncoder_00TickCount) {
        const float f[3] = {ENCODER_00, (float)mEncoder_00TickCount, (float)mKLSTENCODER_00TickCount};
        event_receive(EVENT_ENCODER_ROTATE, f);
        mKLSTENCODER_00TickCount = mEncoder_00TickCount;
    }
    const int16_t mEncoder_01TickCount = (int16_t)ENCODER_01_TIMER->CNT;
    if (mKLSTENCODER_01TickCount != mEncoder_01TickCount) {
        const float f[3] = {ENCODER_01, (float)mEncoder_01TickCount, (float)mKLSTENCODER_01TickCount};
        event_receive(EVENT_ENCODER_ROTATE, f);
        mKLSTENCODER_01TickCount = mEncoder_01TickCount;
    }
    const int16_t mEncoder_02TickCount = (int16_t)ENCODER_02_TIMER->CNT;
    if (mKLSTENCODER_02TickCount != mEncoder_02TickCount) {
        const float f[3] = {ENCODER_02, (float)mEncoder_02TickCount, (float)mKLSTENCODER_02TickCount};
        event_receive(EVENT_ENCODER_ROTATE, f);
        mKLSTENCODER_02TickCount = mEncoder_02TickCount;
    }
    /* buttons */
    bool mENCODER_00ButtonState = digitalRead(ENCODER_00_BUTTON);
    if (mKLSTENCODER_00ButtonState != mENCODER_00ButtonState) {
        const float f[1] = {ENCODER_00};
        if (mENCODER_00ButtonState) {
            event_receive(EVENT_ENCODER_BUTTON_RELEASED, f);
        } else {
            event_receive(EVENT_ENCODER_BUTTON_PRESSED, f);
        }
        mKLSTENCODER_00ButtonState = mENCODER_00ButtonState;
    }
    bool mENCODER_01ButtonState = digitalRead(ENCODER_01_BUTTON);
    if (mKLSTENCODER_01ButtonState != mENCODER_01ButtonState) {
        const float f[1] = {ENCODER_01};
        if (mENCODER_01ButtonState) {
            event_receive(EVENT_ENCODER_BUTTON_RELEASED, f);
        } else {
            event_receive(EVENT_ENCODER_BUTTON_PRESSED, f);
        }
        mKLSTENCODER_01ButtonState = mENCODER_01ButtonState;
    }
    bool mENCODER_02ButtonState = digitalRead(ENCODER_02_BUTTON);
    if (mKLSTENCODER_02ButtonState != mENCODER_02ButtonState) {
        const float f[1] = {ENCODER_02};
        if (mENCODER_02ButtonState) {
            event_receive(EVENT_ENCODER_BUTTON_RELEASED, f);
        } else {
            event_receive(EVENT_ENCODER_BUTTON_PRESSED, f);
        }
        mKLSTENCODER_02ButtonState = mENCODER_02ButtonState;
    }
}

void KLST_ISH_handleSerialPorts() {
#ifdef HAL_UART_MODULE_ENABLED
    {
        const uint8_t mLength = KLST_SERIAL_00.available();
        if (mLength > 0) {
            uint8_t mData[mLength];
            KLST_SERIAL_00.readBytes(mData, mLength);
            data_receive(SERIAL_00, mData, mLength);
        }
    }
    {
        const uint8_t mLength = KLST_SERIAL_01.available();
        if (mLength > 0) {
            uint8_t mData[mLength];
            KLST_SERIAL_01.readBytes(mData, mLength);
            data_receive(SERIAL_01, mData, mLength);
        }
    }
#ifdef KLST_SERIAL_02
    {
        const uint8_t mLength = KLST_SERIAL_02.available();
        if (mLength > 0) {
            uint8_t mData[mLength];
            KLST_SERIAL_02.readBytes(mData, mLength);
            data_receive(SERIAL_02, mData, mLength);
        }
    }
#endif
#endif
}

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
WEAK void KLST_ISH_fill_buffer(uint32_t *pTXBuffer, uint32_t *pRXBuffer, uint16_t pBufferLength) {
#ifdef KLST_DISABLE_INTERRUPTS_IN_AUDIOBLOCK
  noInterrupts();
#endif
  /* unpack receive buffer to sample */
  if (KLST_ISH_OPT_audio_input_enabled()) {
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
#ifdef KLST_DISABLE_INTERRUPTS_IN_AUDIOBLOCK
  interrupts();
#endif
}

/*
 * starts the device in DfuSe ( USB Device Firmware Upgrade ) mode.
 * see https://github.com/MichaelTien8901/JmpBldrDemo
 */
void KLST_ISH_jump_to_bootloader() {
  KLST_ISH_shutdown();

	uint32_t i = 0;
	void (*SysMemBootJump)(void);

	/* Set the address of the entry point to bootloader */
  volatile uint32_t BootAddr = KLST_BSP_boot_address();

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

void KLST_shutdown_toggle_leds(const uint16_t pDelay) {
  delay(pDelay);
  digitalWrite(LED_00, !digitalRead(LED_00));
  digitalWrite(LED_01, !digitalRead(LED_01));
  digitalWrite(LED_02, !digitalRead(LED_02));
}

void KLST_ISH_shutdown() {
  KLST_BSP_shutdown();
  //@todo(check if other peripherals also need to be reset e.g GPIOs, SPI, I2C. see https://github.com/viktorvano/STM32-Bootloader/blob/master/README.md )
  /* stop beat */
  mKLSTBeatTimer->pause();
  /* stop USB */
  #if defined (USBCON) && defined(USBD_USE_CDC)
  SerialUSB.end();
  #endif
  /* flash LEDs */
  digitalWrite(LED_00, LOW);
  digitalWrite(LED_01, HIGH);
  digitalWrite(LED_02, LOW);
  for (uint8_t i=16; i > 0; i--) {
    KLST_shutdown_toggle_leds(24 + 12 * i);
  }
}

unsigned long KLST_get_U_ID(uint8_t pOffset) {
    static const uint32_t UID_ADDR = KLST_BSP_U_ID_address();
    return *((unsigned long *) UID_ADDR + pOffset * 0x04);
}

bool KLST_check_UID(const uint32_t pUID[]) {
  return  pUID[0] == KLST_get_U_ID(0) &&
          pUID[1] == KLST_get_U_ID(1) &&
          pUID[2] == KLST_get_U_ID(2);
}

#ifdef __cplusplus
}
#endif

/* --------------------------------------------------------------------------------------------- */
/* --- application-level functions ------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

#ifdef __cplusplus

int16_t klangstrom::ID() {
  for (uint8_t i = 0; i < KLST_NUM_OF_U_ID; ++i) {
      if (KLST_check_UID(KLST_U_ID[i])) {
          return i;
      }
  }
  return KLST_NO_ID;
}

void klangstrom::begin_serial_debug(bool pWaitForSerial, uint32_t pBaudRate) {
#if defined (USBCON) && defined(USBD_USE_CDC)
        Serial.begin(pBaudRate);
        if (pWaitForSerial) {
            static const uint32_t M_ITERATION_GUARD = 180000000;
            uint32_t mIterationGuard = 0;
            while (!Serial && mIterationGuard < M_ITERATION_GUARD) {
                // wait for serial port to connect. needed for native USB …
                // but only for a limited number of iterations.
                mIterationGuard++;
            }
        }
#endif
}

void klangstrom::option(uint8_t pOption, uint32_t pValue) {
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
        case KLST_OPTION_PROGRAMMER_BUTTON:
            mKLSTOptionEnableProgrammerButton = pValue;
            break;
        case KLST_OPTION_ENABLE_AUDIO_INPUT:
            mKLSTOptionEnableAudioInput = pValue;
            break;
        case KLST_OPTION_SERIAL_00_BAUD_RATE:
            mKLSTOptionSerial00BaudRate = pValue;
            break;
        case KLST_OPTION_SERIAL_01_BAUD_RATE:
            mKLSTOptionSerial01BaudRate = pValue;
            break;
        case KLST_OPTION_SERIAL_02_BAUD_RATE:
            mKLSTOptionSerial02BaudRate = pValue;
            break;
    }
}

void klangstrom::led(uint8_t pLED, bool pState) {
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

void klangstrom::led_toggle(uint8_t pLED) {
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

void klangstrom::beats_per_minute(float pBPM) {
    if (pBPM == 0) { return; }
    klangstrom::beats_per_minute_ms((uint32_t)((60.0 / pBPM) * 1000000));
}

void klangstrom::beats_per_minute_ms(uint32_t pMicroSeconds) {
    mKLSTBeatIntervalDuration = pMicroSeconds;
    mKLSTBeatTimer->setOverflow(mKLSTBeatIntervalDuration, MICROSEC_FORMAT);
}

bool klangstrom::button_state(uint8_t pButton) {
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

bool klangstrom::pin_state(uint8_t pButton) {
    return !digitalRead(pButton);
}

void klangstrom::data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength) {
  switch(pSender) {
#ifdef HAL_UART_MODULE_ENABLED
#ifdef KLST_SERIAL_00
    case SERIAL_00:
      KLST_SERIAL_00.write(pData, pDataLength);
      break;
#endif
#ifdef KLST_SERIAL_01
    case SERIAL_01:
      KLST_SERIAL_01.write(pData, pDataLength);
      break;
#endif
#ifdef KLST_SERIAL_02
      case SERIAL_02:
      KLST_SERIAL_02.write(pData, pDataLength);
      break;
#endif
#endif
  }
}

#endif // __cplusplus
