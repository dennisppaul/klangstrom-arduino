# Klangstrom for Arduino

- install [Arduino IDE](https://www.arduino.cc/en/software/#ide)
- in Arduino IDE at `Tools > Board:... > Board Manager...` ( `CMD+SHIFT+B` ) install `STM32 MCU based board`
- install klangstrom with installer script `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/dennisppaul/klangstrom-arduino/refs/heads/main/install.sh)"` ( <- WIP needs testing )

## Install Emulator (WIP)

- install [Umfeld](https://github.com/dennisppaul/umfeld) with installer script `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/dennisppaul/umfeld/refs/heads/main/install.sh)"`

## Structure

```sh
klangstrom-arduino
├── libraries # `klangstrom-libraries` copied to arduino libraries
│   ├── Klangstrom
│   ├── Klangstrom_KLST_EMU
│   ├── Klangstrom_KLST_PANDA_STM32
│   ├── Klangstrom_KLST_PANDA_STM32_CubeMX
│   ├── USBDevices
│   ├── USBHost # <- WIP not working ATM, do not copy
│   └── klangwellen
├── install.sh # installs libs and variants ( added only to the public repo )
├── tools
│   ├── append-board-variant-definition.sh
│   ├── klst-serialmonitor
│   ├── klst-sketch
│   ├── link_libraries.sh
│   ├── link_variant_files.sh
│   ├── klst-update-variant.sh
│   └── stm32duino.config
└── boards # variants files added to STM32duino via script 
    ├── klangstrom-stm32-standalone # WIP whole repo or copy just `variants` + `tools` 
    └── klangstrom-emulator-standalone # WIP whole repo or copy just `emulator` to `hardware`
```

---

( or on macOS via Homebrew `brew install arduino-ide` )