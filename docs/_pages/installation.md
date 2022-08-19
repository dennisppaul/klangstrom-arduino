---
layout: page
title: Installation
permalink: /installation/
---

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
