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
 *       [ NODE_BRAIDS         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

// @todo(EVERYTHING!!! ... wave shapes, control signals, parameters, ...)

#ifndef NodeMutableInstrumentsBraids_h
#define NodeMutableInstrumentsBraids_h

#include "KlangNode.hpp"
#include "braids/macro_oscillator.h"

using namespace braids;

namespace klang {
    class NodeMutableInstrumentsBraids : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeMutableInstrumentsBraids() {
            osc.Init();
            osc.set_shape(MACRO_OSC_SHAPE_VOWEL_FOF);
            set_note(48);
        }

        ~NodeMutableInstrumentsBraids() {
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
                int16_t buffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];
                uint8_t sync_buffer[KLANG_SAMPLES_PER_AUDIO_BLOCK];
                // osc.set_parameters(tri, 0);
                memset(sync_buffer, 0, sizeof(sync_buffer));
                osc.Render(sync_buffer, buffer, KLANG_SAMPLES_PER_AUDIO_BLOCK);
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = convert_sample_I16(buffer[i]);
                }
            }
        }

        void set_note(uint8_t pNote) {
            // @note(RTFM what with the shift?)
            osc.set_pitch(pNote << 7);
        }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        Connection*     mInput_CH_IN_SIGNAL = nullptr;
        MacroOscillator osc;

        inline float convert_sample_I16(const int16_t pRawSample) {
            const static float mScale  = 1.0 / ((1 << 16) - 1);
            const float        mOffset = pRawSample + (1 << 15);
            const float        mRange  = mOffset * mScale;
            return mRange * 2.0 - 1.0;
        }
    };
}  // namespace klang

#endif /* NodeMutableInstrumentsBraids_h */
