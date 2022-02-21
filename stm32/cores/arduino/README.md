# KLST / cores / modifications

- added `#define HAL_SAI_MODULE_ENABLED`

## `Arduino.h`

- added `#include "KlangstromDefinesArduino.h"`
- added `#include "KlangstromApplicationInterfaceArduino.h"`
- added `extern void KLST_pre_setup() __attribute__((weak));`
- added `extern void KLST_post_setup() __attribute__((weak));`

## `main.cpp`

- added `KLST_pre_setup()` to `main()`
- added `KLST_post_setup()` to `main()`
- added `KLST_ISH_loop()` to `main()`
