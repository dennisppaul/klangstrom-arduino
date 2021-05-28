---
layout: page
title: KLST_TINY Board
permalink: /klst_tiny-board/
index: 11
---

KLST_TINY (v0.1) is a pocket-calculator-sized development board with 3 push-button encoders, 3 LEDs and 2 serial ports, based on STM32F446 MCU. it aims to facilitate the development of smaller audio applications. the board can be connected to other KLST_TINY boards via the serial IDC connector ( 15 + 21 ). it can be programmed via the Arduin IDE through the USB connector ( DFU mode ) or with an external programmer ( SWD mode ).

![KLST_TINY--schematics]({{ site.baseurl }}{% link assets/KLST_TINY--schematics.png %})

- 01 :: audio input, line-in ( 3.5mm ) :: line level input for audio signals ( e.g connection to computer audio output )
- 02 :: audio output, line-out ( 3.5mm ) :: line level output for audio signals ( e.g connection to HiFi systems )
- 03 :: 3× programmable LEDs ( `LED_00` + `LED_01` + `LED_02` ) :: user programmable LEDs
- 04 :: 3× rotary encoders with push buttons
- 05 :: 3.3/5V power pins :: power in-+output pins. can either used to power the board or to power peripherals if board is powered via USB
- 06 :: 8× GPIO pins :: programmable GPIO pins, note that the pins are only 3.3V tolerant
- 07 :: 2× ADCs + 2× DACs ( 12BIT ) :: additional ADCs + DACs with lower 12BIT resolution
- 08 :: 3.3/5V power pins :: see 05
- 09 :: 4× mounting holes ( M3 )
- 10 :: headphones + mic ( 3.5mm ) :: 3 ring audio jack with stereo output and mono input. can be used to connect e.g a *headset*. might require a OMTP to CTIA/AHJ converter.
- 11 :: 2× UART ( serial ) pins ( `SERIAL_00` + `SERIAL_01` ) :: pin out for serial ports. same ports as 15 + 21
- 12 :: 1× I2C pins
- 13 :: 1× SPI pins
- 14 :: audio in-+output pins, line-in+out :: same as 01 + 02
- 15 :: IDC connector with power + UART ( serial ) ( `SERIAL_00` ) :: serial port with IDC connector. requires special *twisted* cable. can be used to connect e.g to other KLST_TINY boards.
- 16 :: STM32F446RET microcontroller ( MCU )
- 17 :: JTAG/SWD interface ( 10-pin ) :: interface for external programmer e.g ( STLINK-V3 )
- 18 :: SD card reader
- 19 :: power LED
- 20 :: USB-B micro + PWR :: USB port for connecting board as *USB device*. can also be used to program and power the board. 
- 21 :: IDC connector with power + UART ( serial ) ( `SERIAL_01` ) :: see 15
- 22 :: WM8731 Audio Codec
- 23 :: boot flash button :: select boot mode. if button is held down during reset board starts in DFU mode
- 24 :: reset button :: resets board
- 25 :: programmer button :: sends board into DFU mode ( LEDs flash in blinking pattern )
- 26 :: USB-B micro :: USB port for connecting board as *USB host* to e.g computer mouse or keyboard

## feature summary

- STM32F446 MCU with 180MHz, 128KB RAM, 512KB Flash
- WM8731 audio codec with 2× audio DAC + 2× audio ADC ( 16/24BIT )
- 2× ADC ( 12BIT, opt 6 extra channels )
- 2× DAC ( 12BIT )
- 3× UART ( serial )
- 8× GPIO
- 1× USB ( device or host )
- 1× I2C 
- 1× SPI 
- 3× rotary encoders with push buttons ( TIM2, TIM3, TIM8 )
- 3× programmable LEDs + 1× power LED
- 11× timers
- 1× SD card reader
- 1× JTAG/SWD interface ( 10-pin )
- programmer + reset + boot flash buttons
- 4× Mounting Holes