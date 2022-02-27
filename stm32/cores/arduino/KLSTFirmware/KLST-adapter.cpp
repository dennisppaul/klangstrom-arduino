/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Arduino.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterfaceArduino.h"
#include "KlangstromDefines.hpp"
#include "KlangstromDefinesArduino.h"
#include "pins_arduino.h"

// @todo(find a way to make library headers available in this context … e.g in `platform.txt`)
// #if defined(TINYUSB_MIDI_DEVICE) || defined(TINYUSB_MOUSE_KEYBOARD_DEVICE)
// #if !(__has_include("USBDevice.h"))
// #warning "could not find USB Device library. make sure to include `#include "USBDevice.h"` at beginning of sketch."
// #endif
// #endif

#if (!defined(KLST_BOARD_KLST_TINY) && !defined(KLST_BOARD_KLST_CORE) && !defined(KLST_BOARD_KLST_SHEEP))
#warning("@KLST board type not defined! options are KLST_BOARD_KLST_CORE or KLST_BOARD_KLST_TINY or KLST_BOARD_KLST_SHEEP")
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

char KLST_U_ID_SERIAL[25] = "000000000000000000000000";  // 96-bit unique ID

/* options */

bool    mKLSTOptionEnableBeat             = true;
bool    mKLSTOptionEnableEncoders         = true;
bool    mKLSTOptionEnableSerialPorts      = true;
bool    mKLSTOptionEnableProgrammerButton = true;
bool    mKLSTOptionEnableAudioInput       = true;
float   mKLSTOptionHeadphoneOutputVolume  = 0.89;
float   mKLSTOptionSerial00BaudRate       = KLST_UART_BAUD;
float   mKLSTOptionSerial01BaudRate       = KLST_UART_BAUD;
float   mKLSTOptionSerial02BaudRate       = KLST_UART_BAUD;
uint8_t mKLSTAudioLine =
#if defined(KLST_BOARD_KLST_TINY)
    KLST_MIC
#elif defined(KLST_BOARD_KLST_CORE)
    KLST_LINE_IN
#elif defined(KLST_BOARD_KLST_SHEEP)
    KLST_MIC
#else
    KLST_MIC
#endif
    ;

/* beat */

HardwareTimer* mKLSTBeatTimer;
uint32_t       mKLSTBeatCounter          = 0;
uint32_t       mKLSTBeatIntervalDuration = 1000000 / 2;

/* encoders */
#define KLST_ENCODERS_OMIT_ROTATION_EVENTS
static int16_t mEncoder_00TickCountPrevious = 0;
static bool    mEncoder_00ButtonState       = true;
static bool    mEncoder_00ToggleOmitEvent   = true;
static int16_t mEncoder_01TickCountPrevious = 0;
static bool    mEncoder_01ButtonState       = true;
static bool    mEncoder_01ToggleOmitEvent   = true;
#ifdef ENCODER_02_TIMER
static int16_t mEncoder_02TickCountPrevious = 0;
static bool    mEncoder_02ButtonState       = true;
static bool    mEncoder_02ToggleOmitEvent   = true;
#endif

/* ----------------------------------------------------------------------------------------------------------------- */
/* OPTIONS                                                                                                           */
/* ----------------------------------------------------------------------------------------------------------------- */

uint32_t KLST_ISH_OPT_audio_line() {
    return mKLSTAudioLine;
}

bool KLST_ISH_OPT_audio_input_enabled() {
    return mKLSTOptionEnableAudioInput;
}

float KLST_ISH_OPT_headphone_output_volume() {
    return mKLSTOptionHeadphoneOutputVolume;
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* USB                                                                                                               */
/* ----------------------------------------------------------------------------------------------------------------- */
#ifdef USE_TINYUSB
extern void board_init(void);
void        KLST_BSP_configure_TinyUSB() {
    board_init();
}
#endif

/* ----------------------------------------------------------------------------------------------------------------- */
/* SETUP                                                                                                             */
/* ----------------------------------------------------------------------------------------------------------------- */
void KLST_ISH_handle_encoder_rotations();
void KLST_ISH_handle_encoder_buttons();

void KLST_IT_beat_callback() {
    beat(mKLSTBeatCounter++);
}

unsigned long KLST_get_U_ID(uint8_t pOffset);

void KLST_update_U_ID_serial() {
    for (uint8_t j = 0; j < 3; j++) {
        uint32_t pUID_0 = KLST_get_U_ID(j);
        char     mHexStr[9];
        sprintf(mHexStr, "%08lX", pUID_0);
        for (uint8_t i = 0; i < 8; i++) {
            KLST_U_ID_SERIAL[i + j * 8] = mHexStr[i];
        }
    }
}

/**
 * called before setup
 * @note(make sure to remove static from functions!)
 */
void KLST_ISH_pre_setup() {
    KLST_update_U_ID_serial();
    KLST_BSP_init_LEDs();
    KLST_BSP_init_peripherals();

    /* encoder buttons */
    pinMode(ENCODER_00_BUTTON, INPUT);
    pinMode(ENCODER_01_BUTTON, INPUT);
#ifdef ENCODER_02_BUTTON
    pinMode(ENCODER_02_BUTTON, INPUT);
#endif
    /* … and read initial button state at start up */
    mEncoder_00ButtonState = digitalRead(ENCODER_00_BUTTON);
    mEncoder_01ButtonState = digitalRead(ENCODER_01_BUTTON);
#ifdef ENCODER_02_BUTTON
    mEncoder_02ButtonState = digitalRead(ENCODER_02_BUTTON);
#endif

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

    /* start encoder timers */
    if (mKLSTOptionEnableEncoders) {
        KLST_BSP_init_encoders();

        mEncoder_00TickCountPrevious = (int16_t)ENCODER_00_TIMER->CNT;
        mEncoder_01TickCountPrevious = (int16_t)ENCODER_01_TIMER->CNT;
#ifdef ENCODER_02_TIMER
        mEncoder_02TickCountPrevious = (int16_t)ENCODER_02_TIMER->CNT;
#endif
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
    KLST_ISH_handle_encoder_rotations();
    KLST_ISH_handle_encoder_buttons();
}

void KLST_ISH_handle_encoder_rotations() {
    /* --- ENCODER_00 --- */
    const int16_t mEncoder_00TickCountCurrent = (int16_t)ENCODER_00_TIMER->CNT;
    if (mEncoder_00TickCountPrevious != mEncoder_00TickCountCurrent) {
        EventEncoder e;
        e.index          = ENCODER_00;
        e.ticks          = (float)mEncoder_00TickCountCurrent;
        e.previous_ticks = (float)mEncoder_00TickCountPrevious;
        e.delta          = e.ticks - e.previous_ticks;
#ifdef KLST_ENCODERS_OMIT_ROTATION_EVENTS
        mEncoder_00ToggleOmitEvent = !mEncoder_00ToggleOmitEvent;
        if (mEncoder_00ToggleOmitEvent) {
#endif
            event_receive(EVENT_ENCODER_ROTATED, (float*)(&e));
#ifdef KLST_ENCODERS_OMIT_ROTATION_EVENTS
        }
#endif
        mEncoder_00TickCountPrevious = mEncoder_00TickCountCurrent;
    }
    /* --- ENCODER_01 --- */
    const int16_t mEncoder_01TickCountCurrent = (int16_t)ENCODER_01_TIMER->CNT;
    if (mEncoder_01TickCountPrevious != mEncoder_01TickCountCurrent) {
        EventEncoder e;
        e.index          = ENCODER_01;
        e.ticks          = (float)mEncoder_01TickCountCurrent;
        e.previous_ticks = (float)mEncoder_01TickCountPrevious;
        e.delta          = e.ticks - e.previous_ticks;
#ifdef KLST_ENCODERS_OMIT_ROTATION_EVENTS
        mEncoder_01ToggleOmitEvent = !mEncoder_01ToggleOmitEvent;
        if (mEncoder_01ToggleOmitEvent) {
#endif
            event_receive(EVENT_ENCODER_ROTATED, (float*)(&e));
#ifdef KLST_ENCODERS_OMIT_ROTATION_EVENTS
        }
#endif
        mEncoder_01TickCountPrevious = mEncoder_01TickCountCurrent;
    }
    /* --- ENCODER_02 --- */
#ifdef ENCODER_02_TIMER
    const int16_t mEncoder_02TickCountCurrent = (int16_t)ENCODER_02_TIMER->CNT;
    if (mEncoder_02TickCountPrevious != mEncoder_02TickCountCurrent) {
        EventEncoder e;
        e.index          = ENCODER_02;
        e.ticks          = (float)mEncoder_02TickCountCurrent;
        e.previous_ticks = (float)mEncoder_02TickCountPrevious;
        e.delta          = e.ticks - e.previous_ticks;
#ifdef KLST_ENCODERS_OMIT_ROTATION_EVENTS
        mEncoder_02ToggleOmitEvent = !mEncoder_02ToggleOmitEvent;
        if (mEncoder_02ToggleOmitEvent) {
#endif
            event_receive(EVENT_ENCODER_ROTATED, (float*)(&e));
#ifdef KLST_ENCODERS_OMIT_ROTATION_EVENTS
        }
#endif
        mEncoder_02TickCountPrevious = mEncoder_02TickCountCurrent;
    }
#endif
}

void KLST_ISH_handle_encoder_buttons() {
    bool mENCODER_00ButtonState = digitalRead(ENCODER_00_BUTTON);
    if (mEncoder_00ButtonState != mENCODER_00ButtonState) {
        EventEncoder e;
        e.index = ENCODER_00;
        if (mENCODER_00ButtonState) {
            event_receive(EVENT_ENCODER_BUTTON_RELEASED, (float*)(&e));
        } else {
            event_receive(EVENT_ENCODER_BUTTON_PRESSED, (float*)(&e));
        }
        mEncoder_00ButtonState = mENCODER_00ButtonState;
    }
    bool mENCODER_01ButtonState = digitalRead(ENCODER_01_BUTTON);
    if (mEncoder_01ButtonState != mENCODER_01ButtonState) {
        EventEncoder e;
        e.index = ENCODER_01;
        if (mENCODER_01ButtonState) {
            event_receive(EVENT_ENCODER_BUTTON_RELEASED, (float*)(&e));
        } else {
            event_receive(EVENT_ENCODER_BUTTON_PRESSED, (float*)(&e));
        }
        mEncoder_01ButtonState = mENCODER_01ButtonState;
    }
#ifdef ENCODER_02_BUTTON
    bool mENCODER_02ButtonState = digitalRead(ENCODER_02_BUTTON);
    if (mEncoder_02ButtonState != mENCODER_02ButtonState) {
        EventEncoder e;
        e.index = ENCODER_02;
        if (mENCODER_02ButtonState) {
            event_receive(EVENT_ENCODER_BUTTON_RELEASED, (float*)(&e));
        } else {
            event_receive(EVENT_ENCODER_BUTTON_PRESSED, (float*)(&e));
        }
        mEncoder_02ButtonState = mENCODER_02ButtonState;
    }
#endif
}

static void handle_serial_data_receive(HardwareSerial& pSerial, DATA_PERIPHERAL_TYPE pPeripheral) {
    // @TOOD(maybe use a fixed static buffer?)
    uint8_t mLength;
    while ((mLength = pSerial.available()) > 0) {
        uint8_t mData[mLength];
        size_t  mBytesRead = pSerial.readBytes(mData, mLength);
        if (mBytesRead > 0) {
            data_receive(pPeripheral, mData, mBytesRead);
        }
    }
}

void KLST_ISH_handleSerialPorts() {
#ifdef HAL_UART_MODULE_ENABLED
    handle_serial_data_receive(KLST_SERIAL_00, SERIAL_00);
    handle_serial_data_receive(KLST_SERIAL_01, SERIAL_01);
#ifdef KLST_SERIAL_02
    handle_serial_data_receive(KLST_SERIAL_02, SERIAL_02);
#endif
//     {
//         // @TOOD(maybe use a fixed static buffer?)
//         uint8_t mLength;
//         while ((mLength = KLST_SERIAL_00.available()) > 0) {
//             uint8_t mData[mLength];
//             size_t  mBytesRead = KLST_SERIAL_00.readBytes(mData, mLength);
//             if (mBytesRead > 0) {
//                 data_receive(SERIAL_00, mData, mBytesRead);
//             }
//         }
//     }
//     {
//         const uint8_t mLength = KLST_SERIAL_01.available();
//         if (mLength > 0) {
//             uint8_t mData[mLength];
//             KLST_SERIAL_01.readBytes(mData, mLength);
//             data_receive(SERIAL_01, mData, mLength);
//         }
//     }
// #ifdef KLST_SERIAL_02
//     {
//         const uint8_t mLength = KLST_SERIAL_02.available();
//         if (mLength > 0) {
//             uint8_t mData[mLength];
//             KLST_SERIAL_02.readBytes(mData, mLength);
//             data_receive(SERIAL_02, mData, mLength);
//         }
//     }
// #endif
#endif
}

const uint8_t  M_NUM_OF_BITS = 16;
const float    M_INT_SCALE   = (1 << (M_NUM_OF_BITS - 1)) - 1.0;  // @todo(see if `-1.0` is required)
const uint32_t M_MASK_LEFT   = (1 << M_NUM_OF_BITS) - 1;
const uint32_t M_MASK_RIGHT  = ~(M_MASK_LEFT);

/**
 * fills buffer with new block of samples
 */
SIGNAL_TYPE mLeftTX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
SIGNAL_TYPE mRightTX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
SIGNAL_TYPE mLeftRX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
SIGNAL_TYPE mRightRX[KLANG_SAMPLES_PER_AUDIO_BLOCK];
WEAK void   KLST_ISH_fill_buffer(uint32_t* pTXBuffer, uint32_t* pRXBuffer, uint16_t pBufferLength) {
#ifdef KLST_DISABLE_INTERRUPTS_IN_AUDIOBLOCK
    noInterrupts();
#endif
    /* unpack receive buffer to sample */
    if (KLST_ISH_OPT_audio_input_enabled()) {
        for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            const uint32_t p         = pRXBuffer[i];
            const int16_t  mLeftInt  = (p & M_MASK_LEFT);
            const int16_t  mRightInt = (p & M_MASK_RIGHT) >> M_NUM_OF_BITS;
            mLeftRX[i]               = mLeftInt / M_INT_SCALE;
            mRightRX[i]              = mRightInt / M_INT_SCALE;
        }
    }
    /* calculate next audio block */
    audioblock(mLeftTX, mRightTX, mLeftRX, mRightRX);
    /* pack sample for transmit buffer */
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        int16_t mLeftInt  = (int16_t)(mLeftTX[i] * M_INT_SCALE);
        int16_t mRightInt = (int16_t)(mRightTX[i] * M_INT_SCALE);
        pTXBuffer[i]      = ((uint32_t)(uint16_t)mLeftInt) << 0 | ((uint32_t)(uint16_t)mRightInt) << M_NUM_OF_BITS;
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
    // - [x] Disable all peripheral clocks
    // - [ ] Disable used PLL
    // - [ ] Disable interrupts
    // - [ ] Clear pending interrupts
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
    SysMemBootJump = (void (*)(void))(*((uint32_t*)((BootAddr + 4))));

    /* Set the main stack pointer to the bootloader stack */
    __set_MSP(*(uint32_t*)BootAddr);

    /* turn on all LEDs */
    digitalWrite(LED_00, HIGH);
    digitalWrite(LED_01, HIGH);
    digitalWrite(LED_02, HIGH);

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
#if defined(USBCON) && defined(USBD_USE_CDC)
    SerialUSB.end();
#endif
    /* flash LEDs */
    digitalWrite(LED_00, LOW);
    digitalWrite(LED_01, HIGH);
    digitalWrite(LED_02, LOW);
    for (uint8_t i = 16; i > 0; i--) {
        KLST_shutdown_toggle_leds(24 + 12 * i);
    }
}

unsigned long KLST_get_U_ID(uint8_t pOffset) {
    static const uint32_t UID_ADDR = KLST_BSP_U_ID_address();
    return *((unsigned long*)UID_ADDR + pOffset * 0x04);
}

bool KLST_check_UID(const uint32_t pUID[]) {
    return pUID[0] == KLST_get_U_ID(0) &&
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
    Serial.begin(pBaudRate);
#if defined(USBCON) && defined(USBD_USE_CDC)
    if (pWaitForSerial) {
        static const uint32_t M_ITERATION_GUARD = 180000000;
        uint32_t              mIterationGuard   = 0;
        while (!Serial && mIterationGuard < M_ITERATION_GUARD) {
            // wait for serial port to connect. needed for native USB …
            // but only for a limited number of iterations.
            mIterationGuard++;
        }
    }
#endif
}

void klangstrom::option(uint8_t pOption, float pValue) {
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
        case KLST_OPTION_HEADPHONE_OUTPUT_VOLUME:
            mKLSTOptionHeadphoneOutputVolume = pValue;
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

void klangstrom::LED(uint16_t pLED, uint8_t pState) {
    switch (pState) {
        case LED_OFF:
        case LED_ON:
            digitalWrite(pLED, pState ? HIGH : LOW);
            break;
        case LED_TOGGLE:
            digitalWrite(pLED, !digitalRead(pLED));
            break;
    }
}

/**
 * @deprecated use `LED(uint16_t pLED, uint8_t pState)` instead
 * @brief
 *
 * @param pLED
 * @param pState
 */
void klangstrom::led(uint16_t pLED, bool pState) {
    digitalWrite(pLED, pState ? HIGH : LOW);
}

/**
 * @deprecated use `LED(uint16_t pLED, uint8_t pState)` instead
 * @brief
 *
 */
void klangstrom::led_toggle(uint16_t pLED) {
    digitalWrite(pLED, !digitalRead(pLED));
}

void klangstrom::beats_per_minute(float pBPM) {
    if (pBPM == 0) {
        return;
    }
    klangstrom::beats_per_minute_ms((uint32_t)((60.0 / pBPM) * 1000000));
}

void klangstrom::beats_per_minute_ms(uint32_t pMicroSeconds) {
    mKLSTBeatIntervalDuration = pMicroSeconds;
    mKLSTBeatTimer->setOverflow(mKLSTBeatIntervalDuration, MICROSEC_FORMAT);
}

bool klangstrom::button_state(uint8_t pButton) {
    switch (pButton) {
        case KLST_BUTTON_ENCODER_00:
            return !mEncoder_00ButtonState;
        case KLST_BUTTON_ENCODER_01:
            return !mEncoder_01ButtonState;
#ifdef ENCODER_02_TIMER
        case KLST_BUTTON_ENCODER_02:
            return !mEncoder_02ButtonState;
#endif
        case KLST_BUTTON_PROGRAMMER:
            return !digitalRead(BUTTON_PROGRAMMER);
    }
    return false;
}

bool klangstrom::pin_state(uint8_t pButton) {
    return !digitalRead(pButton);
}

void klangstrom::data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength) {
    switch (pSender) {
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

char* klangstrom::U_ID() {
    return KLST_U_ID_SERIAL;
}

#endif  // __cplusplus
