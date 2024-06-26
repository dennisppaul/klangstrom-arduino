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

/**
 * @todo(there is a reoccuring problem with HAL callback functions being used in e.g `libraries/SrcWrapper/stm32/uart.c` and `libraries/SrcWrapper/stm32/timer.c`(?) )
 */

#if defined(KLST_BOARD_KLST_SHEEP)

#define KLST_BOARD_KLST_SHEEP__CLEAR_SCREEN_ON_STARTUP

#include "Arduino.h"
#include "AudioCodecWM8731.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterface_ARDUINO-ISH.h"
#include "KlangstromDefinesArduino.h"
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "variant_KLST_SHEEP-BSP.h"

// extern DMA_HandleTypeDef hdma_sai1_a;
// extern DMA_HandleTypeDef hdma_sai1_b;
// extern DMA_HandleTypeDef hdma_spi2_rx;
extern I2C_HandleTypeDef hi2c1;
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;
extern SPI_HandleTypeDef hspi3;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

#include "KlangstromApplicationInterface.h"

int klangstrom::get_LED_pin(uint16_t index) {
    const static int KLST_BOARD_LEDs[KLST_NUM_LEDS] = {
        LED_00,
        LED_01,
        LED_02,
        LED_03,
        LED_04,
        LED_05,
        LED_06,
        LED_07,
        LED_08,
        LED_09,
        LED_10,
        LED_11,
        LED_12,
        LED_13,
        LED_14,
        LED_15};
    if (index >= 0 && index < KLST_NUM_LEDS) {
        return KLST_BOARD_LEDs[index];
    }
    return -1;
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* ERROR HANDLING                                                                                                    */
/* ----------------------------------------------------------------------------------------------------------------- */

uint8_t mErrorCode = KLST_ERROR_CODE_NO_ERROR;
uint8_t KLST_BSP_error_code() {
    return mErrorCode;
}

void KLST_BSP_init_LEDs() {
    for (uint16_t i = 0; i < KLST_NUM_LEDS; i++) {
        pinMode(klangstrom::get_LED_pin(i), OUTPUT);
    }
}

#ifdef KLST_BOARD_KLST_SHEEP__CLEAR_SCREEN_ON_STARTUP
#include "KlangstromDisplay.h"
void KLST_clear_display() {
    klangstrom::KlangstromDisplay *mDisplay = klangstrom::KlangstromDisplay::create_ptr();
    mDisplay->begin();
    mDisplay->background(0, 0, 0);
    mDisplay->clear();
    klangstrom::KlangstromDisplay::release(mDisplay);
}
#endif

void KLST_BSP_init_peripherals() {
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_SAI1_Init();
    MX_SPI3_Init();
#ifdef KLST_BOARD_KLST_SHEEP__CLEAR_SCREEN_ON_STARTUP
    KLST_clear_display();
#endif
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* ENCODERS                                                                                                          */
/* ----------------------------------------------------------------------------------------------------------------- */

void KLST_BSP_init_encoders() {
    MX_TIM1_Init();  // ENCODER_01
    MX_TIM2_Init();  // ENCODER_00

    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
}

void KLST_BSP_deinit_encoders() {
    HAL_TIM_Encoder_DeInit(&htim1);
    HAL_TIM_Encoder_DeInit(&htim2);

    HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);

    HAL_TIM_Encoder_MspDeInit(&htim1);
    HAL_TIM_Encoder_MspDeInit(&htim2);
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* SAI + AUDIO CODEC                                                                                                 */
/* ----------------------------------------------------------------------------------------------------------------- */

void KLST_BSP_configure_audio_codec() {
    /*
     * POWER UP SEQUENCE
     * - Switch on power supplies. By default the WM8731 is in Standby Mode, the DAC is digitally muted and the Audio Interface and Outputs are all OFF.
     * - Set all required bits in the Power Down register (0Ch) to ‘0’; EXCEPT the OUTPD bit, this should be set to ‘1’ (Default).
     * - Set required values in all other registers except 12h (Active).
     * - Set the ‘Active’ bit in register 12h.
     * - The last write of the sequence should be setting OUTPD to ‘0’ (active) in register 0Ch, enabling the DAC signal path, free of any significant power-up noise.
     */
    //	WM8731_write(WM8731_RESET_REGISTER, 0b00000000);
    WM8731_write(WM8731_POWER_DOWN_CONTROL, 0b00010000);
    WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00000000);
    WM8731_write(WM8731_LINE_IN_LEFT, 0b000011111);  // disable simultaneous load, disable mute, (+12db)
    WM8731_write(WM8731_LINE_IN_RIGHT, 0b000011111);
    WM8731_write(WM8731_HEADPHONE_OUT_LEFT, 0b001111001);
    WM8731_write(WM8731_HEADPHONE_OUT_RIGHT, 0b001111001);
    if (KLST_ISH_OPT_audio_line() == KLST_MIC) {
        WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00010100);  // MIC @todo(enable mic boost(Bit 0)?)
    } else if (KLST_ISH_OPT_audio_line() == KLST_LINE_IN) {
        WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00010010);  // LINE_IN
    }
    WM8731_input_level(0x1F);
    WM8731_write(WM8731_DIGITAL_AUDIO_PATH_CONTROL, 0b00111);
    WM8731_write(WM8731_DIGITAL_AUDIO_INTERFACE_FORMAT, 0b00000010);
    WM8731_write(WM8731_SAMPLING_CONTROL, 0b000000010);
    WM8731_write(WM8731_ACTIVE_CONTROL, 0b1);

    WM8731_write(WM8731_POWER_DOWN_CONTROL, 0b00000000);

    WM8731_headphone_output_volume(KLST_ISH_OPT_headphone_output_volume());
}

/**
 * SAI callback implementation
 */

#define I2S_BUFFER_SIZE (KLANG_SAMPLES_PER_AUDIO_BLOCK * 2)
uint32_t  dma_TX_buffer[I2S_BUFFER_SIZE] __attribute__((section(".dma_buffer")));
uint32_t  dma_RX_buffer[I2S_BUFFER_SIZE] __attribute__((section(".dma_buffer")));
uint32_t *mCurrentRXBuffer;

void KLST_BSP_start_audio_codec() {
    memset(dma_TX_buffer, 0, sizeof(dma_TX_buffer));
    memset(dma_RX_buffer, 0, sizeof(dma_RX_buffer));
    if (HAL_OK != HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t *)dma_TX_buffer, I2S_BUFFER_SIZE << 1)) {
        // 		KLST_LOG("### ERROR initializing SAI TX");
    }
    if (KLST_ISH_OPT_audio_input_enabled()) {
        if (HAL_OK != HAL_SAI_Receive_DMA(&hsai_BlockB1, (uint8_t *)dma_RX_buffer, I2S_BUFFER_SIZE << 1)) {
            // 		KLST_LOG("### ERROR initializing SAI RX");
        }
        mCurrentRXBuffer = &(dma_RX_buffer[0]);
    }
}

#define SANITY_TEST             0
#define SANITY_TEST_PASSTHROUGH 0

#if SANITY_TEST

float left_osc_phi      = 0;
float left_osc_phi_inc  = 220.0f / (float)KLANG_AUDIO_RATE;  // generating 220Hz
float right_osc_phi     = 0;
float right_osc_phi_inc = 110.0f / (float)KLANG_AUDIO_RATE;  // generating 110Hz

void FillBuffer(uint32_t *mTXBuffer, uint32_t *mRXBuffer, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
#if SANITY_TEST_PASSTHROUGH
        mTXBuffer[i] = mRXBuffer[i];
#else
        const float mAmplitude = 0.1f;

        float mLeftf = (float)sin(left_osc_phi * 6.2832f) * mAmplitude;
        left_osc_phi += left_osc_phi_inc;
        left_osc_phi -= (float)((uint16_t)left_osc_phi);
        int16_t mLefti = (int16_t)(mLeftf * 32767.0f);

        float mRightf = (float)sin(right_osc_phi * 6.2832f) * mAmplitude;
        right_osc_phi += right_osc_phi_inc;
        right_osc_phi -= (float)((uint16_t)right_osc_phi);
        int16_t mRighti = (int16_t)(mRightf * 32767.0f);

        // both channels
        mTXBuffer[i] = ((uint32_t)(uint16_t)mLefti) << 0 | ((uint32_t)(uint16_t)mRighti) << 16;
#endif
    }
}
#endif

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai) {
#if SANITY_TEST
    FillBuffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
    KLST_ISH_fill_buffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
#if SANITY_TEST
    FillBuffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
    KLST_ISH_fill_buffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
    mCurrentRXBuffer = &(dma_RX_buffer[I2S_BUFFER_SIZE >> 1]);
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
    mCurrentRXBuffer = &(dma_RX_buffer[0]);
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* WM8731                                                                                                            */
/* ----------------------------------------------------------------------------------------------------------------- */

/**
 * WM8731 I2C implementation
 */
bool WM8731_I2C_write(uint8_t device_address, uint8_t *data, uint8_t length) {
    HAL_StatusTypeDef mResult = HAL_I2C_Master_Transmit(&hi2c1, device_address, data, length, 10);
    if (mResult != HAL_OK) {
        mErrorCode = KLST_ERROR_CODE_I2C_WRITE;
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
/* SYSTEM                                                                                                            */
/* ----------------------------------------------------------------------------------------------------------------- */

uint32_t KLST_BSP_boot_address() {
    //    AN2606 Rev 51 STM32 microcontroller system memory boot mode, p261
    return 0x1FF09800;  // boot address for STM32H74xxx/75xxx
}

uint32_t KLST_BSP_U_ID_address() {
    //    RM0433 Reference manual STM32H742, STM32H743/753 and STM32H750, p3271ff
    //    - Base address   : 0x1FF1E800
    //    - address offset : 0x00 U_ID(31:00)
    //    - address offset : 0x04 U_ID(63:32)
    //    - address offset : 0x08 U_ID(95:64)
    return 0x1FF1E800;
}

void KLST_BSP_shutdown() {
    /* stop encoders */
    KLST_BSP_deinit_encoders();
    /* stop audio */
    HAL_SAI_DeInit(&hsai_BlockB1);
    HAL_SAI_DeInit(&hsai_BlockA1);
    HAL_SAI_DMAStop(&hsai_BlockB1);
    HAL_SAI_DMAStop(&hsai_BlockA1);
    HAL_SAI_MspDeInit(&hsai_BlockB1);
    HAL_SAI_MspDeInit(&hsai_BlockA1);
    /* stop I2C */
    HAL_I2C_DeInit(&hi2c1);
    HAL_I2C_MspDeInit(&hi2c1);
    /* stop SPI */
    HAL_SPI_DeInit(&hspi3);
    HAL_SPI_MspDeInit(&hspi3);
    /* stop USB */
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
}

#ifdef __cplusplus
}
#endif

#endif /* KLST_BOARD_KLST_SHEEP */
