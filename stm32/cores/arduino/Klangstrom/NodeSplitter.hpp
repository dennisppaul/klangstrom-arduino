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

/**
 *       [ NODE_SPLITTER       ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUTFF
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeSplitter_hpp
#define NodeSplitter_hpp

#include "NodeBuffer.hpp"

namespace klang {
    class NodeSplitter : public NodeBuffer {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        bool ouput_allow_multiple_connections() { return true; }
    };
}  // namespace klang

#endif /* NodeSplitter_hpp */
