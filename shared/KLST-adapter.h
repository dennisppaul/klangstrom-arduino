//
//  KLST-adapter.h
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#ifndef KLST_adapter_h
#define KLST_adapter_h

#ifdef __cplusplus
namespace klst{}; //@todo(decide if `namespace` is a good option)
void klst_option(uint8_t pOption, uint8_t pValue);
void klst_led(uint8_t pLED, bool pState);
void klst_led_toggle(uint8_t pLED);
bool klst_button_state(uint8_t pButton);
void klst_set_BPM(float pBPM);
void klst_set_BPM_ms(uint32_t pMicroSeconds);
#endif

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

#endif /* KLST_adapter_h */
