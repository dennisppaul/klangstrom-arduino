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

#ifndef klangstrom_events_h
#define klangstrom_events_h

namespace klangstrom {

    /* ---------------------------------------- ENCODER ---------------------------------------- */

    // static const EVENT_DATA INDEX         = 0x00;
    // static const EVENT_DATA TICK          = 0x01;
    // static const EVENT_DATA PREVIOUS_TICK = 0x02;
    // static const EVENT_DATA DELTA         = 0x03;

    typedef struct {
        float index;
        float ticks;
        float previous_ticks;
        float delta;
    } EventEncoder;

    inline EventEncoder* wrap_encoder_event(const float* pData) {
        return (EventEncoder*)pData;
    }

    inline EventEncoder encoder_event(const float* pData) {
        return *wrap_encoder_event(pData);
    }

    /* ----------------------------------------- MOUSE ----------------------------------------- */

    // static const EVENT_DATA X       = 0x00;
    // static const EVENT_DATA Y       = 0x01;
    // static const EVENT_DATA X_DELTA = 0x02;
    // static const EVENT_DATA Y_DELTA = 0x03;
    // static const EVENT_DATA BUTTON  = 0x04;

    typedef struct {
        float x;
        float y;
        float button;
        float x_delta;
        float y_delta;
    } EventMouse;

    inline EventMouse* wrap_mouse_event(const float* pData) {
        return (EventMouse*)pData;
    }

    inline EventMouse mouse_event(const float* pData) {
        return *wrap_mouse_event(pData);
    }

    /* ----------------------------------------- KEYBOARD ----------------------------------------- */

    typedef struct {
        float key;
    } EventKeyboard;

    inline EventKeyboard* wrap_keyboard_event(const float* pData) {
        return (EventKeyboard*)pData;
    }

    inline EventKeyboard keyboard_event(const float* pData) {
        return *wrap_keyboard_event(pData);
    }

    // @TODO
    //     static const EVENT_TYPE EVENT_KEY_PRESSED    = 0x07;  // [X,Y]
    //     static const EVENT_TYPE EVENT_KEY_RELEASED   = 0x08;  // [X,Y]
    // + MIDI

    //     //                      EVENT TYPE                                  // PAYLOAD

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

}  // namespace klangstrom

#endif  // klangstrom_events_h
