# KLST / modifications

## `libraries/SrcWrapper/src/stm32/`

### `uart.c`

- added callbacks to `KLST_RxCpltCallback` in `HAL_UART_RxCpltCallback`
- removed timer de/init functions `HAL_TIM_Base_MspInit`+ `HAL_TIM_Base_MspDeInit`