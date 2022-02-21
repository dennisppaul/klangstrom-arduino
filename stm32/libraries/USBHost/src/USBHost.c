#include "USBHost.h"

#ifdef USE_USBHOST

#ifdef __cplusplus
extern "C" {
#endif

// @note moved IRQ handler to `bsp_klst_*_usbh_conf.c`

#if USBH_DEBUG_LEVEL > 0U
#define USB_DEBUG_SERIAL 0
#endif
#ifdef USB_DEBUG_SERIAL
#if defined(KLST_BOARD_KLST_TINY)

extern UART_HandleTypeDef *uart_handlers;

int printf(const char *__restrict, ...);
int _write(int file, char *data, int len) {
    HAL_StatusTypeDef status = HAL_UART_Transmit(&uart_handlers[USB_DEBUG_SERIAL], (uint8_t *)data, len, 0xFFFF);
    return (status == HAL_OK ? len : 0);
}

#elif defined(KLST_BOARD__CORE)
// @todo(check why this is not required)
#endif
#endif  // USB_DEBUG_SERIAL

#ifdef __cplusplus
}
#endif

void usb_host_init() {
    MX_USB_HOST_Init();
}

void usb_host_process() {
    MX_USB_HOST_Process();
}

#endif  // USE_USBHOST
