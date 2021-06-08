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
#endif
#include "KlangstromDefinesArduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --- hardware-related functions ---
 * 
 * implemented in `KLST_XXX-adapter.cpp`
 * 
 * functions related to low-level operations implemented in adapter.
 * usually these functions are not used on application side.
 *
 */


void KLST_pre_setup();
void KLST_post_setup();
void KLST_loop();

void KLST_fill_buffer(uint32_t *pTXBuffer, uint32_t *pRXBuffer, uint16_t pBufferLength);
void KLST_start_audio_codec();
void KLST_jump_to_bootloader();
uint32_t KLST_boot_address();
uint32_t KLST_U_ID_address();
bool KLST_audio_input_enabled();
void KLST_shutdown();

#ifdef __cplusplus
}
#endif

/* --- application-related functions ---
 * 
 * implemented in `KLST-adapter.cpp` or `KLST_XXX-adapter.cpp`
 *
 * see also `KlangstromApplicationInterface.hpp`
 *
 */

#ifdef __cplusplus

namespace klangstrom {
    void option(uint8_t pOption, uint32_t pValue);
    void beats_per_minute(float pBPM);
    void beats_per_minute_ms(uint32_t pMicroSeconds);
    void led(uint8_t pLED, bool pState);
    void led_toggle(uint8_t pLED);
    bool button_state(uint8_t pButton);
    bool pin_state(uint8_t pButton);
    void event_transmit(EVENT_TYPE pEvent, float* pPayload);
    void data_transmit(const uint8_t pTransmitter, uint8_t* pData, uint8_t pDataLength);
    void begin_serial_debug(bool pWaitForSerial=false, uint32_t pBaudRate=115200);
    int16_t ID();
};

#endif

#endif /* KlangstromApplicationInterfaceArduino_h */
