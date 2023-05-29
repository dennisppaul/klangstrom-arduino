/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#ifndef _KLANGSTROM_EVENTS_H_
#define _KLANGSTROM_EVENTS_H_

#include <stdbool.h>
#include <stdint.h>

/* --- DEFS + TYPEDEFS --- */

#define KEYBOARD_NUM_KEYS 6
#define MOUSE_NUM_BUTTONS 3
// @note(why `static const uint8_t` doesn't work but `#define` does is beyond me)
// static const uint8_t KEYBOARD_NUM_KEYS = 6;
// static const uint8_t MOUSE_NUM_BUTTONS = 3;
typedef uint8_t EVENT_TYPE;
typedef uint8_t EVENT_DATA;

/* --- EVENT_TYPE (HID) --- */

static const EVENT_TYPE EVENT_RESERVED_01 = 0x00;
static const EVENT_TYPE EVENT_RESERVED_02 = 0x01;
static const EVENT_TYPE EVENT_RAW_DATA    = 0x02;
static const EVENT_TYPE EVENT_KEYBOARD    = 0x03;
static const EVENT_TYPE EVENT_MOUSE       = 0x04;
static const EVENT_TYPE EVENT_MIDI        = 0x05;
static const EVENT_TYPE EVENT_ENCODER     = 0x06;

/* --- EVENT_DATA LOCATIONS + VALUES (PERIPHERALS) --- */

static const EVENT_DATA ENCODER_00 = 0x00;
static const EVENT_DATA ENCODER_01 = 0x01;
static const EVENT_DATA ENCODER_02 = 0x02;

/* --- EVENT_DATA LOCATIONS + VALUES (HID) --- */

static const EVENT_DATA MOUSE_BUTTON_LEFT   = 0x00;
static const EVENT_DATA MOUSE_BUTTON_MIDDLE = 0x01;
static const EVENT_DATA MOUSE_BUTTON_RIGHT  = 0x02;
static const EVENT_DATA MOUSE_BUTTON_NONE   = 0xFF;

/* ---------------------------------------- ENCODER ---------------------------------------- */

typedef struct {
    uint8_t index;
    int16_t ticks;
    int16_t ticks_previous;
    int16_t delta;
    bool    button;
} EventEncoder;

inline EventEncoder* wrap_encoder_event(const void* data) {
    return (EventEncoder*)data;
}

inline EventEncoder encoder_event(const void* data) {
    return *wrap_encoder_event(data);
}

/* ----------------------------------------- MOUSE ----------------------------------------- */

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

static const uint8_t KEY_TAB       = 9;
static const uint8_t KEY_RETURN    = 10;
static const uint8_t KEY_BACKSPACE = 13;
static const uint8_t KEY_SPACE     = 32;

static const uint8_t KEYCODE_NONE                            = 0x00;
static const uint8_t KEYCODE_ERRORROLLOVER                   = 0x01;
static const uint8_t KEYCODE_POSTFAIL                        = 0x02;
static const uint8_t KEYCODE_ERRORUNDEFINED                  = 0x03;
static const uint8_t KEYCODE_A                               = 0x04;
static const uint8_t KEYCODE_B                               = 0x05;
static const uint8_t KEYCODE_C                               = 0x06;
static const uint8_t KEYCODE_D                               = 0x07;
static const uint8_t KEYCODE_E                               = 0x08;
static const uint8_t KEYCODE_F                               = 0x09;
static const uint8_t KEYCODE_G                               = 0x0A;
static const uint8_t KEYCODE_H                               = 0x0B;
static const uint8_t KEYCODE_I                               = 0x0C;
static const uint8_t KEYCODE_J                               = 0x0D;
static const uint8_t KEYCODE_K                               = 0x0E;
static const uint8_t KEYCODE_L                               = 0x0F;
static const uint8_t KEYCODE_M                               = 0x10;
static const uint8_t KEYCODE_N                               = 0x11;
static const uint8_t KEYCODE_O                               = 0x12;
static const uint8_t KEYCODE_P                               = 0x13;
static const uint8_t KEYCODE_Q                               = 0x14;
static const uint8_t KEYCODE_R                               = 0x15;
static const uint8_t KEYCODE_S                               = 0x16;
static const uint8_t KEYCODE_T                               = 0x17;
static const uint8_t KEYCODE_U                               = 0x18;
static const uint8_t KEYCODE_V                               = 0x19;
static const uint8_t KEYCODE_W                               = 0x1A;
static const uint8_t KEYCODE_X                               = 0x1B;
static const uint8_t KEYCODE_Y                               = 0x1C;
static const uint8_t KEYCODE_Z                               = 0x1D;
static const uint8_t KEYCODE_1_EXCLAMATION_MARK              = 0x1E;
static const uint8_t KEYCODE_2_AT                            = 0x1F;
static const uint8_t KEYCODE_3_NUMBER_SIGN                   = 0x20;
static const uint8_t KEYCODE_4_DOLLAR                        = 0x21;
static const uint8_t KEYCODE_5_PERCENT                       = 0x22;
static const uint8_t KEYCODE_6_CARET                         = 0x23;
static const uint8_t KEYCODE_7_AMPERSAND                     = 0x24;
static const uint8_t KEYCODE_8_ASTERISK                      = 0x25;
static const uint8_t KEYCODE_9_OPARENTHESIS                  = 0x26;
static const uint8_t KEYCODE_0_CPARENTHESIS                  = 0x27;
static const uint8_t KEYCODE_ENTER                           = 0x28;
static const uint8_t KEYCODE_ESCAPE                          = 0x29;
static const uint8_t KEYCODE_BACKSPACE                       = 0x2A;
static const uint8_t KEYCODE_TAB                             = 0x2B;
static const uint8_t KEYCODE_SPACEBAR                        = 0x2C;
static const uint8_t KEYCODE_MINUS_UNDERSCORE                = 0x2D;
static const uint8_t KEYCODE_EQUAL_PLUS                      = 0x2E;
static const uint8_t KEYCODE_OBRACKET_AND_OBRACE             = 0x2F;
static const uint8_t KEYCODE_CBRACKET_AND_CBRACE             = 0x30;
static const uint8_t KEYCODE_BACKSLASH_VERTICAL_BAR          = 0x31;
static const uint8_t KEYCODE_NONUS_NUMBER_SIGN_TILDE         = 0x32;
static const uint8_t KEYCODE_SEMICOLON_COLON                 = 0x33;
static const uint8_t KEYCODE_SINGLE_AND_DOUBLE_QUOTE         = 0x34;
static const uint8_t KEY_GRAVE_ACCENT_AND_TILDE              = 0x35;
static const uint8_t KEYCODE_COMMA_AND_LESS                  = 0x36;
static const uint8_t KEYCODE_DOT_GREATER                     = 0x37;
static const uint8_t KEYCODE_SLASH_QUESTION                  = 0x38;
static const uint8_t KEY_CAPS_LOCK                           = 0x39;
static const uint8_t KEYCODE_F1                              = 0x3A;
static const uint8_t KEYCODE_F2                              = 0x3B;
static const uint8_t KEYCODE_F3                              = 0x3C;
static const uint8_t KEYCODE_F4                              = 0x3D;
static const uint8_t KEYCODE_F5                              = 0x3E;
static const uint8_t KEYCODE_F6                              = 0x3F;
static const uint8_t KEYCODE_F7                              = 0x40;
static const uint8_t KEYCODE_F8                              = 0x41;
static const uint8_t KEYCODE_F9                              = 0x42;
static const uint8_t KEYCODE_F10                             = 0x43;
static const uint8_t KEYCODE_F11                             = 0x44;
static const uint8_t KEYCODE_F12                             = 0x45;
static const uint8_t KEYCODE_PRINTSCREEN                     = 0x46;
static const uint8_t KEY_SCROLL_LOCK                         = 0x47;
static const uint8_t KEYCODE_PAUSE                           = 0x48;
static const uint8_t KEYCODE_INSERT                          = 0x49;
static const uint8_t KEYCODE_HOME                            = 0x4A;
static const uint8_t KEYCODE_PAGEUP                          = 0x4B;
static const uint8_t KEYCODE_DELETE                          = 0x4C;
static const uint8_t KEYCODE_END1                            = 0x4D;
static const uint8_t KEYCODE_PAGEDOWN                        = 0x4E;
static const uint8_t KEYCODE_RIGHTARROW                      = 0x4F;
static const uint8_t KEYCODE_ARROW_RIGHT                     = KEYCODE_RIGHTARROW;
static const uint8_t KEYCODE_LEFTARROW                       = 0x50;
static const uint8_t KEYCODE_ARROW_LEFT                      = KEYCODE_LEFTARROW;
static const uint8_t KEYCODE_DOWNARROW                       = 0x51;
static const uint8_t KEYCODE_ARROW_DOWN                      = KEYCODE_DOWNARROW;
static const uint8_t KEYCODE_UPARROW                         = 0x52;
static const uint8_t KEYCODE_ARROW_UP                        = KEYCODE_UPARROW;
static const uint8_t KEYCODE_KEYPAD_NUM_LOCK_AND_CLEAR       = 0x53;
static const uint8_t KEYCODE_KEYPAD_SLASH                    = 0x54;
static const uint8_t KEYCODE_KEYPAD_ASTERIKS                 = 0x55;
static const uint8_t KEYCODE_KEYPAD_MINUS                    = 0x56;
static const uint8_t KEYCODE_KEYPAD_PLUS                     = 0x57;
static const uint8_t KEYCODE_KEYPAD_ENTER                    = 0x58;
static const uint8_t KEYCODE_KEYPAD_1_END                    = 0x59;
static const uint8_t KEYCODE_KEYPAD_2_DOWN_ARROW             = 0x5A;
static const uint8_t KEYCODE_KEYPAD_3_PAGEDN                 = 0x5B;
static const uint8_t KEYCODE_KEYPAD_4_LEFT_ARROW             = 0x5C;
static const uint8_t KEYCODE_KEYPAD_5                        = 0x5D;
static const uint8_t KEYCODE_KEYPAD_6_RIGHT_ARROW            = 0x5E;
static const uint8_t KEYCODE_KEYPAD_7_HOME                   = 0x5F;
static const uint8_t KEYCODE_KEYPAD_8_UP_ARROW               = 0x60;
static const uint8_t KEYCODE_KEYPAD_9_PAGEUP                 = 0x61;
static const uint8_t KEYCODE_KEYPAD_0_INSERT                 = 0x62;
static const uint8_t KEYCODE_KEYPAD_DECIMAL_SEPARATOR_DELETE = 0x63;
static const uint8_t KEYCODE_NONUS_BACK_SLASH_VERTICAL_BAR   = 0x64;
static const uint8_t KEYCODE_APPLICATION                     = 0x65;
static const uint8_t KEYCODE_POWER                           = 0x66;
static const uint8_t KEYCODE_KEYPAD_EQUAL                    = 0x67;
static const uint8_t KEYCODE_F13                             = 0x68;
static const uint8_t KEYCODE_F14                             = 0x69;
static const uint8_t KEYCODE_F15                             = 0x6A;
static const uint8_t KEYCODE_F16                             = 0x6B;
static const uint8_t KEYCODE_F17                             = 0x6C;
static const uint8_t KEYCODE_F18                             = 0x6D;
static const uint8_t KEYCODE_F19                             = 0x6E;
static const uint8_t KEYCODE_F20                             = 0x6F;
static const uint8_t KEYCODE_F21                             = 0x70;
static const uint8_t KEYCODE_F22                             = 0x71;
static const uint8_t KEYCODE_F23                             = 0x72;
static const uint8_t KEYCODE_F24                             = 0x73;
static const uint8_t KEYCODE_EXECUTE                         = 0x74;
static const uint8_t KEYCODE_HELP                            = 0x75;
static const uint8_t KEYCODE_MENU                            = 0x76;
static const uint8_t KEYCODE_SELECT                          = 0x77;
static const uint8_t KEYCODE_STOP                            = 0x78;
static const uint8_t KEYCODE_AGAIN                           = 0x79;
static const uint8_t KEYCODE_UNDO                            = 0x7A;
static const uint8_t KEYCODE_CUT                             = 0x7B;
static const uint8_t KEYCODE_COPY                            = 0x7C;
static const uint8_t KEYCODE_PASTE                           = 0x7D;
static const uint8_t KEYCODE_FIND                            = 0x7E;
static const uint8_t KEYCODE_MUTE                            = 0x7F;
static const uint8_t KEYCODE_VOLUME_UP                       = 0x80;
static const uint8_t KEYCODE_VOLUME_DOWN                     = 0x81;
static const uint8_t KEYCODE_LOCKING_CAPS_LOCK               = 0x82;
static const uint8_t KEYCODE_LOCKING_NUM_LOCK                = 0x83;
static const uint8_t KEYCODE_LOCKING_SCROLL_LOCK             = 0x84;
static const uint8_t KEYCODE_KEYPAD_COMMA                    = 0x85;
static const uint8_t KEYCODE_KEYPAD_EQUAL_SIGN               = 0x86;
static const uint8_t KEYCODE_INTERNATIONAL1                  = 0x87;
static const uint8_t KEYCODE_INTERNATIONAL2                  = 0x88;
static const uint8_t KEYCODE_INTERNATIONAL3                  = 0x89;
static const uint8_t KEYCODE_INTERNATIONAL4                  = 0x8A;
static const uint8_t KEYCODE_INTERNATIONAL5                  = 0x8B;
static const uint8_t KEYCODE_INTERNATIONAL6                  = 0x8C;
static const uint8_t KEYCODE_INTERNATIONAL7                  = 0x8D;
static const uint8_t KEYCODE_INTERNATIONAL8                  = 0x8E;
static const uint8_t KEYCODE_INTERNATIONAL9                  = 0x8F;
static const uint8_t KEYCODE_LANG1                           = 0x90;
static const uint8_t KEYCODE_LANG2                           = 0x91;
static const uint8_t KEYCODE_LANG3                           = 0x92;
static const uint8_t KEYCODE_LANG4                           = 0x93;
static const uint8_t KEYCODE_LANG5                           = 0x94;
static const uint8_t KEYCODE_LANG6                           = 0x95;
static const uint8_t KEYCODE_LANG7                           = 0x96;
static const uint8_t KEYCODE_LANG8                           = 0x97;
static const uint8_t KEYCODE_LANG9                           = 0x98;
static const uint8_t KEYCODE_ALTERNATE_ERASE                 = 0x99;
static const uint8_t KEYCODE_SYSREQ                          = 0x9A;
static const uint8_t KEYCODE_CANCEL                          = 0x9B;
static const uint8_t KEYCODE_CLEAR                           = 0x9C;
static const uint8_t KEYCODE_PRIOR                           = 0x9D;
static const uint8_t KEYCODE_RETURN                          = 0x9E;
static const uint8_t KEYCODE_SEPARATOR                       = 0x9F;
static const uint8_t KEYCODE_OUT                             = 0xA0;
static const uint8_t KEYCODE_OPER                            = 0xA1;
static const uint8_t KEYCODE_CLEAR_AGAIN                     = 0xA2;
static const uint8_t KEYCODE_CRSEL                           = 0xA3;
static const uint8_t KEYCODE_EXSEL                           = 0xA4;
static const uint8_t KEYCODE_KEYPAD_00                       = 0xB0;
static const uint8_t KEYCODE_KEYPAD_000                      = 0xB1;
static const uint8_t KEYCODE_THOUSANDS_SEPARATOR             = 0xB2;
static const uint8_t KEYCODE_DECIMAL_SEPARATOR               = 0xB3;
static const uint8_t KEYCODE_CURRENCY_UNIT                   = 0xB4;
static const uint8_t KEYCODE_CURRENCY_SUB_UNIT               = 0xB5;
static const uint8_t KEYCODE_KEYPAD_OPARENTHESIS             = 0xB6;
static const uint8_t KEYCODE_KEYPAD_CPARENTHESIS             = 0xB7;
static const uint8_t KEYCODE_KEYPAD_OBRACE                   = 0xB8;
static const uint8_t KEYCODE_KEYPAD_CBRACE                   = 0xB9;
static const uint8_t KEYCODE_KEYPAD_TAB                      = 0xBA;
static const uint8_t KEYCODE_KEYPAD_BACKSPACE                = 0xBB;
static const uint8_t KEYCODE_KEYPAD_A                        = 0xBC;
static const uint8_t KEYCODE_KEYPAD_B                        = 0xBD;
static const uint8_t KEYCODE_KEYPAD_C                        = 0xBE;
static const uint8_t KEYCODE_KEYPAD_D                        = 0xBF;
static const uint8_t KEYCODE_KEYPAD_E                        = 0xC0;
static const uint8_t KEYCODE_KEYPAD_F                        = 0xC1;
static const uint8_t KEYCODE_KEYPAD_XOR                      = 0xC2;
static const uint8_t KEYCODE_KEYPAD_CARET                    = 0xC3;
static const uint8_t KEYCODE_KEYPAD_PERCENT                  = 0xC4;
static const uint8_t KEYCODE_KEYPAD_LESS                     = 0xC5;
static const uint8_t KEYCODE_KEYPAD_GREATER                  = 0xC6;
static const uint8_t KEYCODE_KEYPAD_AMPERSAND                = 0xC7;
static const uint8_t KEYCODE_KEYPAD_LOGICAL_AND              = 0xC8;
static const uint8_t KEYCODE_KEYPAD_VERTICAL_BAR             = 0xC9;
static const uint8_t KEYCODE_KEYPAD_LOGIACL_OR               = 0xCA;
static const uint8_t KEYCODE_KEYPAD_COLON                    = 0xCB;
static const uint8_t KEYCODE_KEYPAD_NUMBER_SIGN              = 0xCC;
static const uint8_t KEYCODE_KEYPAD_SPACE                    = 0xCD;
static const uint8_t KEYCODE_KEYPAD_AT                       = 0xCE;
static const uint8_t KEYCODE_KEYPAD_EXCLAMATION_MARK         = 0xCF;
static const uint8_t KEYCODE_KEYPAD_MEMORY_STORE             = 0xD0;
static const uint8_t KEYCODE_KEYPAD_MEMORY_RECALL            = 0xD1;
static const uint8_t KEYCODE_KEYPAD_MEMORY_CLEAR             = 0xD2;
static const uint8_t KEYCODE_KEYPAD_MEMORY_ADD               = 0xD3;
static const uint8_t KEYCODE_KEYPAD_MEMORY_SUBTRACT          = 0xD4;
static const uint8_t KEYCODE_KEYPAD_MEMORY_MULTIPLY          = 0xD5;
static const uint8_t KEYCODE_KEYPAD_MEMORY_DIVIDE            = 0xD6;
static const uint8_t KEYCODE_KEYPAD_PLUSMINUS                = 0xD7;
static const uint8_t KEYCODE_KEYPAD_CLEAR                    = 0xD8;
static const uint8_t KEYCODE_KEYPAD_CLEAR_ENTRY              = 0xD9;
static const uint8_t KEYCODE_KEYPAD_BINARY                   = 0xDA;
static const uint8_t KEYCODE_KEYPAD_OCTAL                    = 0xDB;
static const uint8_t KEYCODE_KEYPAD_DECIMAL                  = 0xDC;
static const uint8_t KEYCODE_KEYPAD_HEXADECIMAL              = 0xDD;
static const uint8_t KEYCODE_LEFTCONTROL                     = 0xE0;
static const uint8_t KEYCODE_LEFTSHIFT                       = 0xE1;
static const uint8_t KEYCODE_LEFTALT                         = 0xE2;
static const uint8_t KEYCODE_LEFT_GUI                        = 0xE3;
static const uint8_t KEYCODE_RIGHTCONTROL                    = 0xE4;
static const uint8_t KEYCODE_RIGHTSHIFT                      = 0xE5;
static const uint8_t KEYCODE_RIGHTALT                        = 0xE6;
static const uint8_t KEYCODE_RIGHT_GUI                       = 0xE7;

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

/* ------------------------------------------- MIDI ------------------------------------------- */

typedef struct {
    uint8_t  channel;
    uint8_t  command;
    uint16_t a;
    uint8_t  b;
} EventMIDI;

inline EventMIDI* wrap_midi_event(const void* pData) {
    return (EventMIDI*)pData;
}

inline EventMIDI midi_event(const void* pData) {
    return *wrap_midi_event(pData);
}

// /* --- EVENT_TYPE (MIDI) --- */

static const EVENT_TYPE MIDI_NONE                  = 0x00;
static const EVENT_TYPE MIDI_NOTE_OFF              = 0x80;
static const EVENT_TYPE MIDI_NOTE_ON               = 0x90;
static const EVENT_TYPE MIDI_AFTERTOUCH            = 0xA0;
static const EVENT_TYPE MIDI_CONTROL_CHANGE        = 0xB0;
static const EVENT_TYPE MIDI_PROGRAM_CHANGE        = 0xC0;
static const EVENT_TYPE MIDI_CHANNEL_PRESSURE      = 0xD0;
static const EVENT_TYPE MIDI_PITCHBEND             = 0xE0;
static const EVENT_TYPE MIDI_SYS_EX                = 0xF0;
static const EVENT_TYPE MIDI_MTCQFRAME             = 0xF1;  // dataB = quarter frame number
static const EVENT_TYPE MIDI_SONG_POSITION_POINTER = 0xF2;
static const EVENT_TYPE MIDI_SONG_SELECT           = 0xF3;  // dataB = song#
static const EVENT_TYPE MIDI_UNDEF_1               = 0xF4;
static const EVENT_TYPE MIDI_UNDEF_2               = 0xF5;
static const EVENT_TYPE MIDI_TUNE_REQ              = 0xF6;  // no data, return oscillators
static const EVENT_TYPE MIDI_ENDEX                 = 0xF7;  // ends a SYSEX dump
static const EVENT_TYPE MIDI_CLOCK_TICK            = 0xF8;
static const EVENT_TYPE MIDI_RT_CLOCK              = 0xF8;  // *
static const EVENT_TYPE MIDI_RT_UNDEF_1            = 0xF9;
static const EVENT_TYPE MIDI_CLOCK_START           = 0xFA;
static const EVENT_TYPE MIDI_RT_START              = 0xFA;  // *
static const EVENT_TYPE MIDI_CLOCK_CONTINUE        = 0xFB;
static const EVENT_TYPE MIDI_RT_CONTINUE           = 0xFB;  // *
static const EVENT_TYPE MIDI_CLOCK_STOP            = 0xFC;
static const EVENT_TYPE MIDI_RT_STOP               = 0xFC;  // *
static const EVENT_TYPE MIDI_RT_UNDEF_2            = 0xFD;
static const EVENT_TYPE MIDI_RT_SENSE              = 0xFE;  // Sent every 300ms on a live connection. Can ignore.
static const EVENT_TYPE MIDI_RT_RESET              = 0xFF;  // Reset any parameters to their power up values
static const EVENT_TYPE MIDI_CC_MODWHEEL           = 1;     // 0-127
static const EVENT_TYPE MIDI_CC_BREATH             = 2;     // 0-127
static const EVENT_TYPE MIDI_CC_VOLUME             = 7;     // Channel mix volume
static const EVENT_TYPE MIDI_CC_PAN                = 10;    // 0-127; 64=centered
static const EVENT_TYPE MIDI_CC_EXPRESSION         = 11;    // playback dynamics
static const EVENT_TYPE MIDI_CC_SUSTAIN            = 64;    // 0=off; 127=on
static const EVENT_TYPE MIDI_CC_PORTAMENTO         = 65;    // 0=off; 127=on
static const EVENT_TYPE MIDI_CC_RESET              = 121;   // None. Also called "All Sound Off"
static const EVENT_TYPE MIDI_CC_MODE_LOCAL         = 122;   // 0=off; 127=on. Keyboard on/off
static const EVENT_TYPE MIDI_CC_ALL_NOTES_OFF      = 123;   // None
static const EVENT_TYPE MIDI_CC_OMNI_OFF           = 124;   // None
static const EVENT_TYPE MIDI_CC_OMNI_ON            = 125;   // None
static const EVENT_TYPE MIDI_CC_POLY_OFF           = 126;   // #channels (MONO ON)
static const EVENT_TYPE MIDI_CC_POLY_ON            = 127;   // None (MONO OFF)

// // @REF([Summary of MIDI Messages](https://www.midi.org/specifications/item/table-1-summary-of-midi-message))

// static const EVENT_TYPE EVENT_MIDI_IN_NOTE_ON        = 0x09;  // [???]
// static const EVENT_TYPE EVENT_MIDI_IN_NOTE_OFF       = 0x0A;
// static const EVENT_TYPE EVENT_MIDI_IN_CC             = 0x0B;
// static const EVENT_TYPE EVENT_MIDI_IN_PROGRAM_CHANGE = 0x0C;
// static const EVENT_TYPE EVENT_MIDI_IN_SYSEX          = 0x0D;
// // @TODO("implement other message types like *System Real-Time Messages* ( aka *Midi Clock* )")

// /* --- EVENT_DATA LOCATIONS (MIDI) --- */

// static const EVENT_DATA CHANNEL  = 0x00;
// static const EVENT_DATA NOTE     = 0x01;
// static const EVENT_DATA VELOCITY = 0x02;
// static const EVENT_DATA NUMBER   = 0x01;
// static const EVENT_DATA VALUE    = 0x02;
// static const EVENT_DATA PROG_NUM = 0x01;

#endif  // _KLANGSTROM_EVENTS_H_
