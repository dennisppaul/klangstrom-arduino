# KLST / variants

- KLST_CORE (STM32H743ZI/II)
- KLST_TINY (STM32F446RE)

## structure

each variant folder contains the following 6 files adapted to the respective architecture:

- `KLST_*-adapter.cpp`
- `KLST_*-main.cpp`
- ~~`hal_conf_extra.h`~~
- `variant_KLST.h` 
- `variant_KLST.cpp`
- `PinNamesVar.h`
- `PeripheralPins.c` 
- `ldscript.ld` :: linker script ( can be copied from `STM32F446RETX_FLASH.ld` )

additionally the following files need to be copied from:

- `main.h` :: might not be necessary
- `stm32f4xx_hal_msp.c` :: might need to add `void Error_Handler() {}`
- `stm32f4xx_it.c` :: comment out `SysTick_Handler()`
- `stm32f4xx_it.h`
- `syscalls.c`
- `sysmem.c`
- @todo(find out if `startup_stm32f446retx.s` is also updated)

the following modification should be considered:

- `SystemClock_Config()` must be implemented in `klangstrom_arduino_board_peripherals.cpp` ( function is called from `hw_config_init()` in `$BOARD_DIR/libraries/SrcWrapper/src/stm32/hw_config.c` )

### `stm32f4xx_hal_msp.c`

- comment out `HAL_TIM_Base_MspInit` + `HAL_TIM_Base_MspDeInit`
- comment out `HAL_ADC_MspInit` + `HAL_ADC_MspDeInit` @todo(check!)
- comment out `HAL_DAC_MspInit` + `HAL_DAC_MspDeInit` @todo(check!)

### `KLST_*-main.cpp`

contains auto-generated functions from `main.c` from STM32CubeIDE.

- remove `static` from `MX_*_Init` functions
- remove main function
- keep `SystemClock_Config()` but check if it is implemented elsewhere

note that `SystemClock_Config()` is called from `hw_config_init()` ( in `$BOARD_DIR/libraries/SrcWrapper/src/stm32/hw_config.c` ), right after a call to `HAL_Init()`. 

also note that USB peripherals are initialzed for CDC right after this call with `USBD_CDC_init();` if selected.

### `KLST_*-adapter.cpp`

contains implementations and adpaters to peripherials.

- declare `MX_*_Init` functions
- `KLST_pre_setup()` :: called before `setup()` used to initialize hardware peripherals ( audio codec, serial, SPI via `MX_*_Init` functions ), copied from `main.c` from STM32CubeIDE @todo(check if it s better to not initialize all peripherals here or at least use `#define` to en-/disable them)
- `KLST_post_setup()` :: called after `setup()` starts audio processing

### `variant_KLST.h` 

- define `KLST_BOARD_TYPE` ( e.g `#define KLST_BOARD_TYPE=KLST_TINY` )
- enable SAI `#define HAL_SAI_MODULE_ENABLED` #KLST_TINY

### `variant_KLST.cpp`

- check if `SystemClock_Config()` is implemented and remove it if necessary

## TODO

- @todo add `KLST_CubeIDE.*` which contains all the auto-generated init files
- @todo( this folder should only contain the following files:
- @todo(@KLST_CORE, check if `ARDUINO_KLANGSTROM` is defined because it is used in `PeripheralPins.c` and `variant.*` )