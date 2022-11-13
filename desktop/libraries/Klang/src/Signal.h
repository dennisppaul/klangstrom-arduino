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

#ifndef _KLANG_SIGNAL_H_
#define _KLANG_SIGNAL_H_

#include <cstdint>

namespace klang {

    class Signal {
    public:
        float *signal;
        const uint8_t channels;

        explicit Signal(uint8_t pNumChannels = 1) : channels(pNumChannels) {
            signal = new float[channels];
        }

        Signal(const Signal &s) : channels(s.channels) {
            signal = s.signal;
        }

        ~Signal() {
            delete[] signal;
        }
    };

}  // namespace klang

#endif //_KLANG_SIGNAL_H_
