#ifndef _KLANGSTROM_PERIPHERALS_
#define _KLANGSTROM_PERIPHERALS_

#include <stdint.h>
#include "Klang.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#define I2S_USE_DMA         1
#define I2S_BUFFER_SIZE     (KLANG_SAMPLES_PER_AUDIO_BLOCK*2) // 256 // @todo(define buffer size in klangstrom config file)
#define AUDIO_SAMPLE_RATE   KLANG_AUDIO_RATE // 46876 // ~44100 Hz

void KLST_init();
void KLST_I2S_transmit(uint16_t pLeft, uint16_t pRight);
void KLST_I2S_receive(uint16_t* pLeft, uint16_t* pRight);
void KLST_audio_interrupt();

#ifdef __cplusplus
}
#endif

void KLST_fillBuffer(uint32_t *pTXBuffer, uint32_t *pRXBuffer, uint16_t pBufferLength);

#endif /* _KLANGSTROM_PERIPHERALS_ */
