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

#ifndef Connection_hpp
#define Connection_hpp

#include "KlangNode.hpp"

namespace klang {
    class Connection {
    public:
        Connection(Node& pOutputNode, CHANNEL_ID pOutputChannel,
                   Node& pInputNode, CHANNEL_ID pInputChannel,
                   bool& pStatus);
        Connection(Node& pOutputNode, Node& pInputNode);
        ~Connection();

        CONNECTION_ID ID() {
            return mID;
        }

        Node&      output_node;
        CHANNEL_ID output_channel_id;
        Node&      input_node;
        CHANNEL_ID input_channel_id;

        void update(SIGNAL_TYPE* pAudioBlock);
        void update(AUDIO_BLOCK_ID pAudioBlockID);

        static void reset();

    private:
        CONNECTION_ID        mID;
        static CONNECTION_ID oID;
    };
}  // namespace klang

#endif /* Connection_hpp */
