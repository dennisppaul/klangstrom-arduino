//
//  KlangConfiguration.h
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef KlangConfiguration_h
#define KlangConfiguration_h

/* --- DEBUG --- */

#define DEBUG_AUDIO_POOL
#define DEBUG_CONNECTION_POOL
#define DEBUG_SYNTHESIZER
#define DEBUG_DAC
//#define DEBUG_PRINT_EVENTS
#define DEBUG_OSC
#define KLANG_DEBUG_LEVEL     2

/* --- AUDIO SETUP --- */

#include "KlangDefines.hpp"
#define KLANG_AUDIO_RATE                  44100
#define KLANG_AUDIO_BLOCKS                16
#define KLANG_SAMPLES_PER_AUDIO_BLOCK     1024 // @NOTE("@macos there seems to be a synchronization issue with audio input and output with block sizes smaller than 1024")
#define KLANG_SIGNAL_TYPE                 SIGNAL_TYPE_FLOAT

#define KLANG_OSC_TRANSMIT_ADDRESS        "224.0.0.1"
#define KLANG_OSC_TRANSMIT_PORT           7000
#define KLANG_OSC_RECEIVE_PORT            7001

#endif /* KlangConfiguration_h */
