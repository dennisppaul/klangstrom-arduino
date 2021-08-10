//
//  Klang.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef Klang_hpp
#define Klang_hpp

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* --- USER DEFINES --- */

#if __has_include("KlangConfiguration.h")
#include "KlangConfiguration.h"
#endif

/* --- DEBUG --- */

//#define DEBUG_SYNTHESIZER
//#define DEBUG_AUDIO_POOL
//#define DEBUG_CONNECTION_POOL
//#define DEBUG_SYNTHESIZER
//#define DEBUG_DAC
//#define DEBUG_OSC
//#define DEBUG_PRINT_EVENTS
//#define DEBUG_SHOW_DEFAULT_WARNINGS

/* --- DEFAULTS --- */

#ifndef KLANG_DEBUG_LEVEL
#define KLANG_DEBUG_LEVEL                   0
#endif

#ifndef KLANG_DEFINES
#include "KlangDefines.hpp"
#endif

#ifndef KLANG_AUDIO_RATE
#define KLANG_AUDIO_RATE                    48000
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_RATE to default value: 48000
#endif
#endif

#ifndef KLANG_AUDIO_BLOCKS
#define KLANG_AUDIO_BLOCKS                  32
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_BLOCKS to default value: 32
#endif
#endif

#ifndef KLANG_SAMPLES_PER_AUDIO_BLOCK
#define KLANG_SAMPLES_PER_AUDIO_BLOCK       512
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_SAMPLES_PER_AUDIO_BLOCK to default value: 512
#endif
#endif

#if !defined(SIGNAL_TYPE_FLOAT) && !defined(SIGNAL_TYPE_INT16)
#warning SIGNAL_TYPE is not defined properly. choosing `SIGNAL_TYPE_FLOAT`. check if `KlangDefines.hpp` is included in `KlangConfiguration.h`?
#endif

#ifndef KLANG_SIGNAL_TYPE
#define KLANG_SIGNAL_TYPE                   SIGNAL_TYPE_FLOAT
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_SIGNAL_TYPE to default value: SIGNAL_TYPE_FLOAT
#endif
#endif

/* --- SIGNAL --- */

#if (KLANG_SIGNAL_TYPE == SIGNAL_TYPE_INT16)
typedef int16_t          SIGNAL_TYPE;
const static SIGNAL_TYPE SIGNAL_MIN = -32768;
const static SIGNAL_TYPE SIGNAL_MAX = 32767;
#elif (KLANG_SIGNAL_TYPE == SIGNAL_TYPE_FLOAT)
typedef float            SIGNAL_TYPE;
const static SIGNAL_TYPE SIGNAL_MIN = -1.0;
const static SIGNAL_TYPE SIGNAL_MAX = 1.0;
#else
#error KLANG_SIGNAL_TYPE not defined
#endif

#define KLANG_FILL_AUDIO_BUFFER(DST, VAL) memset(DST, VAL, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE))
#define KLANG_COPY_AUDIO_BUFFER(DST, SRC) memcpy(DST, SRC, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE))

/* --- CONSTANTS --- */

#define DEFAULT_FREQUENCY (KLANG_AUDIO_RATE / 256.0 * 0.5)
static const CONNECTION_ID KLANG_CONNECTION_ERROR  = -1;
static const CONNECTION_ID KLANG_CREATE_NODE_ERROR = -1;
static const uint16_t      KLANG_AUDIO_RATE_UINT16 = KLANG_AUDIO_RATE;

namespace klang {
    class Node;
    class Connection;

    class Klang {
    public:
        static Klang &instance() {
            static Klang instance;
            return instance;
        }

        static CONNECTION_ID connect(Node &pOutputNode, Node &pInputNode);
        static CONNECTION_ID connect(Node &pOutputNode, CHANNEL_ID pOutputChannel, Node &pInputNode, CHANNEL_ID pInputChannel);
        static bool          disconnect(Node &pOutputNode, CHANNEL_ID pOutputChannel, Node &pInputNode, CHANNEL_ID pInputChannel);
        static bool          disconnect(CONNECTION_ID pConnectionID);

        FRAME_TYPE frame_index();
        void       reset_frame_index();

        static void lock() { instance().mLock = true; }
        static void unlock() { instance().mLock = false; }
        static bool islocked() { return instance().mLock; }

        bool frame_begin();
        void frame_end();

    private:
        Klang(Klang const &);
        void operator=(Klang const &);
        Klang() {}

        FRAME_TYPE    mFrameCounter = 0;
        volatile bool mLock         = false;
#ifdef DEBUG_SYNTHESIZER
        bool mFirstFrame = true;
#endif
    };
}  // namespace klang

#endif /* Klang_hpp */
