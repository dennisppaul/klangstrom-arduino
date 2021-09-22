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

#ifndef KLST_BOARD_KLST_CORE
#error "@KLST no or wrong board type defined. KLST_BOARD_KLST_CORE not defined!"
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
#define PA0                     PIN_A0
#define PA1                     PIN_A1
#define PA2                     PIN_A2
#define PA3                     PIN_A3
#define PA4                     PIN_A4
#define PA5                     PIN_A5
#define PA6                     PIN_A6
#define PA7                     PIN_A7
#define PA8                     8
#define PA9                     9
#define PA10                    10
#define PA11                    11
#define PA12                    12
#define PA13                    13
#define PA14                    14
#define PA15                    15
#define PB0                     PIN_A8
#define PB1                     PIN_A9
#define PB2                     18
#define PB3                     19
#define PB4                     20
#define PB5                     21
#define PB6                     22
#define PB7                     23
#define PB8                     24
#define PB9                     25
#define PB10                    26
#define PB11                    27
#define PB12                    28
#define PB13                    29
#define PB14                    30
#define PB15                    31
#define PC0                     PIN_A10
#define PC1                     PIN_A11
#define PC4                     PIN_A12
#define PC5                     PIN_A13
#define PC6                     36
#define PC7                     37
#define PC8                     38
#define PC9                     39
#define PC10                    40
#define PC11                    41
#define PC12                    42
#define PC13                    43
#define PC14                    44
#define PC15                    45
#define PD0                     46
#define PD1                     47
#define PD2                     48
#define PD3                     49
#define PD4                     50
#define PD5                     51
#define PD6                     52
#define PD7                     53
#define PD8                     54
#define PD9                     55
#define PD10                    56
#define PD11                    57
#define PD12                    58
#define PD13                    59
#define PD14                    60
#define PD15                    61
#define PE0                     62
#define PE1                     63
#define PE2                     64
#define PE3                     65
#define PE4                     66
#define PE5                     67
#define PE6                     68
#define PE7                     69
#define PE8                     70
#define PE9                     71
#define PE10                    72
#define PE11                    73
#define PE12                    74
#define PE13                    75
#define PE14                    76
#define PE15                    77
#define PF0                     78
#define PF1                     79
#define PF2                     80
#define PF3                     PIN_A14
#define PF4                     PIN_A15
#define PF5                     PIN_A16
#define PF6                     PIN_A17
#define PF7                     PIN_A18
#define PF8                     PIN_A19
#define PF9                     PIN_A20
#define PF10                    PIN_A21
#define PF11                    PIN_A22
#define PF12                    PIN_A23
#define PF13                    PIN_A24
#define PF14                    PIN_A25
#define PF15                    93
#define PG0                     94
#define PG1                     95
#define PG2                     96
#define PG3                     97
#define PG4                     98
#define PG5                     99
#define PG6                     100
#define PG7                     101
#define PG8                     102
#define PG9                     103
#define PG10                    104
#define PG11                    105
#define PG12                    106
#define PG13                    107
#define PG14                    108
#define PG15                    109
#define PH0                     110
#define PH1                     111
#define PH2                     PIN_A26
#define PH3                     PIN_A27
#define PH4                     PIN_A28
#define PH5                     PIN_A29
#define PH6                     116
#define PH7                     117
#define PH8                     118
#define PH9                     119
#define PH10                    120
#define PH11                    121
#define PH12                    122
#define PH13                    123
#define PH14                    124
#define PH15                    125
#define PI0                     126
#define PI1                     127
#define PI2                     128
#define PI3                     129
#define PI4                     130
#define PI5                     131
#define PI6                     132
#define PI7                     133
#define PI8                     134
#define PI9                     135
#define PI10                    136
#define PI11                    137
#define PC2_C                   PIN_A30
#define PC3_C                   PIN_A31

// Alternate pins number
#define PA0_ALT1                (PA0  | ALT1)
#define PA1_ALT1                (PA1  | ALT1)
#define PA1_ALT2                (PA1  | ALT2)
#define PA2_ALT1                (PA2  | ALT1)
#define PA2_ALT2                (PA2  | ALT2)
#define PA3_ALT1                (PA3  | ALT1)
#define PA3_ALT2                (PA3  | ALT2)
#define PA4_ALT1                (PA4  | ALT1)
#define PA4_ALT2                (PA4  | ALT2)
#define PA5_ALT1                (PA5  | ALT1)
#define PA6_ALT1                (PA6  | ALT1)
#define PA7_ALT1                (PA7  | ALT1)
#define PA7_ALT2                (PA7  | ALT2)
#define PA7_ALT3                (PA7  | ALT3)
#define PA9_ALT1                (PA9  | ALT1)
#define PA10_ALT1               (PA10 | ALT1)
#define PA11_ALT1               (PA11 | ALT1)
#define PA12_ALT1               (PA12 | ALT1)
#define PA15_ALT1               (PA15 | ALT1)
#define PA15_ALT2               (PA15 | ALT2)
#define PB0_ALT1                (PB0  | ALT1)
#define PB0_ALT2                (PB0  | ALT2)
#define PB1_ALT1                (PB1  | ALT1)
#define PB1_ALT2                (PB1  | ALT2)
#define PB3_ALT1                (PB3  | ALT1)
#define PB3_ALT2                (PB3  | ALT2)
#define PB4_ALT1                (PB4  | ALT1)
#define PB4_ALT2                (PB4  | ALT2)
#define PB5_ALT1                (PB5  | ALT1)
#define PB5_ALT2                (PB5  | ALT2)
#define PB6_ALT1                (PB6  | ALT1)
#define PB6_ALT2                (PB6  | ALT2)
#define PB7_ALT1                (PB7  | ALT1)
#define PB8_ALT1                (PB8  | ALT1)
#define PB8_ALT2                (PB8  | ALT2)
#define PB9_ALT1                (PB9  | ALT1)
#define PB9_ALT2                (PB9  | ALT2)
#define PB14_ALT1               (PB14 | ALT1)
#define PB14_ALT2               (PB14 | ALT2)
#define PB15_ALT1               (PB15 | ALT1)
#define PB15_ALT2               (PB15 | ALT2)
#define PC0_ALT1                (PC0  | ALT1)
#define PC0_ALT2                (PC0  | ALT2)
#define PC1_ALT1                (PC1  | ALT1)
#define PC1_ALT2                (PC1  | ALT2)
#define PC4_ALT1                (PC4  | ALT1)
#define PC5_ALT1                (PC5  | ALT1)
#define PC6_ALT1                (PC6  | ALT1)
#define PC6_ALT2                (PC6  | ALT2)
#define PC7_ALT1                (PC7  | ALT1)
#define PC7_ALT2                (PC7  | ALT2)
#define PC8_ALT1                (PC8  | ALT1)
#define PC9_ALT1                (PC9  | ALT1)
#define PC10_ALT1               (PC10 | ALT1)
#define PC11_ALT1               (PC11 | ALT1)
#define PF8_ALT1                (PF8  | ALT1)
#define PF9_ALT1                (PF9  | ALT1)

#define NUM_DIGITAL_PINS        140
#define NUM_DUALPAD_PINS        2
#define NUM_ANALOG_INPUTS       32

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#define D_CACHE_DISABLED

#define HAL_MODULE_ENABLED

#ifndef HAL_I2S_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#endif
#ifndef HAL_SPI_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#endif
#ifndef HAL_TIM_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
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

#define HSE_VALUE               (16000000)
#define HSE_STARTUP_TIMEOUT     (100UL)
#define CSI_VALUE               (4000000UL)
#define HSI_VALUE               (64000000UL)
#define LSE_VALUE               (32768UL)
#define LSE_STARTUP_TIMEOUT     (5000UL)
#define LSI_VALUE               (32000UL)
#define EXTERNAL_CLOCK_VALUE    (12288000UL)

#undef USBD_VBUS_DETECTION_ENABLE

/* -------------------------------------------------------------------------- */

// // On-board LED pin number
// #ifndef LED_BUILTIN
//   #define LED_BUILTIN           PNUM_NOT_DEFINED
// #endif

// // On-board user button
// #ifndef USER_BTN
//   #define USER_BTN              PNUM_NOT_DEFINED
// #endif

// // SPI definitions
// #ifndef PIN_SPI_SS
//   #define PIN_SPI_SS            PA4
// #endif
// #ifndef PIN_SPI_SS1
//   #define PIN_SPI_SS1           PA15
// #endif
// #ifndef PIN_SPI_SS2
//   #define PIN_SPI_SS2           PG10
// #endif
// #ifndef PIN_SPI_SS3
//   #define PIN_SPI_SS3           PNUM_NOT_DEFINED
// #endif
// #ifndef PIN_SPI_MOSI
//   #define PIN_SPI_MOSI          PA7
// #endif
// #ifndef PIN_SPI_MISO
//   #define PIN_SPI_MISO          PA6
// #endif
// #ifndef PIN_SPI_SCK
//   #define PIN_SPI_SCK           PA5
// #endif

// // I2C definitions
// #ifndef PIN_WIRE_SDA
//   #define PIN_WIRE_SDA          PB7
// #endif
// #ifndef PIN_WIRE_SCL
//   #define PIN_WIRE_SCL          PB6
// #endif

// // Timer Definitions
// // Use TIM6/TIM7 when possible as servo and tone don't need GPIO output pin
// #ifndef TIMER_TONE
//   #define TIMER_TONE            TIM6
// #endif
// #ifndef TIMER_SERVO
//   #define TIMER_SERVO           TIM7
// #endif

// // UART Definitions
// #ifndef SERIAL_UART_INSTANCE
//   #define SERIAL_UART_INSTANCE  4
// #endif

// // Default pin used for generic 'Serial' instance
// // Mandatory for Firmata
// #ifndef PIN_SERIAL_RX
//   #define PIN_SERIAL_RX         PA1
// #endif
// #ifndef PIN_SERIAL_TX
//   #define PIN_SERIAL_TX         PA0
// #endif

// Extra HAL modules
#if !defined(HAL_DAC_MODULE_DISABLED)
  #define HAL_DAC_MODULE_ENABLED
#endif
// #if !defined(HAL_ETH_MODULE_DISABLED)
//   #define HAL_ETH_MODULE_ENABLED
// #endif
// #if !defined(HAL_QSPI_MODULE_DISABLED)
//   #define HAL_QSPI_MODULE_ENABLED
// #endif
#if !defined(HAL_SD_MODULE_DISABLED)
  #define HAL_SD_MODULE_ENABLED
#endif

#ifdef __cplusplus
} // extern "C"
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
    #define SERIAL_PORT_MONITOR   Serial
  #endif
  #ifndef SERIAL_PORT_HARDWARE
    #define SERIAL_PORT_HARDWARE  Serial
  #endif
#endif

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

/* --------------- */
#ifdef __cplusplus
extern "C" {
#endif
/* --------------- */

/* GPIO */

#define GPIO_00	PE7
#define GPIO_01	PE3
#define GPIO_02	PE0
#define GPIO_03	PC13
#define GPIO_04	PF2
#define GPIO_05	PF3
#define GPIO_06	PF4
#define GPIO_07	PF10
#define GPIO_08	PC0
#define GPIO_09	PC1
#define GPIO_10	PC4
#define GPIO_11	PC5
#define GPIO_12	PB0
#define GPIO_13	PB1
#define GPIO_14	PG0
#define GPIO_15	PG1

/* BUTTON */

#define BUTTON_PROGRAMMER	PG2
#define BUTTON_USER_00	  PD15
#define BUTTON_USER_01	  PD14

/* LED */

#define LED_00	PG5
#define LED_01	PG3
#define LED_02	PG4

/* ADC+DAC */

#define ADC_00	PF5
#define ADC_01	PF11
#define ADC_02	PF13
#define ADC_03	PF6
#define ADC_04	PF12
#define ADC_05	PF14

#define DAC_00	PA5
#define DAC_01	PA4

/* USART */

#define UART_00_RX	PG9
#define UART_00_TX	PG14
#define UART_01_RX	PA10
#define UART_01_TX	PA9
#define UART_02_RX	PA3
#define UART_02_TX	PA2

/* ENCODER */

#define ENCODER_00_A	    PH10
#define ENCODER_00_B	    PH11
#define ENCODER_00_BUTTON	PG8
#define ENCODER_01_A	    PE9
#define ENCODER_01_B	    PE11
#define ENCODER_01_BUTTON	PG6
#define ENCODER_02_A	    PA0
#define ENCODER_02_B	    PA1
#define ENCODER_02_BUTTON	PG7

/* SPI */

#define SPI_00_MISO	PF8
#define SPI_00_MOSI	PF9
#define SPI_00_SCK	PF7
#define SPI_01_MISO	PA6
#define SPI_01_MOSI	PD7
#define SPI_01_SCK	PG11
#define SPI_02_MISO	PG12
#define SPI_02_MOSI	PA7
#define SPI_02_SCK	PG13

/* I2C */

#define I2C_00_SCL	PF1
#define I2C_00_SDA	PF0
#define I2C_01_SCL	PB6
#define I2C_01_SDA	PB7
#define I2C_02_SCL	PD12
#define I2C_02_SDA	PF15

/* AUDIOCODEC */

#define AUDIOCODEC_I2C_SCL	PA8
#define AUDIOCODEC_I2C_SDA	PC9
#define AUDIOCODEC_I2S_CK	  PB10
#define AUDIOCODEC_I2S_MCK	PC6
#define AUDIOCODEC_I2S_SDI	PI2
#define AUDIOCODEC_I2S_SDO	PI3
#define AUDIOCODEC_I2S_WS	  PB12

/* RJ45 */

#define RJ45_SPI_MISO	      PC11
#define RJ45_SPI_MOSI	      PC12
#define RJ45_SPI_SCK	      PC10
#define RJ45_UART_RX	      PD9
#define RJ45_UART_TX	      PD8

/* SD CARD */

#define SDCARD_CS	          PE4
#define SDCARD_DETECT_SDIO	PE1
#define SDCARD_MISO     	  PE5
#define SDCARD_MOSI	        PE6
#define SDCARD_SCK	        PE2

/* USB */

#define USB_DEVICE_DM	      PA11
#define USB_DEVICE_DP	      PA12
#define USB_HOST_DM	        PB14
#define USB_HOST_DP	        PB15

/* --- PERIPHERALS ---------------------------------------------------------- */

/* USART */
// @todo(maybe add option to def out serial support)
#define ENABLE_HWSERIAL6
#define PIN_SERIAL6_RX      UART_00_RX
#define PIN_SERIAL6_TX      UART_00_TX
#define ENABLE_HWSERIAL1
#define PIN_SERIAL1_RX      UART_01_RX
#define PIN_SERIAL1_TX      UART_01_TX
#define ENABLE_HWSERIAL2
#define PIN_SERIAL2_RX      UART_02_RX
#define PIN_SERIAL2_TX      UART_02_TX

#define KLST_UART_BAUD      115200

/* ONBOARD + DEFAULT */

#define LED_BUILTIN         LED_00
#define USER_BTN            BUTTON_USER_00

/* SPI */
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS        SDCARD_CS
#endif
#ifndef PIN_SPI_SS1
  #define PIN_SPI_SS1       PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS2
  #define PIN_SPI_SS2       PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS3
  #define PIN_SPI_SS3       PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_MOSI
  #define PIN_SPI_MOSI      SDCARD_MOSI
#endif
#ifndef PIN_SPI_MISO
  #define PIN_SPI_MISO      SDCARD_MISO
#endif
#ifndef PIN_SPI_SCK
  #define PIN_SPI_SCK       SDCARD_SCK
#endif

/* I2C */
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA      I2C_00_SDA
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL      I2C_00_SCL
#endif

/* TIMER */

#define ENCODER_00_TIMER    TIM5
#define ENCODER_01_TIMER    TIM1
#define ENCODER_02_TIMER    TIM2
#define KLST_BEAT_TIMER     TIM6
#ifndef TIMER_TONE
  #define TIMER_TONE            TIM13
#endif
#ifndef TIMER_SERVO
  #define TIMER_SERVO           TIM7
#endif

/* USART */
#ifndef SERIAL_UART_INSTANCE
  #define SERIAL_UART_INSTANCE  6
#endif

// Default pin used for generic 'Serial' instance
// Mandatory for Firmata
#ifndef PIN_SERIAL_RX
  #define PIN_SERIAL_RX         UART_00_RX
#endif
#ifndef PIN_SERIAL_TX
  #define PIN_SERIAL_TX         UART_00_TX
#endif

/* --------------- */
#ifdef __cplusplus
}
#endif
/* --------------- */

/* --------------- */
#ifdef __cplusplus
/* --------------- */

/* USART */
#define KLST_SERIAL_00          Serial6
#define KLST_SERIAL_01          Serial1
#define KLST_SERIAL_02          Serial2
#define SerialDebug             Serial
#define KLST_LOG                Serial

/* --------------- */
#endif
/* --------------- */

/* -------------------------------------------------------------------------- */

#endif /* _VARIANT_ARDUINO_STM32_ */
