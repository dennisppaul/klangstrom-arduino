#ifndef _KLANGSTROM_ARDUINO_
#define _KLANGSTROM_ARDUINO_

#include "Klang.hpp"
#include "klangstrom_arduino_defines.h"
#include "klangstrom_peripherals.h"

#ifdef __cplusplus
extern "C" {
#endif

void KLST_JumpToBootloader();
void setup();
void loop();

#ifdef __cplusplus
}
#endif

void beat(uint32_t pBeat);
void audioblock(SIGNAL_TYPE* pLeftTX, SIGNAL_TYPE* pRightTX, SIGNAL_TYPE* pLeftRX, SIGNAL_TYPE* pRightRX);
void event_receive(const EVENT_TYPE event, const float* data);
void data_receive(uint8_t* data, uint8_t length);

#endif /* _KLANGSTROM_ARDUINO_ */
