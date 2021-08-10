//
//  KlangstromDefines.hpp
//  Klangstrom
//
//
//

#ifndef KlangstromDefines_hpp
#define KlangstromDefines_hpp

//@todo(@maybe add application specific debugging flags)

/* --- USER DEFINES --- */

#define SIGNAL_TYPE_FLOAT 1
#define SIGNAL_TYPE_INT16 2

#if __has_include("KlangConfiguration.h")
#include "KlangConfiguration.h"
#endif

#ifndef KLANG_AUDIO_RATE
#define KLANG_AUDIO_RATE                  48000
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_RATE to default value: 48000
#endif
#endif

#ifndef KLANG_AUDIO_BLOCKS
#define KLANG_AUDIO_BLOCKS                32
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_AUDIO_BLOCKS to default value: 32
#endif
#endif

#ifndef KLANG_SAMPLES_PER_AUDIO_BLOCK
#define KLANG_SAMPLES_PER_AUDIO_BLOCK     512
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_SAMPLES_PER_AUDIO_BLOCK to default value: 512
#endif
#endif

#ifndef KLANG_SIGNAL_TYPE
#define KLANG_SIGNAL_TYPE                 SIGNAL_TYPE_FLOAT
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_SIGNAL_TYPE to default value: SIGNAL_TYPE_FLOAT
#endif
#endif

/* --- SIGNAL --- */

#if (KLANG_SIGNAL_TYPE == SIGNAL_TYPE_INT16)
typedef int16_t SIGNAL_TYPE;
#elif (KLANG_SIGNAL_TYPE == SIGNAL_TYPE_FLOAT)
typedef float SIGNAL_TYPE;
#else
#error KLANG_SIGNAL_TYPE not defined
#endif

/* --- TYPEDEFS --- */

typedef uint8_t DATA_PERIPHERAL_TYPE;
typedef uint8_t EVENT_TYPE;
typedef uint8_t EVENT_DATA;

/**
 * this block defines constants for `event_transmit` + `event_receive` operations
 * including some peripherals like encoders + buttons
 */
namespace klangstrom {

    /* --- EVENT_TYPE (HID) --- */

    // @todo(add payload description as comment. see `EVENT_ENCODER_ROTATE_XX`)

    //                      EVENT TYPE                                  // PAYLOAD

    static const EVENT_TYPE EVENT_RESERVED_01    = 0x00;
    static const EVENT_TYPE EVENT_RESERVED_02    = 0x01;
    static const EVENT_TYPE EVENT_RAW_DATA       = 0x02;
    static const EVENT_TYPE EVENT_MOUSE_PRESSED  = 0x03;  // [X,Y]
    static const EVENT_TYPE EVENT_MOUSE_RELEASED = 0x04;  // [X,Y]
    static const EVENT_TYPE EVENT_MOUSE_MOVED    = 0x05;  // [X,Y]
    static const EVENT_TYPE EVENT_MOUSE_DRAGGED  = 0x06;  // [X,Y]
    static const EVENT_TYPE EVENT_KEY_PRESSED    = 0x07;  // [X,Y]
    static const EVENT_TYPE EVENT_KEY_RELEASED   = 0x08;  // [X,Y]

    /* --- EVENT_TYPE (PERIPHERALS) --- */

    static const EVENT_TYPE EVENT_ENCODER_ROTATE          = 0x09;  // [INDEX,TICK,PREVIOUS_TICK]
    static const EVENT_TYPE EVENT_ENCODER_BUTTON_PRESSED  = 0x0A;  // [INDEX]
    static const EVENT_TYPE EVENT_ENCODER_BUTTON_RELEASED = 0x0B;  // [INDEX]

    /* --- EVENT_DATA LOCATIONS + VALUES (PERIPHERALS) --- */

    static const EVENT_DATA INDEX         = 0x00;
    static const EVENT_DATA TICK          = 0x01;
    static const EVENT_DATA PREVIOUS_TICK = 0x02;
    static const EVENT_DATA ENCODER_00    = 0x00;
    static const EVENT_DATA ENCODER_01    = 0x01;
    static const EVENT_DATA ENCODER_02    = 0x02;

    /* --- EVENT_DATA LOCATIONS + VALUES (HID) --- */

    static const EVENT_DATA X      = 0x00;
    static const EVENT_DATA Y      = 0x01;
    static const EVENT_DATA BUTTON = 0x02;
    static const EVENT_DATA LEFT   = 0x00;
    static const EVENT_DATA MIDDLE = 0x01;
    static const EVENT_DATA RIGHT  = 0x02;
    static const EVENT_DATA KEY    = 0x00;

    /* --- EVENT_TYPE (MIDI) --- */

    // @REF([Summary of MIDI Messages](https://www.midi.org/specifications/item/table-1-summary-of-midi-message))

    static const EVENT_TYPE EVENT_MIDI_IN_NOTE_ON        = 0x09;  // [???]
    static const EVENT_TYPE EVENT_MIDI_IN_NOTE_OFF       = 0x0A;
    static const EVENT_TYPE EVENT_MIDI_IN_CC             = 0x0B;
    static const EVENT_TYPE EVENT_MIDI_IN_PROGRAM_CHANGE = 0x0C;
    static const EVENT_TYPE EVENT_MIDI_IN_SYSEX          = 0x0D;
    // @TODO("implement other message types like *System Real-Time Messages* ( aka *Midi Clock* )")

    /* --- EVENT_DATA LOCATIONS (MIDI) --- */

    static const EVENT_DATA CHANNEL  = 0x00;
    static const EVENT_DATA NOTE     = 0x01;
    static const EVENT_DATA VELOCITY = 0x02;
    static const EVENT_DATA NUMBER   = 0x01;
    static const EVENT_DATA VALUE    = 0x02;
    static const EVENT_DATA PROG_NUM = 0x01;
}  // namespace klangstrom

/**
 * this block defines constants for `data_transmit` + `data_receive` operations
 * including peripherals like UART, SPI, I2C
 */
namespace klangstrom {
    static const DATA_PERIPHERAL_TYPE PERIPHERAL_RESERVED_01 = 0x00;
    static const DATA_PERIPHERAL_TYPE ALL_PERIPHERALS        = 0x01;
    static const DATA_PERIPHERAL_TYPE SERIAL_00              = 0x02;
    static const DATA_PERIPHERAL_TYPE SERIAL_01              = 0x03;
    static const DATA_PERIPHERAL_TYPE SERIAL_02              = 0x04;
};  // namespace klangstrom

/* --- OSC_ADDRESS_PATTERNS --- */

/**
 * this block defines constants for OSC communications
 */
namespace klangstrom {
    static const char KLANG_OSC_CMD[]      = "/klang/command";
    static const char KLANG_OSC_DATA[]     = "/klang/data";
    static const char KLANG_OSC_MIDI_IN[]  = "/klang/midi_in";
    static const char KLANG_OSC_MIDI_OUT[] = "/klang/midi_out";
    static const char KLANG_OSC_SIM[]      = "/klang/sim";
};  // namespace klangstrom

#ifndef KLANG_OSC_TRANSMIT_ADDRESS
#define KLANG_OSC_TRANSMIT_ADDRESS "224.0.0.1"
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_OSC_TRANSMIT_ADDRESS to default value: "224.0.0.1"
#endif
#endif

#ifndef KLANG_OSC_TRANSMIT_PORT
#define KLANG_OSC_TRANSMIT_PORT 7000
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_OSC_TRANSMIT_PORT to default value: 7000
#endif
#endif

#ifndef KLANG_OSC_RECEIVE_PORT
#define KLANG_OSC_RECEIVE_PORT 7001
#ifdef DEBUG_SHOW_DEFAULT_WARNINGS
#warning setting KLANG_OSC_RECEIVE_PORT to default value: 7001
#endif
#endif

#endif /* KlangstromDefines_hpp */
