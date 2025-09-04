---
layout: page
title: How to Upload or Run Applications
permalink: /how-to-upload-or-run-applications/
index: 20
---

*Klangstrom* applications can be run on different platforms. currently there are two active platforms[^1]:

- `MCU` :: applications run on microcontrollers (MCU)
- `DESKTOP` :: applications run on desktop computers

a platform can be selected under `Tools > Board > Klangstrom (…)`.

## Platform `MCU`

the `MCU` platform supports different types of microcontrollers. currently it only supports the STM32-based `KLST_TINY` board[^2]. the family of STM32-based microcontrollers can be selected under `Tools > Board > Klangstrom (MCU) > Klangstrom (STM32)`.

### Board `KLST_TINY`

the board can be selected under `Tools > Board part number > KLST_TINY(STM32F446RE)`. once selected there are two different upload options available:

- `DFU`
- `SWD`

the upload method can be selected under `Tools > Upload Method > STM32CubeProgrammer (…)`.

#### Upload Method `DFU`

the *Device Firmware Update* (DFU) mode allows the board to be programmed via the USB connector labeled `DEVICE+PWR`. in order to do so the board first needs to be set to DFU mode. the standard way to achieved this is by pressing the button on the back of the board labeled `PRG`. once the button is pressed all three LEDs start flashing in a pattern for a few seconds. now the board is in DFU mode.

the board can be programmed via `Sketch > Upload`.

if the LEDs did not flash in a pattern or the programming fails, it is likely that the board did not enter DFU mode. the reason for this is usually that the MCU has crashed. in order to recover the board DFU mode must be entered *manually*. this can be achieve by pressing + holding down the `BOOT` button, then pressing + releasing the `RESET` button and then releasing the `BOOT` button again ( it takes a bit of practice because the buttons are so small). if the procedure was executed correctly the board is now in DFU mode.

#### Upload Method `SWD`

the `SWD` mode allows the board to be programmed via an external ST-LINK programmer ( e.g [STLINK-V3MINI](https://www.st.com/content/st_com/en/products/development-tools/hardware-development-tools/hardware-development-tools-for-stm32/stlink-v3mini.html) ). the programmer can be connected to the board via the connector labeled `SWD`.

the main benefit of this option is that it does not required to press a button prior to uploading an application. this option also becomes particularly useful when wanting to use the USB port for other applications ( e.g USB HID or USB MIDI ).

## Platform `DESKTOP`

the `DESKTOP` platform allows to run *Klangstrom* applications in a desktop computer simulator. it can be selected under `Tools > Board > Klangstrom (DESKTOP) > Klangstrom (SDL)`.

---

[^1]: there is a third platform `VCV Rack` which runs applications as a module in the VCV Rack environment. this platform will be released in the future.
[^2]: the `KLST_CORE` board has already been produced and will be released in the near future.
