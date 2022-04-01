---
layout: page
title: Installation
permalink: /installation/
---

- install [Arduino](https://www.arduino.cc/en/software) (v1.8.19)
- install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (v2.10.0)
- install [STM32duino](https://github.com/stm32duino) via *Arduino Board Manager* ( see [STM32duino / Getting Started](https://github.com/stm32duino/Arduino_Core_STM32#getting-started) for detailed instructions ) (v2.2.0)
- on *macOS* if *Xcode* is not installed, it might be necessary to install the *clang/clang++* compiler. in order to do so, open *Terminal* application ( located in `Applications` > `Utilities` ) and run `xcode-select --install`  
- download + unpack klangstrom-arduino [release](https://github.com/dennisppaul/klangstrom-arduino/releases) ( or clone [repository](https://klangstrom-for-arduino.dennisppaul.de) )
- rename folder to `klangstrom`
- move `klangstrom` folder to the Arduino’s `hardware` folder ( e.g on MacOS `~/Documents/Arduino/hardware/` )
- (re)start Arduino

### Default Location of `hardware` Folder on MacOS and Windows

    Documents
    └── Arduino
        ├── hardware
        │   └── klangstrom
        └── libraries
             └── ...
