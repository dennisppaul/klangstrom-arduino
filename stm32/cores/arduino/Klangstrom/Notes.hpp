/*
 * Klang – a node+text-based synthesizer library
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

// @TODO(move to strom)

#ifndef Notes_h
#define Notes_h

namespace klang {
    class Notes {
    public:
        static const uint8_t NOTE_A0 = 9;
        static const uint8_t NOTE_A1 = 21;
        static const uint8_t NOTE_A2 = 33;
        static const uint8_t NOTE_A3 = 45;
        static const uint8_t NOTE_A4 = 57;
        static const uint8_t NOTE_A5 = 69;
        static const uint8_t NOTE_A6 = 81;
        static const uint8_t NOTE_A7 = 93;
        static const uint8_t NOTE_C0 = 0;
        static const uint8_t NOTE_C1 = 12;
        static const uint8_t NOTE_C2 = 24;
        static const uint8_t NOTE_C3 = 36;
        static const uint8_t NOTE_C4 = 48;
        static const uint8_t NOTE_C5 = 60;
        static const uint8_t NOTE_C6 = 72;
        static const uint8_t NOTE_C7 = 84;
    };
}  // namespace klang

#endif /* Notes_h */
