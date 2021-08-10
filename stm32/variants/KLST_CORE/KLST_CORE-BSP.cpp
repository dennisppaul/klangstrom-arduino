//
//  KLST_TINY-adapter.cpp
//  Klangstrom – a node+text-based synthesizer library
//
//
//

/**
 * @todo(there is a reoccuring problem with HAL callback functions being used in e.g `libraries/SrcWrapper/stm32/uart.c` and `libraries/SrcWrapper/stm32/timer.c`(?) )
 */

#include "Arduino.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterfaceArduino.h"
#include "KlangstromDefinesArduino.h"
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "AudioCodecWM8731.h"
#include "stm32h7xx_hal.h"

extern I2C_HandleTypeDef hi2c3;

extern I2S_HandleTypeDef hi2s2;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_spi2_tx;

extern SPI_HandleTypeDef hspi4;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

// extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
// extern HCD_HandleTypeDef hhcd_USB_OTG_HS;

void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_USB_OTG_FS_PCD_Init(void);
void MX_USB_OTG_HS_HCD_Init(void);
void MX_I2S2_Init(void);
void MX_I2C3_Init(void);
void MX_SPI4_Init(void);
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM5_Init(void);

/* ----------------------------------------------------------------------------------------------------------------- */
/* USB                                                                                                 */
/* ----------------------------------------------------------------------------------------------------------------- */
extern void board_init(void);
void        KLST_BSP_configure_TinyUSB() {
    board_init();
    // GPIO_InitTypeDef GPIO_InitStruct = {0};
    // RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    // PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    // PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    // if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    // {
    //   Error_Handler();
    // }

    // /** Enable USB Voltage detector */
    // HAL_PWREx_EnableUSBVoltageDetector();

    // __HAL_RCC_GPIOA_CLK_ENABLE();
    // /* Configure DM DP Pins */
    // GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // /* This for ID line debug */
    // GPIO_InitStruct.Pin = GPIO_PIN_10;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    // GPIO_InitStruct.Pull = GPIO_PULLUP;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_HS;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // // https://community.st.com/s/question/0D50X00009XkYZLSA3/stm32h7-nucleo-usb-fs-cdc
    // // TODO: Board init actually works fine without this line.
    // HAL_PWREx_EnableUSBVoltageDetector();
    // __HAL_RCC_USB2_OTG_FS_CLK_ENABLE();

    // // Disable VBUS sense (B device) via pin PA9
    // USB_OTG_FS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

    // // B-peripheral session valid override enable
    // USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;
    // USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* ENCODERS                                                                                                 */
/* ----------------------------------------------------------------------------------------------------------------- */

void KLST_BSP_init_peripherals() {
    MX_GPIO_Init();
    // MX_USB_OTG_FS_PCD_Init();
    // MX_USB_OTG_HS_HCD_Init();

    MX_DMA_Init();
    MX_I2S2_Init();
    MX_I2C3_Init();

    // @todo(implement SDCARD)
    // MX_SPI4_Init();
}

void KLST_BSP_init_encoders() {
    MX_TIM5_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();

    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
}

void KLST_BSP_deinit_encoders() {
    HAL_TIM_Encoder_DeInit(&htim5);
    HAL_TIM_Encoder_DeInit(&htim1);
    HAL_TIM_Encoder_DeInit(&htim2);

    HAL_TIM_Encoder_Stop(&htim5, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Stop(&htim2, TIM_CHANNEL_ALL);

    HAL_TIM_Encoder_MspDeInit(&htim5);
    HAL_TIM_Encoder_MspDeInit(&htim1);
    HAL_TIM_Encoder_MspDeInit(&htim2);
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* I2S + AUDIO CODEC                                                                                                 */
/* ----------------------------------------------------------------------------------------------------------------- */

void KLST_BSP_configure_audio_codec() {
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
    WM8731_write(WM8731_LINE_IN_LEFT, 0b000011111);  // disable simultaneous load, disable mute, (+12db)
    WM8731_write(WM8731_LINE_IN_RIGHT, 0b000011111);
    WM8731_write(WM8731_HEADPHONE_OUT_LEFT, 0b001111001);
    WM8731_write(WM8731_HEADPHONE_OUT_RIGHT, 0b001111001);
    if (KLST_ISH_OPT_audio_line() == KLST_MIC) {
        WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00010100);  // MIC
    } else if (KLST_ISH_OPT_audio_line() == KLST_LINE_IN) {
        WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0b00010010);  // LINE_IN
    }
    WM8731_inputLevel(0x1F);
    WM8731_write(WM8731_DIGITAL_AUDIO_PATH_CONTROL, 0b00111);
    WM8731_write(WM8731_DIGITAL_AUDIO_INTERFACE_FORMAT, 0b00000010);
    WM8731_write(WM8731_SAMPLING_CONTROL, 0b000000010);
    WM8731_write(WM8731_ACTIVE_CONTROL, 0b1);

    WM8731_write(WM8731_POWER_DOWN_CONTROL, 0b00000000);
}

/**
 * I2S callback implementation
 */

#define I2S_BUFFER_SIZE (KLANG_SAMPLES_PER_AUDIO_BLOCK * 2)
uint32_t __attribute__((section(".dmadata"))) dma_TX_buffer[I2S_BUFFER_SIZE];
uint32_t __attribute__((section(".dmadata"))) dma_RX_buffer[I2S_BUFFER_SIZE];
uint32_t *mCurrentRXBuffer;

void KLST_BSP_start_audio_codec() {
    memset(dma_TX_buffer, 0, sizeof(dma_TX_buffer));
    memset(dma_RX_buffer, 0, sizeof(dma_RX_buffer));
    if (HAL_OK != HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)dma_TX_buffer, I2S_BUFFER_SIZE << 1)) {
    }
    if (KLST_ISH_OPT_audio_input_enabled()) {
        hi2s2.State = HAL_I2S_STATE_READY;  // @note(state flag needs to be cleared manually)
        if (HAL_OK != HAL_I2S_Receive_DMA(&hi2s2, (uint16_t *)dma_RX_buffer, I2S_BUFFER_SIZE << 1)) {
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

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
#if SANITY_TEST
    FillBuffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
    KLST_ISH_fill_buffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
#if SANITY_TEST
    FillBuffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
    KLST_ISH_fill_buffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
    mCurrentRXBuffer = &(dma_RX_buffer[I2S_BUFFER_SIZE >> 1]);
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
    mCurrentRXBuffer = &(dma_RX_buffer[0]);
}

/* ----------------------------------------------------------------------------------------------------------------- */
/* WM8731                                                                                                 */
/* ----------------------------------------------------------------------------------------------------------------- */

/**
 * WM8731 I2C implementation
 */
bool WM8731_I2C_write(uint8_t device_address, uint8_t *data, uint8_t length) {
    HAL_StatusTypeDef mResult = HAL_I2C_Master_Transmit(&hi2c3, device_address, data, length, 10);
    if (mResult != HAL_OK) {
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
/* SYSTEM                                                                                                 */
/* ----------------------------------------------------------------------------------------------------------------- */

uint32_t KLST_BSP_boot_address() {
    //    AN2606 STM32 microcontroller system memory boot mode, p252
    return 0x1FF09800;  // boot address for KLST_CORE
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
    HAL_I2S_DMAStop(&hi2s2);
    HAL_I2S_DeInit(&hi2s2);
    HAL_I2S_MspDeInit(&hi2s2);
    /* stop I2C */
    HAL_I2C_DeInit(&hi2c3);
    HAL_I2C_MspDeInit(&hi2c3);
    /* stop USB */
    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
}

#ifdef __cplusplus
}
#endif

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
//     KLST_ISH_fill_buffer(&(mTXBuffer[0]),
//                     &(mRXBuffer[0]),
//                     I2S_BUFFER_SIZE >> 1);
//   } else if (mBufferPointer >= I2S_BUFFER_SIZE) {
//     // void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
//     KLST_ISH_fill_buffer(&(mTXBuffer[I2S_BUFFER_SIZE >> 1]),
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
