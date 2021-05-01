# KLST / shared sources

this folder contains source files shared by all `Klangstrom (MCU)` variants ( files shared by all architectures ( e.g `MCU`, `CPU` and `VCV` ) are located in `../../../../shared` ):

- `AudioCodecWM8731.*`
- `KLST-adapter.cpp`

## notes

- `$BOARD_DIR/libraries/SrcWrapper/src/stm32/hw_config.c` contains function `hw_config_init()` which calls `HAL_Init()` and then `SystemClock_Config()`. the latter is overriden in the respective `variant.cpp`
