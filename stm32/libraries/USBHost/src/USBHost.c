#ifdef USE_USBHOST

#include "USBHost.h"

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

/* --- callback functions --- */

// TODO add midi callbacks
static CallbackKeyPressed    pCallbackKeyPressed    = NULL;
static CallbackKeyReleased   pCallbackKeyReleased   = NULL;
static CallbackMousePressed  pCallbackMousePressed  = NULL;
static CallbackMouseReleased pCallbackMouseReleased = NULL;
static CallbackMouseMoved    pCallbackMouseMoved    = NULL;
static CallbackEventReceive  pCallbackEventReceive  = NULL;

void usb_host_callback_key_pressed(CallbackKeyPressed callback) {
    pCallbackKeyPressed = callback;
}

void usb_host_call_key_pressed(const uint8_t key) {
    if (pCallbackKeyPressed) {
        pCallbackKeyPressed(key);
    }
}

void usb_host_callback_key_released(CallbackKeyReleased callback) {
    pCallbackKeyReleased = callback;
}

void usb_host_call_key_released(const uint8_t key) {
    if (pCallbackKeyReleased) {
        pCallbackKeyReleased(key);
    }
}

void usb_host_callback_mouse_pressed(CallbackMousePressed callback) {
    pCallbackMousePressed = callback;
}

void usb_host_call_mouse_pressed(const EVENT_DATA button, const int8_t x, const int8_t y) {
    if (pCallbackMousePressed) {
        pCallbackMousePressed(button, x, y);
    }
}

void usb_host_callback_mouse_released(CallbackMouseReleased callback) {
    pCallbackMouseReleased = callback;
}

void usb_host_call_mouse_released(const EVENT_DATA button, const int8_t x, const int8_t y) {
    if (pCallbackMouseReleased) {
        pCallbackMouseReleased(button, x, y);
    }
}

void usb_host_callback_mouse_moved(CallbackMouseMoved callback) {
    pCallbackMouseMoved = callback;
}

void usb_host_call_mouse_moved(const bool *buttons, const int8_t x, const int8_t y) {
    if (pCallbackMouseMoved) {
        pCallbackMouseMoved(buttons, x, y);
    }
}

void usb_host_callback_event_receive(CallbackEventReceive callback) {
    pCallbackEventReceive = callback;
}

void usb_host_call_event_receive(const EVENT_TYPE event, const void *data) {
    if (pCallbackEventReceive) {
        pCallbackEventReceive(event, data);
    }
}

#endif  // USE_USBHOST
