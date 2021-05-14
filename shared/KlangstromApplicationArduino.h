//
// KlangstromApplicationArduino.h
// Klangstrom
//
//
//

#ifndef KlangstromApplicationArduino_h
#define KlangstromApplicationArduino_h

/* --- application-related callback functions --- 
 *
 * implemented in applicaton
 *
 * see also `KlangstromApplication.hpp`
 *
 */

#ifdef __cplusplus
#include "KlangstromDefines.hpp"
#include "KlangstromDefinesArduino.h"

WEAK void setup() {}
WEAK void loop() {}
WEAK void beat(uint32_t pBeat) {}
WEAK void event_receive(const EVENT_TYPE event, const float* data) {} // @todo(as cool as it is to use *raw* arrays, consider using `std::array<float, ?>`)
// WEAK void data_receive(uint8_t* data, uint8_t length) {}
WEAK void data_receive(const uint8_t sender, uint8_t* data, uint8_t length) {}

WEAK void audioblock(SIGNAL_TYPE* pLeftTX, SIGNAL_TYPE* pRightTX, SIGNAL_TYPE* pLeftRX, SIGNAL_TYPE* pRightRX) {
    for (uint16_t i=0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        pLeftTX[i] =  0.0; pLeftRX[i] = 0.0;
        pRightTX[i] =  0.0; pRightRX[i] = 0.0;
    }
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* KlangstromApplicationArduino_h */
