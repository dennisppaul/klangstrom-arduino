#ifndef Arduino_h
#define Arduino_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "KLST_Includes.h"
#include "KLST_SDL-adapter.h"

#ifndef WEAK
#define WEAK __attribute__ ((weak))
#endif

#define LOW         0
#define HIGH        1

/* sketch */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
// Weak empty variant initialization function.
// May be redefined by variant files.
extern void initVariant() __attribute__((weak));

extern void setup(void) ;
extern void loop(void) ;

void yield(void);
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#include "klangstrom_arduino_proxy_serial.h"

extern SerialProxy Serial;

/* KLST -------------------------------------------------------------------------------- */

#endif // Arduino_h