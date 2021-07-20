---
layout: libdoc
title: option
permalink: /klangstrom/option/
index: 203
tag: library
---

configure application and peripherals specific properties and behaviors.

```c
void option(uint8_t pOption, uint8_t pValue);
```

the following options are available:

- `KLST_OPTION_AUDIO_INPUT` :: select audio input ( `KLST_MIC` or `KLST_LINE_IN` ) ( default: `KLST_MIC` )
- `KLST_OPTION_ENCODERS` :: enable/disable encoders ( default: `KLST_ENABLE` )
- `KLST_OPTION_SERIAL_PORTS` :: enable/disable initialization of serial ports ( default: `KLST_ENABLE` )
- `KLST_OPTION_BEAT` :: enable/disable beat callback ( default: `KLST_ENABLE` )
- `KLST_OPTION_PROGRAMMER_BUTTON` :: enable/disable programmer button ( labeled `PRG` ) ( default: `KLST_ENABLE` )
- `KLST_OPTION_ENABLE_AUDIO_INPUT` :: enable/disable audio input processing ( default: `KLST_ENABLE` )
- `KLST_OPTION_SERIAL_00_BAUD_RATE` :: set serial port `SERIAL_00` baud rate ( default: `KLST_UART_BAUD` or `115200` )
- `KLST_OPTION_SERIAL_01_BAUD_RATE` :: set serial port `SERIAL_01` baud rate ( default: `KLST_UART_BAUD` or `115200` )

note, that options might interpreted differently in different contexts.

the `option` function must be called in `setup()`.

## example

```c
void setup() {
    // switch audio input from `MIC` to `INPUT`
    option(KLST_OPTION_AUDIO_INPUT, KLST_LINE_IN);
}
```