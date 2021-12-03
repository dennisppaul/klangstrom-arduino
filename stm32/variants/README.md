# KLST / variants

- KLST_SHEEP (STM32H743VI)
- KLST_CORE (STM32H743II)
- KLST_TINY (STM32F446RE)

## KLST functions

- application-level functions are located in the `klangstrom` namespace
- internal shared (ISH) functions are prefixed with `KLST_ISH_`
- board-specific (BSP) functions are prefixed with `KLST_BSP_`

see `KlangstromApplicationInterfaceArduino.h` for all functions

## variant file structure

each variant folder contains the following 6 files adapted to the respective architecture ( *STM32CubeIDE* auto-generated files from #STM32CubeIDE or copied files from #STM32duino ):

- `KLST_*-BSP.cpp`
- `ldscript.ld` #STM32CubeIDE
- `main.c` #STM32CubeIDE #modified
- `main.h` #STM32CubeIDE
- `PeripheralPins.c` #STM32duino
- `PinNamesVar.h` #STM32duino
- `stm32**xx_hal_msp.c` #STM32CubeIDE
- `stm32**xx_it.c` #STM32CubeIDE #modified
- `stm32**xx_it.h` #STM32CubeIDE
- `variant_KLST_*.cpp` #STM32duino
- `variant_KLST_*.h` #STM32duino

note, that the following have been removed:

- `syscalls.c`
- `sysmem.c`
- `stm32**xx_hal_conf.h`
- `system_stm32**xx.c`

make sure to check `stm32**xx_hal_conf.h` if any configurations need to be copied over to `variant_KLST_*.h` ( e.g `HSE_VALUE` or `HAL_XXX_MODULE_ENABLED` )

the following files comprise a KLST variant. some file require modifications:

### `KLST_*-BSP.cpp`

contains implementations and adapters for peripherials.

### `ldscript.ld`

linker script is copied from file `*_FLASH.ld`.

### `main.c`

- remove `main()` function
- remove `Error_Handler()` function
- add call to `PeriphCommonClock_Config()` at the end of `SystemClock_Config()`
- remove `static` from `MX_*_Init` functions
- note, `SystemClock_Config()` is called from `hw_config_init()` ( in `$BOARD_DIR/libraries/SrcWrapper/src/stm32/hw_config.c` ), right after a call to `HAL_Init()`.
- note, USB peripherals are initialzed for CDC right after this call with `USBD_CDC_init();` if selected.

for `KLST_TINY` if `USB_OTG_FS` is not selected the clock initialization does not happen. to manually intialize the USB clock the following lines must be added to `PeriphCommonClock_Config()`:

- find `PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;` and add `RCC_PERIPHCLK_CLK48` to get `PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1|RCC_PERIPHCLK_CLK48;`
- add `PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLSAIP;` in the same block

### `main.h`

### `PeripheralPins.c`

### `PinNamesVar.h`

### `stm32**xx_hal_msp.c`

- remove `HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)` + `HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)`
- ==@todo(check if this is necessary)== remove `HAL_HCD_MspInit(HCD_HandleTypeDef* hhcd)` + `HAL_HCD_MspDeInit(HCD_HandleTypeDef* hhcd)`

### `stm32**xx_it.c`

- remove `SysTick_Handler()` function

### `stm32**xx_it.h`

### `variant_KLST_*.cpp`

- check if `SystemClock_Config()` is implemented and remove it if necessary

### `variant_KLST_*.h`

- specifiy `PIN MAP`, `HAL` + `PERIPHERALS` sections
- check definitions in `stm32**xx_hal_conf.h` ( e.g `HSE_VALUE` or enabled modules ( e.g `HAL_SAI_MODULE_ENABLED` for `KLST_TINY` ) ) and if needed copy to `variant_KLST_****.h`.
- define board type ( e.g `#define KLST_BOARD_KLST_TINY` )
- define peripherals:
    - `LED_BUILTIN`
    - `USER_BTN`
    - `PIN_SPI_MOSI` +  `PIN_SPI_MISO` +  `PIN_SPI_SCK` +  `PIN_SPI_SS` ( `CS` ) 
    - `PIN_WIRE_SDA` + `PIN_WIRE_SCL` ( `I2C` )
    - `TIMER_TONE`
    - `TIMER_SERVO`
    - `PIN_SERIAL_RX` + `PIN_SERIAL_TX`
- define `KLST_UART_BAUD` + `KLST_SERIAL_**`
- remove default/onboard defines
