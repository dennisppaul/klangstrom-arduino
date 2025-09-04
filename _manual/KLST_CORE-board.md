---
layout: page
title: KLST_CORE Board
permalink: /klst_core-board/
index: 13
---

KLST_CORE (v0.1) is a development board incorporating a lot of features that are deemed useful for the development of *Klangstrom* applications. it also features  connectors to Eurorack format devices.  it can be programmed via the Arduin IDE through the USB connector ( DFU mode ) or with an external programmer ( SWD mode ).

## Feature Summary

- STM32H743II MCU with 480MHz, 1MB RAM, 2MB Flash
- WM8731 audio codec with 2× audio DAC + 2× audio ADC ( 16/24BIT )
- 6× ADC ( 16BIT, opt 6 extra channels )
- 2× DAC ( 12BIT )
- 3× UART ( serial ) ( IDC connector )
- 3× I2C ( IDC connector )
- 3× SPI ( IDC connector )
- 16× GPIO
- 2× USB ( device or host + power supply )
- 3× rotary encoders with push buttons ( TIM5, TIM1, TIM2 )
- 2× push buttons
- 3× programmable LEDs + 1× power LED
- 13× timers
- 1× SD card reader
- RJ45 connector ( GPIO + UART + SPI )
- Eurorack Power Supply ( 12V )
- Eurorack Connectors for 2× AUDIO DAC + 2× AUDIO ADC 
- 1× JTAG/SWD interface ( 10-pin )
- programmer + reset + boot flash buttons
- 4× Mounting Holes

## Errata

see [Known Issues]({{ site.baseurl }}{% link _manual/known-issues.md %}) ( Application )
