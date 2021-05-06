#ifndef Arduino_h
#define Arduino_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifndef WEAK
#define WEAK __attribute__ ((weak))
#endif

#define LOW         0
#define HIGH        1

/* KLST -------------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

#include "KLST-defines.h"
#include "KLST-adapter.h"

// #include "klangstrom_arduino_defines.h"
#include "klangstrom_arduino_proxy.h"
#include "KLST_SDL-adapter.h"

//@todo(move funtion declarations to shared header file `KLST-application.h` )
// #include "KLST-application.h"
void setup();
void loop();
void beat(uint32_t pBeat);
void audioblock(float* pLeftTX, float* pRightTX, float* pLeftRX, float* pRightRX);
void event_receive(const uint8_t event, const float* data);
void data_receive(uint8_t* data, uint8_t length);

#ifdef __cplusplus
}
#endif

#include "klangstrom_arduino_proxy_serial.h"

extern SerialProxy Serial;

/* KLST -------------------------------------------------------------------------------- */

#endif // Arduino_h