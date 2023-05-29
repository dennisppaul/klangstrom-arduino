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

// @TODO(fix temporary deprecation warning for `sprintf` ("-Wdeprecated-declarations"))

#ifndef _KLANGSTROM_MIDI_PARSER_H_
#define _KLANGSTROM_MIDI_PARSER_H_

#include <cstdio>
#include <functional>
#include <string>

#include "Klangstrom.h"
#include "MIDIParser.h"

#ifdef MIDI_PARSER_DISABLE_PRINT_MESSAGES
#warning "MIDI Parser messages are disabled."
#endif

namespace klangstrom {
    class KlangstromMIDIParser {
    private:
        typedef void (*CallbackType0)(void);
        typedef void (*CallbackType1_UI8)(uint8_t);
        typedef void (*CallbackType1_UI16)(uint16_t);
        typedef void (*CallbackType2_UI8)(uint8_t, uint8_t);
        typedef void (*CallbackType2_UI8_I16)(uint8_t, uint16_t);
        typedef void (*CallbackType2_UI8_VOIDPTR)(const uint8_t, const void*);
        typedef void (*CallbackType3_UI8)(uint8_t, uint8_t, uint8_t);

        CallbackType2_UI8_VOIDPTR callback_event_receive;
        CallbackType3_UI8         callback_note_off;
        CallbackType3_UI8         callback_note_on;
        CallbackType3_UI8         callback_aftertouch;
        CallbackType3_UI8         callback_control_change;
        CallbackType2_UI8         callback_program_change;
        CallbackType2_UI8         callback_channel_pressure;
        CallbackType2_UI8_I16     callback_pitch_bend;
        CallbackType0             callback_sys_ex;
        CallbackType1_UI8         callback_mtc_quarter_frame;
        CallbackType1_UI16        callback_song_position_pointer;
        CallbackType1_UI8         callback_song_select;
        CallbackType0             callback_tune_req;
        CallbackType0             callback_end_ex;
        CallbackType0             callback_clock_tick;
        CallbackType0             callback_clock_start;
        CallbackType0             callback_clock_continue;
        CallbackType0             callback_clock_stop;
        CallbackType0             callback_rt_sense;
        CallbackType0             callback_rt_reset;

        void handle_channel_message(uint8_t msg) {
            const uint8_t message_type = p.msg_value(msg);
            const uint8_t channel      = p.msg_channel(msg);
            EventMIDI     e;
            e.command = message_type;
            e.channel = channel;
            e.a       = p.get_dataA();
            e.b       = p.get_dataB();
            switch (message_type) {
                case MIDI_NOTE_OFF:
                    if (callback_note_off) {
                        callback_note_off(channel, p.get_dataA(), p.get_dataB());
                    }
                    break;
                case MIDI_NOTE_ON:
                    if (callback_note_on) {
                        callback_note_on(channel, p.get_dataA(), p.get_dataB());
                    }
                    break;
                case MIDI_AFTERTOUCH:
                    if (callback_aftertouch) {
                        callback_aftertouch(channel, p.get_dataA(), p.get_dataB());
                    }
                    break;
                case MIDI_CONTROL_CHANGE:
                    if (callback_control_change) {
                        callback_control_change(channel, p.get_dataA(), p.get_dataB());
                    }
                    break;
                case MIDI_PROGRAM_CHANGE:
                    if (callback_program_change) {
                        callback_program_change(channel, p.get_dataB());
                    }
                    e.a = e.b;
                    break;
                case MIDI_CHANNEL_PRESSURE:
                    if (callback_channel_pressure) {
                        callback_channel_pressure(channel, p.get_dataB());
                    }
                    e.a = e.b;
                    break;
                case MIDI_PITCHBEND:
                    if (callback_pitch_bend) {
                        callback_pitch_bend(channel, p.get_int14() - 8192);
                    }
                    e.a = p.get_int14();
                    e.b = 0;
                    break;
            }
            if (callback_event_receive) {
                callback_event_receive(EVENT_MIDI, &e);
            }
        }

        void handle_sysex_message(uint8_t msg) {
            EventMIDI e;
            e.command = msg;
            e.channel = 0;
            e.b       = 0;
            e.a       = 0;
            switch (msg) {
                case MIDI_SYS_EX:
                    if (callback_sys_ex) {
                        callback_sys_ex();
                    }
                    break;
                case MIDI_MTCQFRAME:
                    if (callback_mtc_quarter_frame) {
                        callback_mtc_quarter_frame(p.get_dataB());
                    }
                    e.b = p.get_dataB();
                    e.a = e.b;
                    break;
                case MIDI_SONG_POSITION_POINTER:
                    if (callback_song_position_pointer) {
                        callback_song_position_pointer(p.get_int14());
                    }
                    e.a = p.get_int14();
                    e.b = 0;
                    break;
                case MIDI_SONG_SELECT:
                    if (callback_song_select) {
                        callback_song_select(p.get_dataB());
                    }
                    e.b = p.get_dataB();
                    e.a = e.b;
                    break;
                case MIDI_TUNE_REQ:
                    if (callback_tune_req) {
                        callback_tune_req();
                    }
                    break;
                case MIDI_ENDEX:
                    if (callback_end_ex) {
                        callback_end_ex();
                    }
                    break;
            }
            if (callback_event_receive) {
                callback_event_receive(EVENT_MIDI, &e);
            }
        }

        void handle_rt_message(uint8_t msg) {
            EventMIDI e;
            e.command = msg;
            e.channel = 0;
            e.b       = 0;
            e.a       = 0;
            switch (msg) {
                case MIDI_CLOCK_TICK:
                    if (callback_clock_tick) {
                        callback_clock_tick();
                    }
                    break;
                case MIDI_CLOCK_START:
                    if (callback_clock_start) {
                        callback_clock_start();
                    }
                    break;
                case MIDI_CLOCK_CONTINUE:
                    if (callback_clock_continue) {
                        callback_clock_continue();
                    }
                    break;
                case MIDI_CLOCK_STOP:
                    if (callback_clock_stop) {
                        callback_clock_stop();
                    }
                    break;
                case MIDI_RT_SENSE:
                    if (callback_rt_sense) {
                        callback_rt_sense();
                    }
                    break;
                case MIDI_RT_RESET:
                    if (callback_rt_reset) {
                        callback_rt_reset();
                    }
                    break;
            }
            if (callback_event_receive) {
                callback_event_receive(EVENT_MIDI, &e);
            }
        }

    public:
        void register_event_receive(CallbackType2_UI8_VOIDPTR callback) { callback_event_receive = callback; }
        void register_note_off(CallbackType3_UI8 callback) { callback_note_off = callback; }
        void register_note_on(CallbackType3_UI8 callback) { callback_note_on = callback; }
        void register_aftertouch(CallbackType3_UI8 callback) { callback_aftertouch = callback; }
        void register_control_change(CallbackType3_UI8 callback) { callback_control_change = callback; }
        void register_program_change(CallbackType2_UI8 callback) { callback_program_change = callback; }
        void register_channel_pressure(CallbackType2_UI8 callback) { callback_channel_pressure = callback; }
        void register_pitch_bend(CallbackType2_UI8_I16 callback) { callback_pitch_bend = callback; }
        void register_sys_ex(CallbackType0 callback) { callback_sys_ex = callback; }
        void register_mtc_quarter_frame(CallbackType1_UI8 callback) { callback_mtc_quarter_frame = callback; }
        void register_song_position_pointer(CallbackType1_UI16 callback) { callback_song_position_pointer = callback; }
        void register_song_select(CallbackType1_UI8 callback) { callback_song_select = callback; }
        void register_tune_req(CallbackType0 callback) { callback_tune_req = callback; }
        void register_end_ex(CallbackType0 callback) { callback_end_ex = callback; }
        void register_clock_tick(CallbackType0 callback) { callback_clock_tick = callback; }
        void register_clock_start(CallbackType0 callback) { callback_clock_start = callback; }
        void register_clock_continue(CallbackType0 callback) { callback_clock_continue = callback; }
        void register_clock_stop(CallbackType0 callback) { callback_clock_stop = callback; }
        void register_rt_sense(CallbackType0 callback) { callback_rt_sense = callback; }
        void register_rt_reset(CallbackType0 callback) { callback_rt_reset = callback; }

        EVENT_TYPE parse(char midiByte) {
            const uint8_t msg = p.parse(midiByte);
            if (msg > MIDI_NONE) {
                if (p.is_rtmsg(msg)) {
                    handle_rt_message(msg);
                } else if (p.is_sysmsg(msg)) {
                    handle_sysex_message(msg);
                } else if (p.is_msg(msg)) {
                    handle_channel_message(msg);
                }
            }
            return msg;
        }

        String message_str(uint8_t msg) {
            return message_str(msg, get_dataA(), get_dataB());
        }

        String message_str(uint8_t msg, uint8_t data_a, uint8_t data_b) {
#ifndef MIDI_PARSER_DISABLE_PRINT_MESSAGES
            char const midi_sysmsg_names[16][8] = {
                "SYSEX  ",
                "TMCODE ",
                "SNGPOS ",
                "SNGSEL ",
                "       ",
                "       ",
                "TUNE   ",
                "ENDEX  ",
                "CLOCK  ",
                "       ",
                "START  ",
                "CONT   ",
                "STOP   ",
                "       ",
                "SENSE  ",
                "RESET  ",
            };

            char const midi_msg_names[7][8] = {
                "NOTOFF ",
                "NOTEON ",
                "ATOUCH ",
                "CC CHG ",
                "PRGCHG ",
                "CTOUCH ",
                "P BEND ",
            };
            char buffer[48];
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
            if (msg != MIDI_NONE) {
                if (p.is_rtmsg(msg)) {
                    sprintf(buffer, "RT  : %s 0x%02X", midi_sysmsg_names[msg & 0x0F], (int)msg);
                } else if (p.is_sysmsg(msg)) {
                    sprintf(buffer, "SYS : %s 0x%02X", midi_sysmsg_names[msg & 0x0F], (int)msg);
                } else if (p.is_msg(msg)) {
                    if (p.msg_value(msg) == MIDIParser::PITCH_BEND) {
                        sprintf(buffer, "%s(0x%02X) CH:0x%02X > 0x%04X",
                                midi_msg_names[(msg >> 4) & 0x07],
                                p.msg_value(msg),
                                p.msg_channel(msg),
                                p.get_int14());
                    } else {
                        sprintf(buffer, "%s(0x%02X) CH:0x%02X > 0x%02X, 0x%02X",
                                midi_msg_names[(msg >> 4) & 0x07],
                                p.msg_value(msg),
                                p.msg_channel(msg),
                                p.get_dataA(),
                                p.get_dataB());
                    }
                } else {
                    sprintf(buffer, "ERR");
                }
            }
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
            return std::string(buffer).c_str();
#else
            return "";
#endif
        }

        void reset() {
            p.reset();
        }

        MIDIParser parser() {
            return p;
        }

        uint8_t get_message() const { return p.get_message(); }
        uint8_t get_dataA() const { return p.get_dataA(); }
        uint8_t get_dataB() const { return p.get_dataA(); }

    private:
        MIDIParser p;
    };
}  // namespace klangstrom

#endif  // _KLANGSTROM_MIDI_PARSER_H_