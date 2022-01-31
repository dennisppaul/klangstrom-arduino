//
// KlangstromApplicationInterfaceArduino.h
// Klangstrom
//
//
//

#ifndef KlangstromApplicationInterfaceArduino_h
#define KlangstromApplicationInterfaceArduino_h

#ifdef __cplusplus
#include "KlangstromDefines.hpp"
#include "KlangstromEvents.h"
#endif
#include "KlangstromDefinesArduino.h"

/* --------------------------------------------------------------------------------------------- */
/* --- internal shared (ISH) functions --------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

void KLST_ISH_jump_to_bootloader();
void KLST_ISH_loop();
void KLST_ISH_post_setup();
void KLST_ISH_pre_setup();
void KLST_ISH_fill_buffer(uint32_t* pTXBuffer, uint32_t* pRXBuffer, uint16_t pBufferLength);
void KLST_ISH_shutdown();
void KLST_ISH_handle_encoders();
void KLST_ISH_handleSerialPorts();

uint32_t KLST_ISH_OPT_audio_line();
bool     KLST_ISH_OPT_audio_input_enabled();
float    KLST_ISH_OPT_headphone_output_volume();

#ifdef __cplusplus
}
#endif

/* --------------------------------------------------------------------------------------------- */
/* --- board-specific (BSP) functions ---------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

uint32_t KLST_BSP_boot_address();
uint32_t KLST_BSP_U_ID_address();
void     KLST_BSP_configure_audio_codec();
void     KLST_BSP_init_encoders();
void     KLST_BSP_deinit_encoders();
void     KLST_BSP_start_audio_codec();
void     KLST_BSP_shutdown();
void     KLST_BSP_init_peripherals();
void     KLST_BSP_init_LEDs();
void     KLST_BSP_configure_TinyUSB();
uint8_t  KLST_BSP_error_code();

#ifdef __cplusplus
}
#endif

/* --------------------------------------------------------------------------------------------- */
/* --- application-level functions ------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

#ifdef __cplusplus

namespace klangstrom {
    void    beats_per_minute(float pBPM);
    void    beats_per_minute_ms(uint32_t pMicroSeconds);
    void    begin_serial_debug(bool pWaitForSerial = false, uint32_t pBaudRate = 115200);
    bool    button_state(uint8_t pButton);
    void    data_transmit(const uint8_t pTransmitter, uint8_t* pData, uint8_t pDataLength);
    void    event_transmit(EVENT_TYPE pEvent, float* pPayload);
    int16_t ID();
    char*   U_ID();
    void    LED(uint16_t pLED, uint8_t pState);
    void    led(uint16_t pLED, bool pState);
    void    led_toggle(uint16_t pLED);
    void    option(uint8_t pOption, float pValue);
    bool    pin_state(uint8_t pButton);
};  // namespace klangstrom

#endif

#endif /* KlangstromApplicationInterfaceArduino_h */
