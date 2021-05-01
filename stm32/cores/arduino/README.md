# KLST / cores / modifications

- added `#define HAL_SAI_MODULE_ENABLED`

## `Arduino.h`

-  added import `#include "klangstrom_arduino_defines.h"`
-  added import `#include "klangstrom_arduino.h"`
-  added `extern void KLST_pre_setup() __attribute__((weak));`
-  added `extern void KLST_post_setup() __attribute__((weak));`

## `main.cpp`

- added import `#include "klangstrom_arduino.h"`
- added `KLST_pre_setup()` + `KLST_post_setup()` to `main()`
