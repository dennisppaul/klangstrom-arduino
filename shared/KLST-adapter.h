//
//  KLST-adapter.h
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#ifndef KLST_adapter_h
#define KLST_adapter_h

/* 
* functions related to low-level operations implemented in adapter.
* usually these functions are not used on application side.
*/

#ifdef __cplusplus
extern "C" {
#endif

void KLST_pre_setup();
void KLST_post_setup();
void KLST_loop();

void KLST_fill_buffer(uint32_t *pTXBuffer, uint32_t *pRXBuffer, uint16_t pBufferLength);
void KLST_start_audio_codec();
void KLST_jump_to_bootloader();
uint32_t KLST_boot_address();

#ifdef __cplusplus
}
#endif

/*
* functions 
*/

#ifdef __cplusplus

namespace klst {
    void option(uint8_t pOption, uint8_t pValue);
    void beats_per_minute(float pBPM);
    void beats_per_minute_ms(uint32_t pMicroSeconds);
    void led(uint8_t pLED, bool pState);
    void led_toggle(uint8_t pLED);
    bool button_state(uint8_t pButton);
    bool pin_state(uint8_t pButton);
    void event_transmit(const uint8_t pEvent, float* pPayload);
    void data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength);
};

#endif

#endif /* KLST_adapter_h */
