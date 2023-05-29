/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#include "KlangstromDefines.hpp"  // @NOTE(this is required for defines below)

#if (KLST_ARCH == KLST_ARCH_MCU)
#include "Arduino.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterface.h"
#include "KlangstromApplicationInterface_ARDUINO-ISH.h"
#include "KlangstromDefinesArduino.h"
#include "pins_arduino.h"

// @TODO check if this works ... copied from `KlangstromApplication.cpp`

#include <vector>

#include "Klangstrom.h"
#include "core_callback.h"

void klangstrom_callback();
bool klangstrom_is_initialized = false;

void initVariant() {
    /* pre_setup */
    registerCoreCallback(&klangstrom_callback);
    // Serial2.begin(115200);
    // Serial2.println("pre_setup");
    configure();
    KLST_ISH_pre_setup();
}

void klangstrom_callback() {
    if (klangstrom_is_initialized) {
        /* loop */
        // Serial2.println("loop");
        KLST_ISH_loop();
    } else {
        /* post_setup */
        klangstrom_is_initialized = true;
        // Serial2.println("post_setup");
        KLST_ISH_post_setup();
    }
}

// @todo(find a way to make library headers available in this context … e.g in `platform.txt`)
// #if defined(TINYUSB_MIDI_DEVICE) || defined(TINYUSB_MOUSE_KEYBOARD_DEVICE)
// #if !(__has_include("USBDevice.h"))
// #warning "could not find USB Device library. make sure to include `#include "USBDevice.h"` at beginning of sketch."
// #endif
// #endif

#if (!defined(KLST_BOARD_KLST_TINY) && !defined(KLST_BOARD_KLST_CORE) && !defined(KLST_BOARD_KLST_SHEEP))
#warning("@KLST board type not defined! options are KLST_BOARD_KLST_CORE or KLST_BOARD_KLST_TINY or KLST_BOARD_KLST_SHEEP")
#endif

#define KLST_DISABLE_INTERRUPTS_IN_AUDIOBLOCK

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
EventEncoder fEncoderEvents[KLST_NUM_ENCODERS];

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

void KLST_IT_beat_callback() {
    beat(mKLSTBeatCounter++);
}

unsigned long KLST_get_U_ID(uint8_t offset);

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

/* --- ENCODER helper functions --- */

static int16_t getEncoderRotation(uint8_t index) {
    switch (index) {
        case ENCODER_00:
            return (int16_t)ENCODER_00_TIMER->CNT / 2;
        case ENCODER_01:
            return (int16_t)ENCODER_01_TIMER->CNT / 2;
#ifdef ENCODER_02_TIMER
        case ENCODER_02:
            return (int16_t)ENCODER_02_TIMER->CNT / 2;
#endif
    }
    return 0;
}

static bool isEncoderButtonPressed(uint8_t index) {
    bool button = true;
    switch (index) {
        case ENCODER_00: {
            button = !digitalRead(ENCODER_00_BUTTON);
            break;
        }
        case ENCODER_01: {
            button = !digitalRead(ENCODER_01_BUTTON);
            break;
        }
#ifdef ENCODER_02_BUTTON
        case ENCODER_02: {
            button = !digitalRead(ENCODER_02_BUTTON);
            break;
        }
#endif
    }
    return button;
}

static bool isEncoderRotated(EventEncoder& e) {
    return fEncoderEvents[e.index].ticks != getEncoderRotation(e.index);
}

static void updateEncoderEvent(EventEncoder& encoder, bool new_button_state) {
    encoder.ticks_previous = encoder.ticks;
    encoder.ticks          = getEncoderRotation(encoder.index);
    encoder.delta          = encoder.ticks - encoder.ticks_previous;
    encoder.button         = new_button_state;
}

/* --- ENCODER helper functions --- */

/**
 * called before setup
 * @note(make sure to remove static from functions!)
 */
void KLST_ISH_pre_setup() {
    register_audioblock(audioblock);

    KLST_update_U_ID_serial();
    KLST_BSP_init_LEDs();
    KLST_BSP_init_peripherals();

    /* ADC + DAC */
    // @todo(this needs to be handled in a more generic fashion)
    if (KLST_NUM_ADCS == 2) {
        pinMode(ADC_00, INPUT);
        pinMode(ADC_01, INPUT);
        analogReadResolution(KLST_ANALOG_RESOLUTION);
    }
    if (KLST_NUM_DACS == 2) {
        pinMode(DAC_00, OUTPUT);
        pinMode(DAC_01, OUTPUT);
        analogWriteResolution(KLST_ANALOG_RESOLUTION);
    }

    /* encoder buttons */
    pinMode(ENCODER_00_BUTTON, INPUT);
    pinMode(ENCODER_01_BUTTON, INPUT);
#ifdef ENCODER_02_BUTTON
    pinMode(ENCODER_02_BUTTON, INPUT);
#endif
    /* … and read initial encoder states at start up */
    for (uint8_t i = 0; i < KLST_NUM_ENCODERS; i++) {
        fEncoderEvents[i].index          = ENCODER_00 + i;
        fEncoderEvents[i].ticks          = getEncoderRotation(fEncoderEvents[i].index);
        fEncoderEvents[i].ticks_previous = fEncoderEvents[i].ticks;
        fEncoderEvents[i].delta          = 0;
        fEncoderEvents[i].button         = isEncoderButtonPressed(fEncoderEvents[i].index);
    }

    /* programmer button */
    pinMode(KLST_BUTTON_PROGRAMMER, INPUT);

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
        for (uint8_t i = 0; i < KLST_NUM_ENCODERS; i++) {
            updateEncoderEvent(fEncoderEvents[i], isEncoderButtonPressed(i));
        }
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

static void handleEncoderButtonEvent(EventEncoder& e) {
    if (e.button) {
        call_encoder_pressed(e.index);
    } else {
        call_encoder_released(e.index);
    }
    call_encoder_event_receive(EVENT_ENCODER, &e);
}

static void handleEncoderRotationEvent(EventEncoder& e) {
    call_encoder_rotated(e.index, e.ticks, e.delta);
    call_encoder_event_receive(EVENT_ENCODER, &e);
}

void KLST_ISH_handle_encoders() {
    for (uint8_t i = 0; i < KLST_NUM_ENCODERS; i++) {
        const bool mIsEncoderRotated  = isEncoderRotated(fEncoderEvents[i]);
        const bool mButtonState       = isEncoderButtonPressed(fEncoderEvents[i].index);
        const bool mHasEncoderChanged = (mButtonState != fEncoderEvents[i].button);
        updateEncoderEvent(fEncoderEvents[i], mButtonState);
        if (mIsEncoderRotated) {
            handleEncoderRotationEvent(fEncoderEvents[i]);
        }
        if (mHasEncoderChanged) {
            handleEncoderButtonEvent(fEncoderEvents[i]);
        }
    }
}

static void handleSerialDataReceive(HardwareSerial& pSerial, DATA_PERIPHERAL_TYPE pPeripheral) {
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
    handleSerialDataReceive(KLST_SERIAL_00, SERIAL_00);
    handleSerialDataReceive(KLST_SERIAL_01, SERIAL_01);
#ifdef KLST_SERIAL_02
    handleSerialDataReceive(KLST_SERIAL_02, SERIAL_02);
#endif
#endif  // HAL_UART_MODULE_ENABLED
}

const uint8_t  M_NUM_OF_BITS = 16;
const float    M_INT_SCALE   = (1 << (M_NUM_OF_BITS - 1)) - 1.0;  // @todo(see if `-1.0` is required)
const uint32_t M_MASK_LEFT   = (1 << M_NUM_OF_BITS) - 1;
const uint32_t M_MASK_RIGHT  = ~(M_MASK_LEFT);

/**
 * fills buffer with new block of samples
 */
float     fOutputBuffers[KLANG_OUTPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
float     fInputBuffers[KLANG_INPUT_CHANNELS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
WEAK void KLST_ISH_fill_buffer(uint32_t* pTXBuffer, uint32_t* pRXBuffer, uint16_t pBufferLength) {
#ifdef KLST_DISABLE_INTERRUPTS_IN_AUDIOBLOCK
    noInterrupts();
#endif
    /* unpack receive buffer to sample */
    if (KLST_ISH_OPT_audio_input_enabled()) {
        for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            const uint32_t p         = pRXBuffer[i];
            const int16_t  mLeftInt  = (p & M_MASK_LEFT);
            const int16_t  mRightInt = (p & M_MASK_RIGHT) >> M_NUM_OF_BITS;
            fInputBuffers[LEFT][i]               = mLeftInt / M_INT_SCALE;
            fInputBuffers[RIGHT][i]              = mRightInt / M_INT_SCALE;
        }
    }
    /* calculate next audio block */
    // NOTE the code below is NOT very elegant, however it seems as if this is the only way to pass a 2D array to a function.
    // the option to pass the arrays by performing a C-style cast to a double pointer ( e.g `(float **)fOutputBuffers` ) is not
    // allowed and may produce undefined behavior or a segmantation fault.
    // it might be a better idea to use dynamic memory allocation after all …
    float* mInputBuffer[KLANG_INPUT_CHANNELS];
    float* mOutputBuffer[KLANG_OUTPUT_CHANNELS];
    for (size_t j = 0; j < KLANG_INPUT_CHANNELS; j++) {
        mInputBuffer[j] = fInputBuffers[j];
    }
    for (size_t j = 0; j < KLANG_OUTPUT_CHANNELS; j++) {
        mOutputBuffer[j] = fOutputBuffers[j];
    }
    call_audioblock(mInputBuffer, mOutputBuffer);

    /* pack sample for transmit buffer */
    for (int i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        int16_t mLeftInt  = (int16_t)(fOutputBuffers[LEFT][i] * M_INT_SCALE);
        int16_t mRightInt = (int16_t)(fOutputBuffers[RIGHT][i] * M_INT_SCALE);
        pTXBuffer[i]      = ((uint32_t)(uint16_t)mLeftInt) << 0 | ((uint32_t)(uint16_t)mRightInt) << M_NUM_OF_BITS;
        // //@todo this assumes a 16bit sample depth!
        // int16_t mL = (int16_t) (fOutputBuffers[LEFT][i] * 32767.0f);
        // int16_t mR = (int16_t) (fOutputBuffers[RIGHT][i] * 32767.0f);
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

void KLST_shutdown_toggle_leds(const uint16_t delay_time) {
    delay(delay_time);
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

void klangstrom::option(uint8_t option, float value) {
    switch (option) {
        case KLST_OPTION_AUDIO_INPUT:
            mKLSTAudioLine = value;
            break;
        case KLST_OPTION_ENCODERS:
            mKLSTOptionEnableEncoders = value;
            break;
        case KLST_OPTION_SERIAL_PORTS:
            mKLSTOptionEnableSerialPorts = value;
            break;
        case KLST_OPTION_BEAT:
            mKLSTOptionEnableBeat = value;
            break;
        case KLST_OPTION_PROGRAMMER_BUTTON:
            mKLSTOptionEnableProgrammerButton = value;
            break;
        case KLST_OPTION_ENABLE_AUDIO_INPUT:
            mKLSTOptionEnableAudioInput = value;
            break;
        case KLST_OPTION_HEADPHONE_OUTPUT_VOLUME:
            mKLSTOptionHeadphoneOutputVolume = value;
            break;
        case KLST_OPTION_SERIAL_00_BAUD_RATE:
            mKLSTOptionSerial00BaudRate = value;
            break;
        case KLST_OPTION_SERIAL_01_BAUD_RATE:
            mKLSTOptionSerial01BaudRate = value;
            break;
        case KLST_OPTION_SERIAL_02_BAUD_RATE:
            mKLSTOptionSerial02BaudRate = value;
            break;
    }
}

float klangstrom::get_option(uint8_t option) {
    switch (option) {
        case KLST_OPTION_AUDIO_INPUT:
            return mKLSTAudioLine;
        case KLST_OPTION_ENCODERS:
            return mKLSTOptionEnableEncoders;
        case KLST_OPTION_SERIAL_PORTS:
            return mKLSTOptionEnableSerialPorts;
        case KLST_OPTION_BEAT:
            return mKLSTOptionEnableBeat;
        case KLST_OPTION_PROGRAMMER_BUTTON:
            return mKLSTOptionEnableProgrammerButton;
        case KLST_OPTION_ENABLE_AUDIO_INPUT:
            return mKLSTOptionEnableAudioInput;
        case KLST_OPTION_HEADPHONE_OUTPUT_VOLUME:
            return mKLSTOptionHeadphoneOutputVolume;
        case KLST_OPTION_SERIAL_00_BAUD_RATE:
            return mKLSTOptionSerial00BaudRate;
        case KLST_OPTION_SERIAL_01_BAUD_RATE:
            return mKLSTOptionSerial01BaudRate;
        case KLST_OPTION_SERIAL_02_BAUD_RATE:
            return mKLSTOptionSerial02BaudRate;
    }
    return -1.0;
}

void klangstrom::LED(uint16_t pin, uint8_t state) {
    switch (state) {
        case LED_OFF:
        case LED_ON:
            digitalWrite(pin, state ? HIGH : LOW);
            break;
        case LED_TOGGLE:
            digitalWrite(pin, !digitalRead(pin));
            break;
    }
}

void klangstrom::DAC(uint8_t DAC, uint16_t value) {
    analogWrite(DAC, value);
}

uint16_t klangstrom::ADC(uint8_t ADC) {
    return analogRead(ADC);
}

void klangstrom::beats_per_minute(float BPM) {
    if (BPM <= 0) {
        klangstrom::beats_per_minute_ms(0);
        mKLSTBeatTimer->pause();
        return;
    }
    mKLSTBeatTimer->resume();
    klangstrom::beats_per_minute_ms((uint32_t)((60.0 / BPM) * 1000000));
}

void klangstrom::beats_per_minute_ms(uint32_t micro_seconds) {
    mKLSTBeatIntervalDuration = micro_seconds;
    mKLSTBeatTimer->setOverflow(mKLSTBeatIntervalDuration, MICROSEC_FORMAT);
}

bool klangstrom::button_state(uint8_t button) {
    // TODO check if the button state should be flipped
    switch (button) {
        case KLST_BUTTON_ENCODER_00:
            return fEncoderEvents[ENCODER_00].button;
        case KLST_BUTTON_ENCODER_01:
            return fEncoderEvents[ENCODER_01].button;
#ifdef ENCODER_02_TIMER
        case KLST_BUTTON_ENCODER_02:
            return fEncoderEvents[ENCODER_02].button;
#endif
        case KLST_BUTTON_PROGRAMMER:
            return !digitalRead(BUTTON_PROGRAMMER);
    }
    return false;
}

bool klangstrom::pin_state(uint8_t button) {
    return !digitalRead(button);
}

void klangstrom::data_transmit(const uint8_t sender, uint8_t* data, uint16_t length) {
    switch (sender) {
#ifdef HAL_UART_MODULE_ENABLED
#ifdef KLST_SERIAL_00
        case SERIAL_00:
            KLST_SERIAL_00.write(data, length);
            break;
#endif
#ifdef KLST_SERIAL_01
        case SERIAL_01:
            KLST_SERIAL_01.write(data, length);
            break;
#endif
#ifdef KLST_SERIAL_02
        case SERIAL_02:
            KLST_SERIAL_02.write(data, length);
            break;
#endif
#endif
    }
}

char* klangstrom::U_ID() {
    return KLST_U_ID_SERIAL;
}

uint32_t klangstrom::query_available_memory_block() {
    uint32_t              size     = 1;
    uint32_t              acc_size = 0;
    uint8_t*              ptr      = nullptr;
    std::vector<uint8_t*> pointers;
    while (true) {
        ptr = new (std::nothrow) uint8_t[size];
        if (ptr == nullptr) {
            break;
        }
        acc_size += size;
        pointers.push_back(ptr);
        size *= 2;
    }
    for (auto ptr : pointers) {
        delete[] ptr;
    }
    return size / 2;
}

#endif  // __cplusplus

#endif  // (KLST_ARCH==KLST_ARCH_MCU)
