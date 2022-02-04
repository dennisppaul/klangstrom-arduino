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
 * @brief `Node` is the base class for all nodes in the `Klang` library. it provides the basic
 * functionality for all nodes. appart from the required interface methods there are a few
 * conventions:
 *
 * - methods to modify fields should be implemented as `set_` and `get_` or `add_` ( e.g
 *   `void set_gain(SIGNAL_TYPE)`, `SIGNAL_TYPE get_gain()` and `void add_gain(SIGNAL_TYPE)` )
 * - public methods should be all lowercase letters, words separated by underscore and names start
 *   with a verb ( e.g `void set_gain(SIGNAL_TYPE)` )
 * - `CH_IN_SIGNAL` + `CH_OUT_SIGNAL` should be defined if not `0`
 * - channel IDs should be defined as public static constants with `CH_IN_` and `CH_OUT_`
 *   ( e.g `CH_IN_GAIN` and `CH_OUT_SIGNAL_LEFT` )
 * - node classes should be preface with an ASCII drawing of inputs and outputs following the
 *   naming convention shown in the example below.
 *   \/\*\*
 *    \*       [ NODE_VCO_WAVETABLE  ]
 *    \*       +---------------------+
 *    \*       |                     |
 *    \* IN00--| FREQ         SIGNAL |--OUT00
 *    \* IN01--| AMP                 |
 *    \*       |                     |
 *    \*       +---------------------+
 *    \*
 *    \*       @description(
 *    \*          the wavetable node produce a periodic signal from a chunk of memory ( wavetable ).
 *    \*          the speed at which the signal is produced from the wavetable can be adjusted by the
 *    \*          changing its frequency `FREQ` and the amplitude by changing `AMP`. the node is often
 *    \*          used as an *oscillator* to reproduce a sinewave, triangle, square or sawtooth shaped
 *    \*          signal at an audible frequency ( Voltage-Controlled Oscillator (`VCO`) ). however, a
 *    \*          wavetable node can also be used to manipulate other parameters ( e.g the frequency
 *    \*          of another wavetable node ) at slower speeds ( Low Frequency Oscillator (`LFO`) ).
 *    \*          note that the maximum size of a wavetable is 65536 samples.
 *    \*       )
 *    \*
 *    \*       @set_value(FREQUENCY_F32,AMPLITUDE_F32,OFFSET_F32,WAVEFORM)
 *    \*\/
 * - the `set_command` should be implemented for all fields
 */

#ifndef KlangNode_hpp
#define KlangNode_hpp

#include "AudioBlockPool.hpp"
#include "Connection.hpp"
#include "Klang.hpp"
#include "KlangCommands.hpp"

namespace klang {
    class Connection;
    class Communincation;
    class Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL  = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL = 0;

        Node();
        virtual ~Node() {}

        NODE_ID ID() {
            return mID;
        }

        bool is_not_updated() {
            return Klang::instance().frame_index() != mFrame;
        }

        void flag_updated() {
            mFrame = Klang::instance().frame_index();
        }

        // virtual CHANNEL_ID get_num_input_channels() = 0;
        // virtual CHANNEL_ID get_num_output_channels() = 0;
        virtual void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock)          = 0;
        virtual bool connect(Connection* pConnection, CHANNEL_ID pInChannel)        = 0;
        virtual bool disconnect(CHANNEL_ID pInChannel)                              = 0;
        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) = 0;
        virtual bool ouput_allow_multiple_connections() { return false; }

        static void reset();

    private:
        FRAME_TYPE     mFrame = -1;
        NODE_ID        mID;
        static NODE_ID oID;
    };
}  // namespace klang

#endif /* KlangNode_hpp */
