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

    /* --- ENCODER ROTATED --- */

    typedef struct {
        //     static const EVENT_DATA INDEX         = 0x00;
        //     static const EVENT_DATA TICK          = 0x01;
        //     static const EVENT_DATA PREVIOUS_TICK = 0x02;
        //     static const EVENT_DATA DELTA         = 0x03;
        float index;
        float ticks;
        float previous_ticks;
        float delta;
    } EventEncoderRotated;

    inline EventEncoderRotated* wrap_encoder_rotated(const float* pData) {
        return (EventEncoderRotated*)pData;
    }

    inline EventEncoderRotated encoder_rotated(const float* pData) {
        return *wrap_encoder_rotated(pData);
    }

    /* --- ENCODER PRESSED --- */

    typedef struct {
        //     static const EVENT_DATA INDEX         = 0x00;
        float index;
    } EventEncoderPressed;

    inline EventEncoderPressed* wrap_encoder_pressed(const float* pData) {
        return (EventEncoderPressed*)pData;
    }

    inline EventEncoderPressed encoder_pressed(const float* pData) {
        return *wrap_encoder_pressed(pData);
    }

    /* --- ENCODER RELEASED --- */

    typedef struct {
        //     static const EVENT_DATA INDEX         = 0x00;
        float index;
    } EventEncoderReleased;

    inline EventEncoderReleased* wrap_encoder_released(const float* pData) {
        return (EventEncoderReleased*)pData;
    }

    inline EventEncoderReleased encoder_released(const float* pData) {
        return *wrap_encoder_released(pData);
    }

    /* --- MOUSE MOVED --- */

    typedef struct {
        //     static const EVENT_DATA X      = 0x00;
        //     static const EVENT_DATA Y      = 0x01;
        float x;
        float y;
    } EventMouseMoved;

    inline EventMouseMoved* wrap_mouse_moved(const float* pData) {
        return (EventMouseMoved*)pData;
    }

    inline EventMouseMoved mouse_moved(const float* pData) {
        return *wrap_mouse_moved(pData);
    }

    /* --- MOUSE DRAGGED --- */

    typedef struct {
        //     static const EVENT_DATA X      = 0x00;
        //     static const EVENT_DATA Y      = 0x01;
        //     static const EVENT_DATA BUTTON = 0x02;
        float x;
        float y;
        float button;
    } EventMouseDragged;

    inline EventMouseDragged* wrap_mouse_dragged(const float* pData) {
        return (EventMouseDragged*)pData;
    }

    inline EventMouseDragged mouse_dragged(const float* pData) {
        return *wrap_mouse_dragged(pData);
    }

    /* --- MOUSE PRESSED --- */

    typedef struct {
        //     static const EVENT_DATA X      = 0x00;
        //     static const EVENT_DATA Y      = 0x01;
        //     static const EVENT_DATA BUTTON = 0x02;
        float x;
        float y;
        float button;
    } EventMousePressed;

    inline EventMousePressed* wrap_mouse_pressed(const float* pData) {
        return (EventMousePressed*)pData;
    }

    inline EventMousePressed mouse_pressed(const float* pData) {
        return *wrap_mouse_pressed(pData);
    }

    /* --- MOUSE RELEASED --- */

    typedef struct {
        //     static const EVENT_DATA X      = 0x00;
        //     static const EVENT_DATA Y      = 0x01;
        //     static const EVENT_DATA BUTTON = 0x02;
        float x;
        float y;
        float button;
    } EventMouseReleased;

    inline EventMouseReleased* wrap_mouse_released(const float* pData) {
        return (EventMouseReleased*)pData;
    }

    inline EventMouseReleased mouse_released(const float* pData) {
        return *wrap_mouse_released(pData);
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
