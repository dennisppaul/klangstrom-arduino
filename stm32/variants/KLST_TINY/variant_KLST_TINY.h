/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_ARDUINO_STM32_
#define _VARIANT_ARDUINO_STM32_

#ifndef KLST_BOARD_KLST_TINY
#error "@KLST no or wrong board type defined. KLST_BOARD_KLST_TINY not defined!"
#endif

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
#define PA0  PIN_A0
#define PA1  PIN_A1
#define PA2  PIN_A2
#define PA3  PIN_A3
#define PA4  PIN_A4
#define PA5  PIN_A5
#define PA6  PIN_A6
#define PA7  PIN_A7
#define PA8  8
#define PA9  9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15
#define PB0  PIN_A8
#define PB1  PIN_A9
#define PB2  18
#define PB3  19
#define PB4  20
#define PB5  21
#define PB6  22
#define PB7  23
#define PB8  24
#define PB9  25
#define PB10 26
#define PB12 27
#define PB13 28
#define PB14 29
#define PB15 30
#define PC0  PIN_A10
#define PC1  PIN_A11
#define PC2  PIN_A12
#define PC3  PIN_A13
#define PC4  PIN_A14
#define PC5  PIN_A15
#define PC6  37
#define PC7  38
#define PC8  39
#define PC9  40
#define PC10 41
#define PC11 42
#define PC12 43
#define PC13 44
#define PC14 45
#define PC15 46
#define PD2  47
#define PH0  48
#define PH1  49

// Alternate pins number
#define PA0_ALT1  (PA0 | ALT1)
#define PA0_ALT2  (PA0 | ALT2)
#define PA1_ALT1  (PA1 | ALT1)
#define PA1_ALT2  (PA1 | ALT2)
#define PA2_ALT1  (PA2 | ALT1)
#define PA2_ALT2  (PA2 | ALT2)
#define PA3_ALT1  (PA3 | ALT1)
#define PA3_ALT2  (PA3 | ALT2)
#define PA4_ALT1  (PA4 | ALT1)
#define PA5_ALT1  (PA5 | ALT1)
#define PA6_ALT1  (PA6 | ALT1)
#define PA7_ALT1  (PA7 | ALT1)
#define PA7_ALT2  (PA7 | ALT2)
#define PA7_ALT3  (PA7 | ALT3)
#define PA15_ALT1 (PA15 | ALT1)
#define PB0_ALT1  (PB0 | ALT1)
#define PB0_ALT2  (PB0 | ALT2)
#define PB1_ALT1  (PB1 | ALT1)
#define PB1_ALT2  (PB1 | ALT2)
#define PB3_ALT1  (PB3 | ALT1)
#define PB4_ALT1  (PB4 | ALT1)
#define PB5_ALT1  (PB5 | ALT1)
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
#define PC2_ALT1  (PC2 | ALT1)
#define PC2_ALT2  (PC2 | ALT2)
#define PC3_ALT1  (PC3 | ALT1)
#define PC3_ALT2  (PC3 | ALT2)
#define PC4_ALT1  (PC4 | ALT1)
#define PC5_ALT1  (PC5 | ALT1)
#define PC6_ALT1  (PC6 | ALT1)
#define PC7_ALT1  (PC7 | ALT1)
#define PC8_ALT1  (PC8 | ALT1)
#define PC9_ALT1  (PC9 | ALT1)
#define PC10_ALT1 (PC10 | ALT1)
#define PC11_ALT1 (PC11 | ALT1)

#define NUM_DIGITAL_PINS  50
#define NUM_ANALOG_INPUTS 16

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* --- HAL ------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#define HAL_SAI_MODULE_ENABLED

#ifdef USE_TINYUSB  // @todo(clean this up!!!)
/*******************************************************/
/* TinyUSB */
#define HAL_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_SAI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_EXTI_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HSE_STARTUP_TIMEOUT 100U
#define HSI_VALUE           ((uint32_t)16000000U)
#define LSI_VALUE           32000U
#define LSE_STARTUP_TIMEOUT 5000U
/*******************************************************/
#endif  // USE_TINYUSB

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* --- PIN MAP -------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#define GPIO_00           PC13
#define GPIO_01           PC14
#define GPIO_02           PC15
#define GPIO_03           PC2
#define GPIO_04           PC3
#define GPIO_05           PA2
#define GPIO_06           PA3
#define GPIO_07           PC5
#define ADC_00            PB0
#define ADC_01            PB1
#define DAC_00            PA4
#define DAC_01            PA5
#define LED_00            PB5
#define LED_01            PD2
#define LED_02            PC12
#define I2C_00_SCL        PB8
#define I2C_00_SDA        PB7
#define SPI_00_MISO       PB14
#define SPI_00_MOSI       PB15
#define SPI_00_SCK        PB13
#define UART_00_RX        PA10
#define UART_00_TX        PB6
#define UART_01_RX        PC11
#define UART_01_TX        PC10
#define ENCODER_00_BUTTON PC4
#define ENCODER_01_BUTTON PC8
#define ENCODER_02_BUTTON PB4

/* -------------------------------------------------------------------------- */
/* --- KLST ----------------------------------------------------------------- */
/* --- PERIPHERALS ---------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

// Timer Definitions
// Use TIM6/TIM7 when possible as servo and tone don't need GPIO output pin
#define TIMER_TONE  TIM6
#define TIMER_SERVO TIM7

// UART Definitions
#define SERIAL_UART_INSTANCE 1
#define PIN_SERIAL_RX        UART_00_RX
#define PIN_SERIAL_TX        UART_00_TX

#define HSE_VALUE 16000000U

#define BUTTON_PROGRAMMER      PC5
#define KLST_NUM_LEDS          3
#define KLST_NUM_ADCS          2
#define KLST_NUM_DACS          2
#define KLST_ANALOG_RESOLUTION 16
#define KLST_UART_BAUD         115200

#ifndef LED_BUILTIN
#define LED_BUILTIN LED_00
#endif

#define ENCODER_00_TIMER TIM3
#define ENCODER_01_TIMER TIM8
#define ENCODER_02_TIMER TIM2
#define KLST_BEAT_TIMER  TIM5

/* UART */

// @todo(maybe add option to def out serial support)
#define ENABLE_HWSERIAL1
#define PIN_SERIAL1_RX UART_00_RX
#define PIN_SERIAL1_TX UART_00_TX
#define ENABLE_HWSERIAL4
#define PIN_SERIAL4_RX UART_01_RX
#define PIN_SERIAL4_TX UART_01_TX

/* SPI */

// @todo(this definition is for KLST_TINYv0.1! future versions have different pin mappings)
#define PIN_SPI_MOSI SPI_00_MOSI
#define PIN_SPI_MISO SPI_00_MISO
#define PIN_SPI_SCK  SPI_00_SCK
// #define PIN_SPI_SS   // @note(there is no CS/SS pin available in KLST_TINYv0.1)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define KLST_SERIAL_00 Serial1
#define KLST_SERIAL_01 Serial4
#define SerialDebug    Serial
#define KLST_LOG       Serial
#endif

#define ARM_MATH_CM4

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
#define SERIAL_PORT_MONITOR  Serial
#define SERIAL_PORT_HARDWARE Serial
#endif

#endif /* _VARIANT_ARDUINO_STM32_ */
