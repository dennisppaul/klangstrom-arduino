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

#ifndef KlangEventListener_hpp
#define KlangEventListener_hpp

#include <stdint.h>

#include <vector>

#include "Klang.hpp"

namespace klang {
    class KlangEventListener {
    public:
        virtual void event(KLANG_EVENT pEventType, std::vector<float> pData) = 0;
    };
}  // namespace strom

#endif /* KlangEventListener_hpp */
