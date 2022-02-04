/*
 * Strom – a node+text-based composition library
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

#include "Cable.hpp"

using namespace strom;

Cable::Cable(StromNode*       pOutNode,
             STROM_CHANNEL_ID pOutChannel,
             StromNode*       pInNode,
             STROM_CHANNEL_ID pInChannel) : out_node(pOutNode),
                                            out_channel(pOutChannel),
                                            in_node(pInNode),
                                            in_channel(pInChannel) {}

Cable::~Cable() {}

void Cable::patch() {
    // STROM_LOG("+++ @Cable Node(%02d#%02d) > Node(%02d#%02d)          : %f", out_node->ID(), out_channel, in_node->ID(), in_channel, out_node->out(out_channel));
    in_node->in(in_channel, out_node->out(out_channel));
}
