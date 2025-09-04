---
layout: page
title: Working with GPIOs + DACs + ADCs
permalink: /working-with-GPIOs+DACs+ADCs/
index: 33
---

pins marked as `GPIO` + `DAC` + `ADC` can be used in the same ways as known from any other arduino environments.

![KLST_TINY--schematics--pin-map]({{ site.baseurl }}{% link assets/KLST_TINY--schematics--pin-map.png %})

on the [KLST_TINY board]({{ site.baseurl }}{% link _manual/KLST_TINY-board.md %}) there are two sections of 2×10 pin rows ( marked `05`–`08` + `11`–`14` on the schematic drawing ) incorporating in addition to `GPIO` + `DAC` + `ADC` several additional peripherals like `SERIAL`, `I2C`, `SPI`, audio in- + output and power supply[^1].

## `GPIO` pins

*Klangstrom* provides 8 `GPIO` pins, named `GPIO_00` to `GPIO_07`. these pins can be used as digital in- and outputs via the known arduino idiom `digitalRead()` + `digitalWrite()`:

```c
pinMode(GPIO_00, OUTPUT);
digitalWrite(GPIO_00, HIGH);

pinMode(GPIO_01, INPUT);
int mState = digitalRead(GPIO_01);
```

note that all pins markes as `ADC`, `DAC`, `SERIAL`, `I2C`, and `SPI` can also be used as `GPIO` pins if need be[^2].

the table below shows all available `GPIO` pin with 3 different naming conventions for the same physical pins:

| KLST       | STM32 | ARDUINO | 
|------------|-------|---------|
| GPIO_00    | PC13  | D23     |
| GPIO_01    | PC14  | D24     |
| GPIO_02    | PC15  | D25     |
| GPIO_03    | PC2   | D28     |
| GPIO_04    | PC3   | D29     |
| GPIO_05    | PA2   | D1      |
| GPIO_06    | PA3   | D0      |
| GPIO_07    | PC5   | D35     |

## `ADC` + `DAC` pins 

*Klangstrom* provides 2 analog input ( named `ADC_00` + `ADC_01` ) and 2 analog output ( named `DAC_00` + `DAC_01` ) pins. these pins can be used as analog in- and outputs via the known arduino idiom `analogRead()` + `analogWrite()`:

```c
int mValue = analogRead(ADC_00);

pinMode(DAC_00, OUTPUT);
analogWrite(DAC_00, mValue);
```

note that 5 of the `GPIO` pins can also be used `ADC` pins. 

the table below shows all available `ADC` capable pins with 3 different naming conventions for the same physical pins:

| KLST       | STM32 | ARDUINO | 
|------------|-------|---------|
| ADC_00     | PB0   | A3      |
| ADC_01     | PB1   | A12     |
|            |       |         |
| GPIO_03    | PC2   | A9      |
| GPIO_04    | PC3   | A10     |
| GPIO_07    | PC5   | A11     |
| GPIO_05    | PA2   | D1*     |
| GPIO_06    | PA3   | D0*     |

*) not tested

note that non of the `GPIO` pins can be used as an additional `DAC`. the board is limited to just the two. 

---

[^1]: note, that due to the nature of the KLST_TINY MCU ( STM32F446RET ) most pins can be configured to have multiple functions e.g some `GPIO` pin can act as `ADC` pins, while other can also act as serial ports or `I2C` interfaces. consult the STM32F446RET manual for further information of pin functionalities.
[^2]: technically 
