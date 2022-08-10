/*
 *******************************************************************************
 * Copyright (c) 2020-2021, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#ifndef _VARIANT_ARDUINO_STM32_
#define _VARIANT_ARDUINO_STM32_

#ifndef KLST_BOARD_KLST_SHEEP
#error "@KLST no or wrong board type defined. KLST_BOARD_KLST_SHEEP not defined!"
#endif

// #ifdef __cplusplus
// extern "C" {
// #endif  // __cplusplus

#include <stddef.h>
#include <stdint.h>

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
#define PA0   PIN_A0
#define PA1   PIN_A1
#define PA2   PIN_A2
#define PA3   PIN_A3
#define PA4   PIN_A4
#define PA5   PIN_A5
#define PA6   PIN_A6
#define PA7   PIN_A7
#define PA8   8
#define PA9   9
#define PA10  10
#define PA11  11
#define PA12  12
#define PA13  13
#define PA14  14
#define PA15  15
#define PB0   PIN_A8
#define PB1   PIN_A9
#define PB2   18
#define PB3   19
#define PB4   20
#define PB5   21
#define PB6   22
#define PB7   23
#define PB8   24
#define PB9   25
#define PB10  26
#define PB11  27
#define PB12  28
#define PB13  29
#define PB14  30
#define PB15  31
#define PC0   PIN_A10
#define PC1   PIN_A11
#define PC4   PIN_A12
#define PC5   PIN_A13
#define PC6   36
#define PC7   37
#define PC8   38
#define PC9   39
#define PC10  40
#define PC11  41
#define PC12  42
#define PC13  43
#define PC14  44
#define PC15  45
#define PD0   46
#define PD1   47
#define PD2   48
#define PD3   49
#define PD4   50
#define PD5   51
#define PD6   52
#define PD7   53
#define PD8   54
#define PD9   55
#define PD10  56
#define PD11  57
#define PD12  58
#define PD13  59
#define PD14  60
#define PD15  61
#define PE0   62
#define PE1   63
#define PE2   64
#define PE3   65
#define PE4   66
#define PE5   67
#define PE6   68
#define PE7   69
#define PE8   70
#define PE9   71
#define PE10  72
#define PE11  73
#define PE12  74
#define PE13  75
#define PE14  76
#define PE15  77
#define PH0   78
#define PH1   79
#define PC2_C PIN_A14
#define PC3_C PIN_A15

// Alternate pins number
#define PA0_ALT1  (PA0 | ALT1)
#define PA1_ALT1  (PA1 | ALT1)
#define PA1_ALT2  (PA1 | ALT2)
#define PA2_ALT1  (PA2 | ALT1)
#define PA2_ALT2  (PA2 | ALT2)
#define PA3_ALT1  (PA3 | ALT1)
#define PA3_ALT2  (PA3 | ALT2)
#define PA4_ALT1  (PA4 | ALT1)
#define PA4_ALT2  (PA4 | ALT2)
#define PA5_ALT1  (PA5 | ALT1)
#define PA6_ALT1  (PA6 | ALT1)
#define PA7_ALT1  (PA7 | ALT1)
#define PA7_ALT2  (PA7 | ALT2)
#define PA7_ALT3  (PA7 | ALT3)
#define PA9_ALT1  (PA9 | ALT1)
#define PA10_ALT1 (PA10 | ALT1)
#define PA11_ALT1 (PA11 | ALT1)
#define PA12_ALT1 (PA12 | ALT1)
#define PA15_ALT1 (PA15 | ALT1)
#define PA15_ALT2 (PA15 | ALT2)
#define PB0_ALT1  (PB0 | ALT1)
#define PB0_ALT2  (PB0 | ALT2)
#define PB1_ALT1  (PB1 | ALT1)
#define PB1_ALT2  (PB1 | ALT2)
#define PB3_ALT1  (PB3 | ALT1)
#define PB3_ALT2  (PB3 | ALT2)
#define PB4_ALT1  (PB4 | ALT1)
#define PB4_ALT2  (PB4 | ALT2)
#define PB5_ALT1  (PB5 | ALT1)
#define PB5_ALT2  (PB5 | ALT2)
#define PB6_ALT1  (PB6 | ALT1)
#define PB6_ALT2  (PB6 | ALT2)
#define PB7_ALT1  (PB7 | ALT1)
#define PB8_ALT1  (PB8 | ALT1)
#define PB8_ALT2  (PB8 | ALT2)
#define PB9_ALT1  (PB9 | ALT1)
#define PB9_ALT2  (PB9 | ALT2)
#define PB14_ALT1 (PB14 | ALT1)
#define PB14_ALT2 (PB14 | ALT2)
#define PB15_ALT1 (PB15 | ALT1)
#define PB15_ALT2 (PB15 | ALT2)
#define PC0_ALT1  (PC0 | ALT1)
#define PC0_ALT2  (PC0 | ALT2)
#define PC1_ALT1  (PC1 | ALT1)
#define PC1_ALT2  (PC1 | ALT2)
#define PC4_ALT1  (PC4 | ALT1)
#define PC5_ALT1  (PC5 | ALT1)
#define PC6_ALT1  (PC6 | ALT1)
#define PC6_ALT2  (PC6 | ALT2)
#define PC7_ALT1  (PC7 | ALT1)
#define PC7_ALT2  (PC7 | ALT2)
#define PC8_ALT1  (PC8 | ALT1)
#define PC9_ALT1  (PC9 | ALT1)
#define PC10_ALT1 (PC10 | ALT1)
#define PC11_ALT1 (PC11 | ALT1)

#define NUM_DIGITAL_PINS  82
#define NUM_DUALPAD_PINS  2
#define NUM_ANALOG_INPUTS 16

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* --- PIN MAP -------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#define PC2 PC2_C
#define PC3 PC3_C

#define ADC_00                      PC4
#define ADC_01                      PC5
#define AUDIOCODEC_I2C_SCL          PB6
#define AUDIOCODEC_I2C_SDA          PB7
#define AUDIOCODEC_SAI1_FS_A        PE4
#define AUDIOCODEC_SAI1_MCLK_A      PE2
#define AUDIOCODEC_SAI1_SCK_A       PE5
#define AUDIOCODEC_SAI1_SD_A        PE6
#define AUDIOCODEC_SAI1_SD_B        PE3
#define BUTTON_PRG                  PB9
#define DAC_00                      PA4
#define DAC_01                      PA5
#define ENCODER_00_A                PA0
#define ENCODER_00_B                PA1
#define ENCODER_00_BUTTON           PA3
#define ENCODER_01_A                PA8
#define ENCODER_01_B                PA9
#define ENCODER_01_BUTTON           PA10
#define GPIO_00                     PD7
#define GPIO_01                     PD4
#define GPIO_02                     PD3
#define GPIO_03                     PD2
#define GPIO_04                     PD11
#define GPIO_05                     PD10
#define GPIO_06                     PD9
#define GPIO_07                     PD8
#define GPIO_08                     PE15
#define GPIO_09                     PE10
#define GPIO_10                     PE9
#define GPIO_11                     PB2
#define GPIO_12                     PC0
#define GPIO_13                     PC15
#define GPIO_14                     PC14
#define GPIO_15                     PC13
#define I2C_USR_SCL                 PB10
#define I2C_USR_SDA                 PB11
#define LED_00                      PB8
#define LED_01                      PB5
#define LED_02                      PB4
#define LED_03                      PC9
#define LED_04                      PC8
#define LED_05                      PC7
#define LED_06                      PC6
#define LED_07                      PD15
#define LED_08                      PD14
#define LED_09                      PD13
#define LED_10                      PD12
#define LED_11                      PB1
#define LED_12                      PB0
#define LED_13                      PA7
#define LED_14                      PA6
#define LED_15                      PA2
#define SDCARD_SPI_CS               PB12
#define SDCARD_SPI_MISO             PC2
#define SDCARD_SPI_MOSI             PC3
#define SDCARD_SPI_SCK              PB13
#define SDCARD_CARD_DETECT          PC1
#define SERIAL_00_RX                PE7
#define SERIAL_00_TX                PE8
#define SERIAL_01_RX                PE0
#define SERIAL_01_TX                PE1
#define SERIAL_DEBUG_RX             PD6
#define SERIAL_DEBUG_TX             PD5
#define SPI_DISPLAY_CS              PA15_ALT1  // was `PA15`
#define SPI_DISPLAY_MISO            PC11
#define SPI_DISPLAY_MOSI            PC12
#define SPI_DISPLAY_REGISTER_SELECT PD0
#define SPI_DISPLAY_RESET           PD1
#define SPI_DISPLAY_SCK             PC10
#define SPI_USR_CS                  PE11
#define SPI_USR_MISO                PE13
#define SPI_USR_MOSI                PE14
#define SPI_USR_SCK                 PE12
#define SWD_SWCLK                   PA14
#define SWD_SWDIO                   PA13
#define SWD_SWO                     PB3
#define USB_DEVICE_DM               PA11
#define USB_DEVICE_DP               PA12
#define USB_HOST_DM                 PB14
#define USB_HOST_DP                 PB15

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* --- HAL ------------------------------------------------------------------ */
/* -------------------------------------------------------------------------- */

#define D_CACHE_DISABLED

#define HAL_MODULE_ENABLED

#if !defined(HAL_ADC_MODULE_DISABLED)
#define HAL_ADC_MODULE_ENABLED
#endif
#if !defined(HAL_DAC_MODULE_DISABLED)
#define HAL_DAC_MODULE_ENABLED
#endif
#ifndef HAL_SAI_MODULE_ENABLED
#define HAL_SAI_MODULE_ENABLED
#endif
#if !defined(HAL_SD_MODULE_DISABLED)
#define HAL_SD_MODULE_ENABLED
#endif
#ifndef HAL_SPI_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#endif
#ifndef HAL_TIM_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#endif
#ifndef HAL_UART_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#endif
#ifndef HAL_PCD_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED
#endif
#ifndef HAL_HCD_MODULE_ENABLED
#define HAL_HCD_MODULE_ENABLED
#endif
#ifndef HAL_GPIO_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#endif
#ifndef HAL_DMA_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#endif
#ifndef HAL_MDMA_MODULE_ENABLED
#define HAL_MDMA_MODULE_ENABLED
#endif
#ifndef HAL_RCC_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#endif
#ifndef HAL_FLASH_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#endif
#ifndef HAL_EXTI_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#endif
#ifndef HAL_PWR_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#endif
#ifndef HAL_I2C_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#endif
#ifndef HAL_CORTEX_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#endif
#ifndef HAL_HSEM_MODULE_ENABLED
#define HAL_HSEM_MODULE_ENABLED
#endif

#define HSE_VALUE            (16000000)
#define HSE_STARTUP_TIMEOUT  (100UL)
#define CSI_VALUE            (4000000UL)
#define HSI_VALUE            (64000000UL)
#define LSE_VALUE            (32768UL)
#define LSE_STARTUP_TIMEOUT  (5000UL)
#define LSI_VALUE            (32000UL)
#define EXTERNAL_CLOCK_VALUE (12288000UL)

#undef USBD_VBUS_DETECTION_ENABLE

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* --- PERIPHERALS ---------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#define KLST_NUM_LEDS          16
#define KLST_NUM_ADCS          2
#define KLST_NUM_DACS          2
#define KLST_ANALOG_RESOLUTION 16
#define LED_BUILTIN            LED_00
#define USER_BTN               ENCODER_00_BUTTON
#define BUTTON_PROGRAMMER      BUTTON_PRG

/* --- U(S)ART -------------------------------------------------------------- */

#define ENABLE_HWSERIAL2
#define PIN_SERIAL2_RX SERIAL_DEBUG_RX
#define PIN_SERIAL2_TX SERIAL_DEBUG_TX
#define ENABLE_HWSERIAL7
#define PIN_SERIAL7_RX SERIAL_00_RX
#define PIN_SERIAL7_TX SERIAL_00_TX
#define ENABLE_HWSERIAL8
#define PIN_SERIAL8_RX SERIAL_01_RX
#define PIN_SERIAL8_TX SERIAL_01_TX

#ifndef SERIAL_UART_INSTANCE
#define SERIAL_UART_INSTANCE 2
#endif

// Default pin used for generic 'Serial' instance
// Mandatory for Firmata
#ifndef PIN_SERIAL_RX
#define PIN_SERIAL_RX SERIAL_DEBUG_RX
#endif
#ifndef PIN_SERIAL_TX
#define PIN_SERIAL_TX SERIAL_DEBUG_TX
#endif

/* --- SPI ------------------------------------------------------------------ */

#ifndef PIN_SPI_SS
#define PIN_SPI_SS SPI_USR_CS
#endif
#ifndef PIN_SPI_SS1
#define PIN_SPI_SS1 PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS2
#define PIN_SPI_SS2 PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS3
#define PIN_SPI_SS3 PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_MOSI
#define PIN_SPI_MOSI SPI_USR_MOSI
#endif
#ifndef PIN_SPI_MISO
#define PIN_SPI_MISO SPI_USR_MISO
#endif
#ifndef PIN_SPI_SCK
#define PIN_SPI_SCK SPI_USR_SCK
#endif

/* --- SdFat --- */

#define SPI_DRIVER_SELECT    3
#define ENABLE_DEDICATED_SPI 1  // set to 0 save memory

/* --- I2C ------------------------------------------------------------------ */

#ifndef PIN_WIRE_SDA
#define PIN_WIRE_SDA I2C_USR_SDA
#endif
#ifndef PIN_WIRE_SCL
#define PIN_WIRE_SCL I2C_USR_SCL
#endif

/* --- TIMER ---------------------------------------------------------------- */

#define ENCODER_01_TIMER   TIM1
#define ENCODER_00_TIMER   TIM2
#define KLST_BEAT_TIMER    TIM6
#define KLST_BUTTON_PRG_IT TIM17

#ifndef TIMER_TONE
#define TIMER_TONE TIM12
#endif
#ifndef TIMER_SERVO
#define TIMER_SERVO TIM7
#endif

#define ARM_MATH_CM7

// #ifdef __cplusplus
// }  // extern "C"
// #endif

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* --- PERIPHERALS ---------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#ifdef __cplusplus

#define KLST_UART_BAUD 115200
#define KLST_SERIAL_00 Serial7
#define KLST_SERIAL_01 Serial8
#define KLST_LOG       Serial
#define Console        Serial

#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#ifndef SERIAL_PORT_MONITOR
#define SERIAL_PORT_MONITOR Serial
#endif
#ifndef SERIAL_PORT_HARDWARE
#define SERIAL_PORT_HARDWARE Serial
#endif

#endif  // __cplusplus

/* -------------------------------------------------------------------------- */

#endif /* _VARIANT_ARDUINO_STM32_ */
