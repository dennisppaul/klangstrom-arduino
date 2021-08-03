# KLST / variants

- KLST_CORE (STM32H743II)
- KLST_TINY (STM32F446RE)

## KLST functions

- application-level functions are located in the `klangstrom` namespace
- internal shared (ISH) functions are prefixed with `KLST_ISH_`
- board-specific (BSP) functions are prefixed with `KLST_BSP_`

see `KlangstromApplicationInterfaceArduino.h` for all functions

## variant file structure

each variant folder contains the following 6 files adapted to the respective architecture:

- `KLST_*-BSP.cpp`
- `ldscript.ld` #STM32CubeIDE
- `main.c` #STM32CubeIDE #modified
- `main.h` #STM32CubeIDE
- `PeripheralPins.c`
- `PinNamesVar.h`
- `stm32**xx_hal_msp.c` #STM32CubeIDE
- `stm32**xx_it.c` #STM32CubeIDE #modified
- `stm32**xx_it.h` #STM32CubeIDE
- `variant_KLST_*.cpp`
- `variant_KLST_*.h`

note, that the following *STM32CubeIDE* auto-generated files have been removed:

- `syscalls.c`
- `sysmem.c`
- `stm32**xx_hal_conf.h`
- `system_stm32**xx.c`

make sure to check `stm32**xx_hal_conf.h` if any configurations need to be copied over to `variant_KLST_*.h` ( e.g `HSE_VALUE` or `HAL_XXX_MODULE_ENABLED` )

the following files comprise a KLST variant. some file require modifications:

### `KLST_*-BSP.cpp`

contains implementations and adapters for peripherials.

### `ldscript.ld`

linker script can be copied from `STM32********_FLASH.ld`

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

### `stm32**xx_it.c`

- remove `SysTick_Handler()` function

### `stm32**xx_it.h`

### `variant_KLST_*.cpp`

- check if `SystemClock_Config()` is implemented and remove it if necessary

### `variant_KLST_*.h`

- check definitions in `stm32**xx_hal_conf.h` ( e.g `HSE_VALUE` or enabled modules ( e.g `HAL_SAI_MODULE_ENABLED` for `KLST_TINY` ) ) and if needed copy to `variant_KLST_****.h`.
- define board type ( e.g `#define KLST_BOARD_KLST_TINY` )
- define `LED_BUILTIN`
- define peripherals
- remove default/onboard defines

## required variant defines

`KLST_TINY` defines:

```
/* --------------------------------------------------------------- */
/* KLST                                                            */
/* --------------------------------------------------------------- */
#ifdef __cplusplus
extern "C" {
#endif

/* PIN LABELS */

#define GPIO_00	                  PC13
#define GPIO_01	                  PC14
#define GPIO_02	                  PC15
#define GPIO_03	                  PC2
#define GPIO_04	                  PC3
#define GPIO_05	                  PA2
#define GPIO_06	                  PA3
#define GPIO_07	                  PC5

#define ADC_00	                  PB0
#define ADC_01	                  PB1
#define DAC_00	                  PA4
#define DAC_01	                  PA5

#define LED_00	                  PB5
#define LED_01	                  PD2
#define LED_02	                  PC12

#define I2C_00_SCL	              PB8
#define I2C_00_SDA	              PB7
#define SPI_00_MISO	              PB14
#define SPI_00_MOSI	              PB15
#define SPI_00_SCK	              PB13

#define UART_00_RX	              PA10
#define UART_00_TX	              PB6
#define UART_01_RX	              PC11
#define UART_01_TX	              PC10

#define ENCODER_00_BUTTON         PC4
#define ENCODER_01_BUTTON         PC8
#define ENCODER_02_BUTTON         PB4

/* --- */

#define BUTTON_PROGRAMMER         PC5
#define LED_BUILTIN               LED_00
#define KLST_UART_BAUD            115200

#define ENCODER_00_TIMER          TIM3
#define ENCODER_01_TIMER          TIM8
#define ENCODER_02_TIMER          TIM2
#define KLST_BEAT_TIMER           TIM5

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
  #define KLST_SERIAL_00          Serial1
  #define KLST_SERIAL_01          Serial4
  #define SerialDebug             Serial
  #define KLST_LOG                Serial
#endif

/* serial */

// @todo(maybe add option to def out serial support)
#define ENABLE_HWSERIAL1
#define PIN_SERIAL1_RX            UART_00_RX
#define PIN_SERIAL1_TX            UART_00_TX
#define ENABLE_HWSERIAL4
#define PIN_SERIAL4_RX            UART_01_RX
#define PIN_SERIAL4_TX            UART_01_TX
```
