---
layout: page
title: Known Issues
permalink: /known-issues/
index: 666
---

*Klangstrom* is a project in a very experimental state. there are a number of known issues. in addition to the following list, make sure to also consult the [issues tracking system](https://github.com/dennisppaul/klangstrom-arduino/issues) where bugs + feature requests are submitted.

## Software

### Klangstrom 

- desktop simulator (`SDL`) currently only works in MacOS
- desktop simulator (`SDL`) lacks most arduino functions

## Hardware

### KLST_TINY v0.1

- *SD Card reader* is not working. `CS` is not connected properly.
- `HEADPHONE+MIC` ( TRRS audio jack ) requires adapter OMTP to CTIA/AHJ
- `SERIAL_00` + `SERIAL_01` require dedicated *twisted* cable
- `GPIO_07` is also used by the programmer button 

### KLST_CORE v0.1

- `5/12V` power switch does not work. jumper `PATCH_5V` needs to used to power the board.
- USB connectors ( `HOST` + `DEVICE+PWR` ) are switched. in *Arduino IDE* this can be rectified by selecting `High Speed in Full Speed mode` ti use `DEVICE+PWR` connector and `Low/Full Speed` to use `HOST`. however, for `DFU` upload to work a custom USB-A-to-USB-A cable must be used on `HOST` connector. 
