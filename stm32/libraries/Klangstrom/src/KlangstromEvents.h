/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>

#ifndef klangstrom_events_h
#define klangstrom_events_h

/* --- TYPEDEFS --- */

typedef uint8_t EVENT_TYPE;
typedef uint8_t EVENT_DATA;

/**
 * this block defines constants for `event_transmit` + `event_receive` operations
 * including some peripherals like encoders + buttons
 */
// namespace klangstrom {

/* --- EVENT_TYPE (HID) --- */

// @todo(add payload description as comment. see `EVENT_ENCODER_ROTATE_XX`)

//                      EVENT TYPE                    // PAYLOAD
// @TOOD(clean up events. do we need *_PRESSED?)
static const EVENT_TYPE EVENT_RESERVED_01    = 0x00;
static const EVENT_TYPE EVENT_RESERVED_02    = 0x01;
static const EVENT_TYPE EVENT_RAW_DATA       = 0x02;
static const EVENT_TYPE EVENT_MOUSE_PRESSED  = 0x03;
static const EVENT_TYPE EVENT_MOUSE_RELEASED = 0x04;
static const EVENT_TYPE EVENT_MOUSE_MOVED    = 0x05;
static const EVENT_TYPE EVENT_MOUSE_DRAGGED  = 0x06;
static const EVENT_TYPE EVENT_KEY_PRESSED    = 0x07;
static const EVENT_TYPE EVENT_KEY_RELEASED   = 0x08;
static const EVENT_TYPE EVENT_KEYBOARD       = 0x09;
static const EVENT_TYPE EVENT_MOUSE          = 0x0A;

/* --- EVENT_TYPE (PERIPHERALS) --- */

static const EVENT_TYPE EVENT_ENCODER_ROTATED         = 0x09;  // [INDEX,TICK,PREVIOUS_TICK]
static const EVENT_TYPE EVENT_ENCODER_BUTTON_PRESSED  = 0x0A;  // [INDEX]
static const EVENT_TYPE EVENT_ENCODER_BUTTON_RELEASED = 0x0B;  // [INDEX]

/* --- EVENT_DATA LOCATIONS + VALUES (PERIPHERALS) --- */

// static const EVENT_DATA INDEX                         = 0x00;
// static const EVENT_DATA TICK                          = 0x01;
// static const EVENT_DATA PREVIOUS_TICK                 = 0x02;
// static const EVENT_DATA DELTA                         = 0x03;
// static const EVENT_DATA ENCODER_ROTATED_NUM_LOCATIONS = 0x04;
// static const EVENT_DATA ENCODER_BUTTON_NUM_LOCATIONS  = 0x01;
static const EVENT_DATA ENCODER_00 = 0x00;
static const EVENT_DATA ENCODER_01 = 0x01;
static const EVENT_DATA ENCODER_02 = 0x02;

/* --- EVENT_DATA LOCATIONS + VALUES (HID) --- */

// static const EVENT_DATA X       = 0x00;
// static const EVENT_DATA Y       = 0x01;
// static const EVENT_DATA X_DELTA = 0x02;
// static const EVENT_DATA Y_DELTA = 0x03;
// static const EVENT_DATA BUTTON  = 0x04;
// static const EVENT_DATA NONE                = 0x00;
// static const EVENT_DATA LEFT                = 0x01;
// static const EVENT_DATA MIDDLE              = 0x02;
// static const EVENT_DATA RIGHT               = 0x03;
static const EVENT_DATA MOUSE_BUTTON_LEFT   = 0x00;
static const EVENT_DATA MOUSE_BUTTON_RIGHT  = 0x01;
static const EVENT_DATA MOUSE_BUTTON_MIDDLE = 0x02;
static const EVENT_DATA MOUSE_BUTTON_NONE   = 0xFF;
// static const EVENT_DATA KEY     = 0x00;

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
// }  // namespace klangstrom

// namespace klangstrom {

/* ---------------------------------------- ENCODER ---------------------------------------- */

// static const EVENT_DATA INDEX         = 0x00;
// static const EVENT_DATA TICK          = 0x01;
// static const EVENT_DATA PREVIOUS_TICK = 0x02;
// static const EVENT_DATA DELTA         = 0x03;

typedef struct {
    uint8_t index;
    int16_t ticks;
    int16_t previous_ticks;
    int16_t delta;
} EventEncoder;

inline EventEncoder* wrap_encoder_event(const void* data) {
    return (EventEncoder*)data;
}

inline EventEncoder encoder_event(const void* data) {
    return *wrap_encoder_event(data);
}

/* ----------------------------------------- MOUSE ----------------------------------------- */

// static const EVENT_DATA X       = 0x00;
// static const EVENT_DATA Y       = 0x01;
// static const EVENT_DATA X_DELTA = 0x02;
// static const EVENT_DATA Y_DELTA = 0x03;
// static const EVENT_DATA BUTTON  = 0x04;

typedef struct {
    int8_t x;
    int8_t y;
    bool   LEFT;
    bool   MIDDLE;
    bool   RIGHT;
} EventMouse;

inline EventMouse* wrap_mouse_event(const void* pData) {
    return (EventMouse*)pData;
}

inline EventMouse mouse_event(const void* pData) {
    return *wrap_mouse_event(pData);
}

/* ----------------------------------------- KEYBOARD ----------------------------------------- */

typedef struct {
    uint8_t* keys;
    uint8_t* keycodes;
    bool     SHIFT_LEFT;
    bool     SHIFT_RIGHT;
    bool     CTRL_LEFT;
    bool     CTRL_RIGHT;
    bool     ALT_LEFT;
    bool     ALT_RIGHT;  // :(
    bool     GUI_LEFT;
    bool     GUI_RIGHT;
} EventKeyboard;

inline EventKeyboard* wrap_keyboard_event(const void* pData) {
    return (EventKeyboard*)pData;
}

inline EventKeyboard keyboard_event(const void* pData) {
    return *wrap_keyboard_event(pData);
}

// @TODO
//     static const EVENT_TYPE EVENT_KEY_PRESSED    = 0x07;  // [X,Y]
//     static const EVENT_TYPE EVENT_KEY_RELEASED   = 0x08;  // [X,Y]
// + MIDI

//     //                      EVENT TYPE                                  // PAYLOAD

/* ------------------------------------------- MIDI ------------------------------------------- */

typedef struct {
    uint8_t cable;
    uint8_t channel;
    uint8_t command;
    uint8_t a;
    uint8_t b;
} EventMIDI;

//     /* --- EVENT_TYPE (MIDI) --- */

//     // @REF([Summary of MIDI Messages](https://www.midi.org/specifications/item/table-1-summary-of-midi-message))

//     static const EVENT_TYPE EVENT_MIDI_IN_NOTE_ON        = 0x09;  // [???]
//     static const EVENT_TYPE EVENT_MIDI_IN_NOTE_OFF       = 0x0A;
//     static const EVENT_TYPE EVENT_MIDI_IN_CC             = 0x0B;
//     static const EVENT_TYPE EVENT_MIDI_IN_PROGRAM_CHANGE = 0x0C;
//     static const EVENT_TYPE EVENT_MIDI_IN_SYSEX          = 0x0D;
//     // @TODO("implement other message types like *System Real-Time Messages* ( aka *Midi Clock* )")

//     /* --- EVENT_DATA LOCATIONS (MIDI) --- */

//     static const EVENT_DATA CHANNEL  = 0x00;
//     static const EVENT_DATA NOTE     = 0x01;
//     static const EVENT_DATA VELOCITY = 0x02;
//     static const EVENT_DATA NUMBER   = 0x01;
//     static const EVENT_DATA VALUE    = 0x02;
//     static const EVENT_DATA PROG_NUM = 0x01;
// }

// }  // namespace klangstrom

#endif  // klangstrom_events_h
