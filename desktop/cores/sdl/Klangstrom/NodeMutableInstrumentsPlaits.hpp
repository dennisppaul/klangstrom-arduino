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
 *       [ NODE_PLAITS         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

// @todo(EVERYTHING!!! ... wave shapes, control signals, parameters, ...)

#ifndef NodeMutableInstrumentsPlaits_h
#define NodeMutableInstrumentsPlaits_h

#include "KlangNode.hpp"
#include "plaits/dsp/oscillator/oscillator.h"

using namespace plaits;

namespace klang {
    class NodeMutableInstrumentsPlaits : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeMutableInstrumentsPlaits() {
            osc.Init();
            fPitch = 110.0;
        }

        ~NodeMutableInstrumentsPlaits() {
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mInput_CH_IN_SIGNAL = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mInput_CH_IN_SIGNAL = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mInput_CH_IN_SIGNAL != nullptr) {
                    mInput_CH_IN_SIGNAL->output_node.update(mInput_CH_IN_SIGNAL->output_channel_id, pAudioBlock);
                }
                flag_updated();
            }

            if (pChannel == CH_OUT_SIGNAL) {
                const float f        = fPitch / KLANG_AUDIO_RATE;
                const float mControl = 0.0;
                osc.Render<OSCILLATOR_SHAPE_SLOPE>(f, mControl, pAudioBlock, KLANG_SAMPLES_PER_AUDIO_BLOCK);
            }
        }

        void set_pitch(float pPitch) {
            fPitch = pPitch;
        }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        Connection* mInput_CH_IN_SIGNAL = nullptr;
        Oscillator  osc;
        float       fPitch;
    };
}  // namespace klang

#endif /* NodeMutableInstrumentsPlaits_h */
