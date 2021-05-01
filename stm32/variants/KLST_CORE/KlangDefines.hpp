//
//  KlangDefines.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef KlangDefines_hpp
#define KlangDefines_hpp

/* --- TARGETS ARCHITEXTURES --- */

#if defined(TARGET_STM32F4) || defined(STM32H743xx)
#define KLANG_TARGET_MCU
#else
#define KLANG_TARGET_CPU
#endif

/* --- DEBUG --- */

#ifndef KLANG_DEBUG_LEVEL
#define KLANG_DEBUG_LEVEL 2
#endif

#if defined(KLANG_TARGET_MCU)
#define KLANG_NL "\n\r"
#else
#define KLANG_NL "\n"
#endif

#if (KLANG_DEBUG_LEVEL == 0)
#define  KLANG_LOG_ERR(...)
#define  KLANG_LOG(...)
#define  KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL == 1)
#define  KLANG_LOG_ERR(...)       printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG(...)
#define  KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL == 2)
#define  KLANG_LOG_ERR(...)   printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG(...)       printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG_SIM(...)
#elif (KLANG_DEBUG_LEVEL >= 3)
#define  KLANG_LOG_ERR(...)   printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG(...)       printf(__VA_ARGS__);\
printf(KLANG_NL);
#define  KLANG_LOG_SIM(...)   printf(__VA_ARGS__);\
printf(KLANG_NL);
#endif

/* --- CONSTANTS --- */

#define TRUE                    1
#define FALSE                   0
#define OSC_DEFAULT_FREQUENCY   220.0
#define OSC_DEFAULT_AMPLITUDE   1.0
#define SIGNAL_TYPE_FLOAT       1
#define SIGNAL_TYPE_INT16       2

/* --- TOOLS --- */

#define PPQN                24
#define FULL_NOTE(x)        ((x) % (PPQN * 4) == 0)
#define HALF_NOTE(x)        ((x) % (PPQN * 2) == 0)
#define QUARTER_NOTE(x)     ((x) % (PPQN    ) == 0)
#define EIGTH_NOTE(x)       ((x) % (PPQN / 2) == 0)
#define SIXTEENTH_NOTE(x)   ((x) % (PPQN / 4) == 0)
#define THIRTYTWO_NOTE(x)   ((x) % (PPQN / 8) == 0)

/* --- TYPEDEFS --- */

typedef uint8_t KLANG_CMD_TYPE;
typedef uint32_t FRAME_TYPE;
typedef uint8_t EVENT_TYPE;
typedef uint8_t EVENT_DATA;
typedef uint8_t CHANNEL_ID;
typedef int8_t AUDIO_BLOCK_ID;
typedef int16_t NODE_ID;
typedef int16_t CONNECTION_ID;

namespace klang {

    /* --- EVENT_TYPE (HID) --- */

    static const EVENT_TYPE EVENT_RESERVED_01 = 0x00;
    static const EVENT_TYPE EVENT_RESERVED_02 = 0x01;
    static const EVENT_TYPE EVENT_RAW_DATA = 0x02;
    static const EVENT_TYPE EVENT_MOUSE_PRESSED = 0x03;
    static const EVENT_TYPE EVENT_MOUSE_RELEASED = 0x04;
    static const EVENT_TYPE EVENT_MOUSE_MOVED = 0x05;
    static const EVENT_TYPE EVENT_MOUSE_DRAGGED = 0x06;
    static const EVENT_TYPE EVENT_KEY_PRESSED = 0x07;
    static const EVENT_TYPE EVENT_KEY_RELEASED = 0x08;

/* --- EVENT_DATA LOCATIONS (HID) --- */

    static const EVENT_DATA X = 0x00;
    static const EVENT_DATA Y = 0x01;
    static const EVENT_DATA BUTTON = 0x02;
    static const EVENT_DATA LEFT = 0x00;
    static const EVENT_DATA MIDDLE = 0x01;
    static const EVENT_DATA RIGHT = 0x02;
    static const EVENT_DATA KEY = 0x00;

/* --- EVENT_TYPE (MIDI) --- */

// @REF([Summary of MIDI Messages](https://www.midi.org/specifications/item/table-1-summary-of-midi-message))

    static const EVENT_TYPE EVENT_MIDI_IN_NOTE_ON = 0x09;
    static const EVENT_TYPE EVENT_MIDI_IN_NOTE_OFF = 0x0A;
    static const EVENT_TYPE EVENT_MIDI_IN_CC = 0x0B;
    static const EVENT_TYPE EVENT_MIDI_IN_PROGRAM_CHANGE = 0x0C;
    static const EVENT_TYPE EVENT_MIDI_IN_SYSEX = 0x0D;
// @TODO("implement other message types like *System Real-Time Messages* ( aka *Midi Clock* )")

/* --- EVENT_DATA LOCATIONS (MIDI) --- */

    static const EVENT_DATA CHANNEL = 0x00;
    static const EVENT_DATA NOTE = 0x01;
    static const EVENT_DATA VELOCITY = 0x02;
    static const EVENT_DATA NUMBER = 0x01;
    static const EVENT_DATA VALUE = 0x02;
    static const EVENT_DATA PROG_NUM = 0x01;

}

/* --- OSC_ADDRESS_PATTERNS --- */

#define KLANG_OSC_CMD               "/klang/command"
#define KLANG_OSC_MIDI_IN           "/klang/midi_in"
#define KLANG_OSC_MIDI_OUT          "/klang/midi_out"

/* --- INCLUDE MARKER --- */

#define KLANG_DEFINES

#endif /* KlangDefines_hpp */
