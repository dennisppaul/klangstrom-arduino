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
 *       [ NODE_COMPRESSOR     ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

// @TODO(RRCompressor, sort out mono/stereo processing … it s a mess)
// @TODO(RRCompressor, give names to paramters)
// @TODO(expose paramaters through methods)
// @TODO(implement `set_command`)

#ifndef NodeCompressor_hpp
#define NodeCompressor_hpp

#include "KlangNode.hpp"
#include "RRCompressor.h"

namespace klang {

    class NodeCompressor : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        static const uint8_t PRESET_2_TO_1            = 0;
        static const uint8_t PRESET_4_TO_1            = 1;
        static const uint8_t PRESET_8_TO_1            = 2;
        static const uint8_t PRESET_FINAL_LIMITER     = 3;
        static const uint8_t PRESET_HARMONIC_ENHANCER = 4;
        static const uint8_t PRESET_BAND_COMP_BAND    = 5;
        static const uint8_t PRESET_END_COMP_BAND     = 6;
        static const uint8_t NUM_PRESETS              = 7;
        static const uint8_t PARAM_THRESHOLD          = 0;
        static const uint8_t PARAM_RATIO              = 1;
        static const uint8_t PARAM_OUTPUT             = 2;
        static const uint8_t PARAM_ATT                = 3;
        static const uint8_t PARAM_REL                = 4;
        static const uint8_t PARAM_A_OUT              = 5;
        static const uint8_t PARAM_KNEE               = 6;
        static const uint8_t PARAM_STEREO             = 7;
        static const uint8_t PARAM_PEAK               = 8;
        static const uint8_t NUM_PARAMS               = 9;

        NodeCompressor() {
            mCompressor.Compressor_Change_Preset(RRCompressor::PRESET_2_TO_1);
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mInput_SIGNAL = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mInput_SIGNAL = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mInput_SIGNAL != nullptr) {
                    mInput_SIGNAL->output_node.update(mInput_SIGNAL->output_channel_id, pAudioBlock);
                }
                flag_updated();
            }

            if (pChannel == CH_OUT_SIGNAL) {
                float mSignalRight[KLANG_SAMPLES_PER_AUDIO_BLOCK];
                KLANG_COPY_AUDIO_BUFFER(mSignalRight, pAudioBlock);
                mCompressor.out(pAudioBlock, mSignalRight);
            }
        }

        void set_preset(uint8_t pPreset) {
            if (pPreset < NUM_PRESETS) {
                mCompressor.Compressor_Change_Preset(pPreset);
            }
        }

        void set_parameter(uint8_t pParameter, int value) {
            if (pParameter < NUM_PARAMS) {
                mCompressor.Compressor_Change(pParameter, value);
            }
        }

        int get_parameter(uint8_t pParameter) {
            if (pParameter < NUM_PARAMS) {
                return mCompressor.getpar(pParameter);
            }
            return 0;
        }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
        }

    private:
        Connection*  mInput_SIGNAL = nullptr;
        RRCompressor mCompressor;
    };
}  // namespace klang

#endif /* NodeCompressor_hpp */
