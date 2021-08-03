#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_klst_inc.h"

#ifndef USB_VENDOR_ID
  #define USB_VENDOR_ID 0xCAFE
  // #define USB_VENDOR_ID 0x0483 // STMicroelectronics
#endif

/* -------------------------------------------------------------------------- */
/* --- KLST BOARD ----------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

#if defined(KLST_BOARD_KLST_TINY)
  #define CFG_TUSB_MCU              OPT_MCU_STM32F4
#elif defined(KLST_BOARD_KLST_CORE )
  #define OTG_FS_VBUS_SENSE     0
  #define OTG_HS_VBUS_SENSE     0
  #define  BOARD_DEVICE_RHPORT_NUM 0
  #define CFG_TUSB_MCU              OPT_MCU_STM32H7
#else
  #warning "@TINYUSB :: MCU not supported"
#endif

/* -------------------------------------------------------------------------- */
/* --- USB CLASS ------------------------------------------------------------ */
/* -------------------------------------------------------------------------- */

#if defined(TINYUSB_MIDI_DEVICE)
  #include "tusb_config-midi_device.h"
#elif defined(TINYUSB_MOUSE_KEYBOARD_DEVICE)
  #include "tusb_config-mouse_keyboard_device.h"
#else
  #ifdef TINYUSB_CUSTOM_CONFIG_FILE
    #include TINYUSB_CUSTOM_CONFIG_FILE
  #else
    #if __has_include("tusb_config-custom.h")
      #include "tusb_config-custom.h"
    #else
      #warning @TINYUSB could not find any config file. try defining `TINYUSB_CUSTOM_CONFIG_FILE`.
    #endif
  #endif
  #define TINYUSB_CUSTOM
#endif

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
