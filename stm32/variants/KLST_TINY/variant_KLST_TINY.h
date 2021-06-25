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

#define HAL_SAI_MODULE_ENABLED
#define KLST_BOARD_TYPE KLST_TINY

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

#define PA3   0
#define PA2   1
#define PA10  2
#define PB3   3
#define PB5   4
#define PB4   5
#define PB10  6
#define PA8   7
#define PA9   8
#define PC7   9
#define PB6   10
#define PA7   A6
#define PA6   A7
#define PA5   A8 // LD2
#define PB9   14
#define PB8   15
// ST Morpho
// CN7 Left Side
#define PC10  16
#define PC12  17
// 18 is NC - BOOT0
#define PA13  19 // SWD
#define PA14  20 // SWD
#define PA15  21
#define PB7   22
#define PC13  23 // USER_BTN
#define PC14  24 // NC by default SB49 opened
#define PC15  25 // NC by default SB48 opened
#define PH0   26 // NC by default SB55 opened
#define PH1   27
#define PC2   A9
#define PC3   A10
// CN7 Right Side
#define PC11  30
#define PD2   31
// CN10 Left Side
#define PC9   32
// CN10 Right side
#define PC8   33
#define PC6   34
#define PC5   A11
#define PA12  36
#define PA11  37
#define PB12  38
// 39 is NC
#define PB2   40
#define PB1   A12
#define PB15  42
#define PB14  43
#define PB13  44
#define PC4   A13
#define PA0   A0
#define PA1   A1
#define PA4   A2
#define PB0   A3
#define PC1   A4
#define PC0   A5

// This must be a literal
#define NUM_DIGITAL_PINS        52
// This must be a literal with a value less than or equal to to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       14

// On-board user button
// #define USER_BTN                PC13

// Timer Definitions
// Use TIM6/TIM7 when possible as servo and tone don't need GPIO output pin
#define TIMER_TONE              TIM6
#define TIMER_SERVO             TIM7

// UART Definitions
#define SERIAL_UART_INSTANCE    2 //Connected to ST-Link
// Default pin used for 'Serial' instance (ex: ST-Link)
// Mandatory for Firmata
#define PIN_SERIAL_RX           0
#define PIN_SERIAL_TX           1

/* Extra HAL modules */
#define HAL_DAC_MODULE_ENABLED

#define HSE_VALUE 16000000

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
  #define SERIAL_PORT_MONITOR     Serial
  #define SERIAL_PORT_HARDWARE    Serial
#endif

/* --------------------------------------------------------------- */
/* KLST                                                            */
/* --------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* PIN LABELS */

#define GPIO_00	                  PC13
#define GPIO_01	                  PC14
#define GPIO_02	                  PC15
#define GPIO_03	                  PC2
#define GPIO_04	                  PC3
#define GPIO_05	                  PA2
#define GPIO_06	                  PA3
#define GPIO_07	                  PC5

#define ADC_00	                  PB0
#define ADC_01	                  PB1
#define DAC_00	                  PA4
#define DAC_01	                  PA5

#define LED_00	                  PB5
#define LED_01	                  PD2
#define LED_02	                  PC12

#define I2C_00_SCL	              PB8
#define I2C_00_SDA	              PB7
#define SPI_00_MISO	              PB14
#define SPI_00_MOSI	              PB15
#define SPI_00_SCK	              PB13

#define UART_00_RX	              PA10
#define UART_00_TX	              PB6
#define UART_01_RX	              PC11
#define UART_01_TX	              PC10

#define ENCODER_00_BUTTON         PC4
#define ENCODER_01_BUTTON         PC8
#define ENCODER_02_BUTTON         PB4

/* --- */

#define BUTTON_PROGRAMMER         PC5
#define LED_BUILTIN               LED_00
#define KLST_UART_BAUD            115200

#define ENCODER_00_TIMER          TIM3
#define ENCODER_01_TIMER          TIM8
#define ENCODER_02_TIMER          TIM2

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
  #define KLST_SERIAL_00          Serial1
  #define KLST_SERIAL_01          Serial4
  #define KLST_LOG                Serial
#endif

/* serial */

// @todo(maybe add option to def out serial support)
#define ENABLE_HWSERIAL1
#define PIN_SERIAL1_RX            UART_00_RX
#define PIN_SERIAL1_TX            UART_00_TX
#define ENABLE_HWSERIAL4
#define PIN_SERIAL4_RX            UART_01_RX
#define PIN_SERIAL4_TX            UART_01_TX

/* SPI */

// @todo(this definition is for KLST_TINYv0.1! future versions have different pin mappings)
#define PIN_SPI_MOSI              SPI_00_MOSI
#define PIN_SPI_MISO              SPI_00_MISO
#define PIN_SPI_SCK               SPI_00_SCK
// #define PIN_SPI_SS   // @note(there is no CS/SS pin available in KLST_TINYv0.1)

#endif /* _VARIANT_ARDUINO_STM32_ */
