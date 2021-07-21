#ifndef _TINYUSB_H_
#define _TINYUSB_H_

#include "tusb_option.h"
#include "tusb.h"

#ifdef TUSB_OPT_DEVICE_ENABLED
#endif

#ifdef __cplusplus
extern "C" {
#endif



/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#if defined(TINYUSB_BOARD__STM32F072B_DISCO) // BOARD
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void USB_IRQHandler(void) {
    tud_int_handler(0);
}

void board_init(void) {

    // Enable All GPIOs clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // USB Pins
    // Configure USB DM and DP pins. This is optional, and maintained only for user guidance.
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // USB Clock enable
    __HAL_RCC_USB_CLK_ENABLE();
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#elif defined(TINYUSB_BOARD__KLST_TINY)
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

//--------------------------------------------------------------------+
// Forward USB interrupt events to TinyUSB IRQ Handler
//--------------------------------------------------------------------+
void OTG_FS_IRQHandler(void) {
    // @note(always make sure to spec the correct callback. i.e STM32F0 callback is `USB_IRQHandler`)
    tud_int_handler(0);
}

static inline void board_vbus_sense_init(void) {
    /* Deactivate VBUS Sensing B */
    USB_OTG_FS->GCCFG &= ~USB_OTG_GCCFG_VBDEN;

    /* B-peripheral session valid override enable */
    USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOEN;
    USB_OTG_FS->GOTGCTL |= USB_OTG_GOTGCTL_BVALOVAL;
}

void board_init(void) {
    /* Configure USB FS GPIOs */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure USB D+ D- Pins */
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure VBUS Pin */
//     GPIO_InitStruct.Pin = GPIO_PIN_9;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ID Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Enable USB OTG clock
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();

    board_vbus_sense_init();
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#else  // BOARD: NOT DEFINED
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

#warning "TinyUSB: board not supported"
void board_init(void) {}

#endif // BOARD

#ifdef __cplusplus
}
#endif

#endif /* _TINYUSB_H_ */
