/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
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

#include "Klangstrom.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

#include "main.h"
#include "AudioDevice.h"
#include "Console.h"
#include "System.h"

#ifdef KLST_PERIPHERAL_ENABLE_GPIO
#include "gpio.h"
#endif // KLST_PERIPHERAL_ENABLE_GPIO

#if defined(KLST_PERIPHERAL_ENABLE_AUDIODEVICE) || \
    defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) ||  \
    defined(KLST_PERIPHERAL_ENABLE_SD_CARD) ||     \
    defined(KLST_PERIPHERAL_ENABLE_MIDI)
#include "dma.h"
#endif // defined(KLST_PERIPHERAL_ENABLE_AUDIODEVICE) || defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) || defined(KLST_PERIPHERAL_ENABLE_MIDI)

#ifdef KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY
#include "octospi.h"
#include "SystemExternalMemory_KLST_PANDA.h"
#endif // KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY

#ifdef __cplusplus
extern "C" {
#endif

AudioDevice* system_init_audiocodec() {
    // TOOD move this to BSP ... see SerialDevice for reference
    AudioInfo audioinfo;
    audioinfo.sample_rate     = 48000;
    audioinfo.output_channels = 2;
    audioinfo.input_channels  = 2;
    audioinfo.block_size      = 128;
    audioinfo.bit_depth       = 16;
    AudioDevice* audiodevice  = audiodevice_init_audiocodec(&audioinfo);
    if (audioinfo.device_id == AUDIO_DEVICE_INIT_ERROR) {
        console_error("error initializing audio device");
    }
    audiodevice_resume(audiodevice);
    return audiodevice;
}

static void system_init_BSP_KLST_PANDA();
static void system_init_BSP_KLST_PANDA_MX_Init_Modules();

static void system_init_BSP_KLST_PANDA_MX_Init_Modules() {
    // TODO really consider moving this to the periperhals
#ifdef KLST_PERIPHERAL_ENABLE_GPIO
    MX_GPIO_Init();
#endif // KLST_PERIPHERAL_ENABLE_GPIO

    /* initialize DMA */
#if defined(KLST_PERIPHERAL_ENABLE_AUDIODEVICE) || \
    defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) ||  \
    defined(KLST_PERIPHERAL_ENABLE_SD_CARD) ||     \
    defined(KLST_PERIPHERAL_ENABLE_MIDI) ||\
    defined(KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC)
    MX_DMA_Init();
#endif // defined(KLST_PERIPHERAL_ENABLE_AUDIODEVICE) || defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) || defined(KLST_PERIPHERAL_ENABLE_MIDI)

#ifdef KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY
    MX_OCTOSPI1_Init();
    HAL_Delay(100);
    externalmemory_init(); // TODO move somewhere better
//    externalmemory_test();
#endif // KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY
}

static void system_init_BSP_KLST_PANDA() {
    /* MPU Configuration--------------------------------------------------------*/
    MPU_Config();
    /* Enable the CPU Cache */

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* Configure the peripherals common clocks */
    PeriphCommonClock_Config();
}

#ifdef KLST_PANDA_STM32
static void system_deactivate_display() {
    /* turn display and backlight off when display is not used */
    static const uint8_t _DISPLAY_ON_OFF_Pin_ID = 4; // PC4
    GPIOC->MODER &= ~(0x3 << (_DISPLAY_ON_OFF_Pin_ID * 2));
    GPIOC->MODER |= ((1 & 0x3) << (_DISPLAY_ON_OFF_Pin_ID * 2));
    GPIOC->OTYPER &= ~(1 << _DISPLAY_ON_OFF_Pin_ID);
    //  GPIOC->ODR    |=  (  1 << _DISPLAY_ON_OFF_Pin_ID); // HIGH
    //  GPIOC->ODR    &= ~(  1 << _DISPLAY_ON_OFF_Pin_ID); // LOW
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_RESET);

    __HAL_RCC_GPIOC_CLK_ENABLE();
    static const uint8_t _DISPLAY_BACKLIGHT_PWM_Pin_ID = 8; // PC8
    GPIOC->MODER &= ~(0x3 << (_DISPLAY_BACKLIGHT_PWM_Pin_ID * 2));
    GPIOC->MODER |= ((1 & 0x3) << (_DISPLAY_BACKLIGHT_PWM_Pin_ID * 2));
    GPIOC->OTYPER &= ~(1 << _DISPLAY_BACKLIGHT_PWM_Pin_ID);
    //  GPIOC->ODR    |=  (  1 << _DISPLAY_BACKLIGHT_PWM_Pin_ID); // HIGH
    //  GPIOC->ODR    &= ~(  1 << _DISPLAY_BACKLIGHT_PWM_Pin_ID); // LOW
    HAL_GPIO_WritePin(_DISPLAY_BACKLIGHT_PWM_GPIO_Port, _DISPLAY_BACKLIGHT_PWM_Pin, GPIO_PIN_RESET);

    //    // TODO a bit of a hack to turn off the display backlight right away at startup
    //    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    //    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
}
#endif // KLST_PANDA_STM32

void system_init_BSP() {
    system_init_BSP_KLST_PANDA();
    system_init_BSP_KLST_PANDA_MX_Init_Modules();
#ifdef KLST_PANDA_STM32
    system_deactivate_display();
#endif // KLST_PANDA_STM32
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32 || KLST_CATERPILLAR_STM32
