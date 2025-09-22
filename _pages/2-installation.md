---
layout: page
title: Installation
permalink: /installation/
---

this instruction guide through the *manual* installation process: 

## Install Prerequisite

install *Arduino IDE*, *Arduino_Core_STM32*, and *Umfeld for Arduino* by following the instructions provided on the projects’ websites:

- install *Arduino IDE* (v2.3.6) by following instructions at [Arduino IDE](https://www.arduino.cc/en/software/#ide) and/or install [`arduino-cli`](https://docs.arduino.cc/arduino-cli/installation/) (v1.3.1) 
- install *Arduino_Core_STM32* (v2.11.0) by following instructions at [STM32duino / Arduino_Core_STM32](https://github.com/stm32duino/Arduino_Core_STM32?tab=readme-ov-file#getting-started)
- install *Umfeld* (v2.4.1) by following instructions at [Umfeld](https://github.com/dennisppaul/umfeld?tab=readme-ov-file#quickstart)
- install *Umfeld for Arduino* (v2.4.1) by following instructions at [Umfeld for Arduino](https://github.com/dennisppaul/umfeld-arduino)

## Install Klangstrom Boards + Emulator

note, the installation below assumes that the paths to the *Arduino Sketchbook* folder and the *Arduino15 Data* folder are known. in a default installation the folders are in the following locations:

```sh
# macOS
ARDUINO_SKETCHBOOK_PATH=~/Documents/Arduino/
ARDUINO15_DATA_PATH=~/Library/Arduino15/
# Linux
ARDUINO_SKETCHBOOK_PATH=~/Arduino/
ARDUINO15_DATA_PATH=~/.arduino15/
# Windows
ARDUINO_SKETCHBOOK_PATH=C:\Users\<username>\Documents\Arduino\
ARDUINO15_DATA_PATH=C:\Users\<username>\AppData\Local\Arduino15\
```

- download latest release of [Klangstrom for Arduino](https://github.com/Klangstrom/klangstrom-arduino/archive/refs/tags/v1.0.0.zip) (v1.0.0) as `.zip` and unpack:
    ```sh
    klangstrom-arduino
    ├── libraries
    │   ├── CODING-STYLE.md
    │   ├── Klangstrom
    │   ├── Klangstrom_Emulator
    │   ├── Klangstrom_KLST_EMU
    │   ├── Klangstrom_KLST_PANDA_STM32
    │   ├── Klangstrom_KLST_PANDA_STM32_CubeMX
    │   ├── LICENSE
    │   ├── README.md
    │   ├── USBDevices
    │   └── USBHost
    ├── variants
    │   ├── KLST_CATERPILLAR
    │   └── KLST_PANDA
    └── ...
    ```
- copy all subfolders of `klangstrom-arduino/libraries/` into `ARDUINO_SKETCHBOOK_PATH/libraries/`:
    ```sh
    ARDUINO_SKETCHBOOK_PATH
    ├── hardware
    │   ├── ...
    │   └── umfeld-arduino
    └── libraries
        ├── ...
        ├── Klangstrom
        ├── Klangstrom_Emulator
        ├── Klangstrom_KLST_EMU
        ├── Klangstrom_KLST_PANDA_STM32
        └── Klangstrom_KLST_PANDA_STM32_CubeMX
    ```
- now add board definitions to *STM32duino*
- open `variants` folder:
    ```sh
    klangstrom-arduino/variants
    ├── KLST_CATERPILLAR
    │   ├── KLST_CATERPILLAR-boards.txt
    │   └── variant
    │       ├── PeripheralPins_KLST_CATERPILLAR.c
    │       ├── variant_KLST_CATERPILLAR.cpp
    │       ├── variant_KLST_CATERPILLAR.h
    │       └── variant_KLST_CATERPILLAR.ld
    └── KLST_PANDA
        ├── KLST_PANDA-boards.txt
        └── variant
            ├── PeripheralPins_KLST_PANDA.c
            ├── variant_KLST_PANDA.cpp
            ├── variant_KLST_PANDA.h
            └── variant_KLST_PANDA.ld
    ```
- find `STM32H723ZGT` folder in STM32duino in *Arduino* library folder at: `ARDUINO15_DATA_PATH/packages/STMicroelectronics/hardware/stm32/2.11.0/variants/STM32H7xx/H723Z\(E-G\)T_H730ZBT_H733ZGT/`
- copy all files from `klangstrom-arduino/variants/KLST_PANDA/variant/` + `klangstrom-arduino/variants/KLST_CATERPILLAR/variant/` to `STM32H723ZGT` folder:
- next open `boards.txt` file in `ARDUINO15_DATA_PATH/packages/STMicroelectronics/hardware/stm32/2.11.0/`
- append content of `KLST_CATERPILLAR-boards.txt` and `KLST_PANDA-boards.txt` to `board.txt` file and save
- now either restart *Arduino IDE* and look under `Tools > Board > STM32 MCU based boards` for `Klangstrom KLST_...` boards
- or test with `arduino-cli` in console with `arduino-cli board listall | grep KLST` if boards are present:
    ```sh
    Klangstrom Emulator                        umfeld-arduino:umfeld:KLST_EMU
    Klangstrom KLST_CATERPILLAR (STM32H723ZGT) STMicroelectronics:stm32:KLST_CATERPILLAR
    Klangstrom KLST_PANDA (STM32H723ZGT)       STMicroelectronics:stm32:KLST_PANDA
    ```

<!--
⚠️⚠️⚠️ WIP info is outdated ⚠️⚠️⚠️

begin by installing [Arduino](https://www.arduino.cc/en/software) (v1.8.19+).

the easiest and fastest way to install the lastest *Klangstrom* release is to open the Arduino preferences under `Ardunio > Preferences…` and then add to `Additional Boards Manager URLs: ` the following URLs ( separated by `,` ):

- `https://raw.githubusercontent.com/dennisppaul/klangstrom-arduino/release/package_klangstrom-arduino-stm32_index.json` ( for boards )
- `https://raw.githubusercontent.com/dennisppaul/klangstrom-arduino/release/package_klangstrom-arduino-desktop_index.json` ( for simulator )

the boards can then be installed by navigating to `Tools > Board: … > Boards Manager…` and searching for *Klangstrom*. two version should appear where one is for the *Klangstrom* boards and the other installs the desktop simulator.

## Install Development Versions

alternatively a local development version can be installed by following the steps below:

- install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (v2.11.0+)
- install [STM32duino](https://github.com/stm32duino) via *Arduino Board Manager* ( see [STM32duino / Getting Started](https://github.com/stm32duino/Arduino_Core_STM32#getting-started) for detailed instructions ) (v2.3.0+)
- on *macOS* if *Xcode* is not installed, it might be necessary to install the *clang/clang++* compiler. in order to do so, open *Terminal* application ( located in `Applications` > `Utilities` ) and run `xcode-select --install`  
- download + unpack [current snapshot](https://github.com/dennisppaul/klangstrom-arduino/archive/refs/heads/main.zip) or clone [repository](https://klangstrom-for-arduino.dennisppaul.de). ( note, an unstable development version is available in a [private repository](https://github.com/interaktion-und-raum/klangstrom) by invitation only. )
- rename folder to `klangstrom`
- move `klangstrom` folder to the Arduino’s `hardware` folder ( e.g on macOS `{$HOME}/Documents/Arduino/hardware/` )
- (re)start Arduino

### Default Location of `hardware` Folder on macOS and Windows

the `hardware` is located in the same folder as the `library` folder and the sketches. on macOS and Windows it is organized as follows:

```
    {$HOME}
    └── Documents
        └── Arduino
            ├── hardware
            │   └── klangstrom
            └── libraries
                 └── ...
```

on Linux it is, depending on the installation, e.g organized as follows:

```
    {$HOME}
    └── Arduino
        ├── hardware
        │   └── klangstrom
        └── libraries
             └── ...
```

examples for the `HOME` placeholder can be e.g `/home/example_user` on Linux or `/Users/example_user` on macOS.

### Simulator on Windows + Linux

the simulator currently only works reliably macOS. it is not working on Windows yet. on Linux systems it should work ( tested under *Ubuntu 20.04 LTS* ). in case of failure it sometimes works to install the SDL library via `apt` with `sudo apt install libsdl2-dev`.

### Installing *STM32CubeProgrammer* on *macOS*

occasionally the *STM32CubeProgrammer* installer application fails to start properly. below is a set of instructions that show how to run the installer from the command line. 

- download most recent version of *STM32CubeProgrammer* from https://www.st.com/en/development-tools/stm32cubeprog.html ( e.g version `2.11.0` )
- unzip downloaded file in download folder ( e.g `en.stm32cubeprg-mac_v2-11-0.zip` )
- open command line interface (CLI) ( e.g to use `Terminal.app` go to `Finder`, open *Utilities* folder via menu ( `Go > Utilities` ) or by pressing `CMD+SHIFT+U`, and start `Terminal.app` )
- run installer from CLI ( e.g `~/Downloads/en.stm32cubeprg-mac_v2-11-0/SetupSTM32CubeProgrammer-2.11.0.app/Contents/MacOs/SetupSTM32CubeProgrammer-2_11_0_macos` )
-->