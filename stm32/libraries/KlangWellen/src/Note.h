/*
 * KlangWellen
 *
 * This file is part of the *KlangWellen* library (https://github.com/dennisppaul/klangwellen).
 * Copyright (c) 2023 Dennis P Paul
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

#pragma once

#include <stdint.h>

namespace klangwellen {
    class Note {
    public:
        static const uint8_t A0 = 9;
        static const uint8_t A1 = 21;
        static const uint8_t A2 = 33;
        static const uint8_t A3 = 45;
        static const uint8_t A4 = 57;
        static const uint8_t A5 = 69;
        static const uint8_t A6 = 81;
        static const uint8_t A7 = 93;
        static const uint8_t C0 = 0;
        static const uint8_t C1 = 12;
        static const uint8_t C2 = 24;
        static const uint8_t C3 = 36;
        static const uint8_t C4 = 48;
        static const uint8_t C5 = 60;
        static const uint8_t C6 = 72;
        static const uint8_t C7 = 84;
    };
}  // namespace klangwellen