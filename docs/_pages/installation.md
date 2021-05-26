---
layout: page
title: Installation
permalink: /installation/
---

- install [Arduino](https://www.arduino.cc/en/software) (v1.8.13)
- install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (v2.7.0)
- install [STM32duino](https://github.com/stm32duino) via *Arduino Board Manager* ( see [STM32duino / Getting Started](https://github.com/stm32duino/Arduino_Core_STM32#getting-started) for detailed instructions ) (v2.0.0)
- download + unpack *klangstrom-arduino* release ( or clone repository )
- rename folder to `klangstrom`
- move `klangstrom` folder to the Arduino’s `hardware` folder ( e.g on MacOS `~/Documents/Arduino/hardware/` )
- (re)start Arduino

> this is a block quote. 

### Default Location of `hardware` Folder on MacOS

    Documents
    └── Arduino
        ├── hardware
        │   └── klangstrom
        └── libraries
             └── ...
