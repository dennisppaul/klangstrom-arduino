<img src="./assets/KLST--app-icon.png" alt="KLST--app-icon" width="256"/>

*Klangstrom* (KLST) is an infrastructure to facilitate generative, networked, embedded sound + music + composition making. *klangstrom* is comprised of two software libraries ( *klang* a node+text-based synthesizer library, *strom* a node+text-based generative composition library ), an embedded hardware platform, and a programming environment to allow seamless development of generative, networked, embedded sound + music + composition applications.

# Klangstrom Arduino Library

note, that this repository is only used for arduino hardware library ( board ) releases. the development of the klangstrom project happens ( incl other platform and hardware ) in a dedicated repository at [klangstrom](https://github.com/interaktion-und-raum/klangstrom/) ( currently a private repository ). the development process can also be followed in the [developer diary](https://klangstrom.dennisppaul.de).

## installation

- install [Arduino](https://www.arduino.cc/en/software) (v1.8.13)
- install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) (v2.7.0)
- install [STM32duino](https://github.com/stm32duino) via *Arduino Board Manager* ( see [Getting Started](https://github.com/stm32duino/Arduino_Core_STM32#getting-started) for detailed instructions ) (v1.9)
- download + unpack *klangstrom-arduino* release ( or clone repository )
- if necessary rename folder to `klangstrom`
- move `klangstrom` folder to the arduino `hardware` folder ( e.g on MacOS `~/Documents/Arduino/hardware/` )

### default location of hardware folder on MacOS

    Documents
    └── Arduino
        ├── hardware
        │   └── klangstrom
        └── libraries
             └── ...
