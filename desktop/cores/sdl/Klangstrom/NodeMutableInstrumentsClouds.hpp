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
 *       [ NODE_CLOUDS         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

// @todo(EVERYTHING!!! ... wave shapes, control signals, parameters, ...)
// @todo(make stereo)
// @todo(buffer inputs)

#ifndef NodeMutableInstrumentsClouds_h
#define NodeMutableInstrumentsClouds_h

#include "KlangNode.hpp"
#include "clouds/dsp/granular_processor.h"
#include "clouds/resources.h"

using namespace clouds;

namespace klang {
    class NodeMutableInstrumentsClouds : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeMutableInstrumentsClouds() {
            KLANG_LOG("@Clouds AUDIO_BLOCK_ITERATIONS: %i", AUDIO_BLOCK_ITERATIONS);

            processor.Init(&large_buffer[0], sizeof(large_buffer),
                           &small_buffer[0], sizeof(small_buffer));

            processor.set_num_channels(2);
            processor.set_low_fidelity(false);
            processor.set_playback_mode(PLAYBACK_MODE_GRANULAR);

            p                = processor.mutable_parameters();
            p->gate          = false;
            p->trigger       = false;
            p->freeze        = false;
            p->position      = 0.0;
            p->size          = 0.5;
            p->pitch         = -7.0;
            p->density       = 0.2;
            p->texture       = 0.5;
            p->feedback      = 0.8;
            p->dry_wet       = 0.5;
            p->reverb        = 0.7;
            p->stereo_spread = 0.4;

            processor.Prepare();
        }

        ~NodeMutableInstrumentsClouds() {
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
                uint8_t mIterations = 0;
                do {
                    const uint8_t mOffset = mIterations * MAX_CLOUDS_AUDIO_BLOCK_SIZE;
                    ShortFrame    input[ACTUAL_AUDIO_BLOCK_SIZE];
                    for (uint16_t i = 0; i < ACTUAL_AUDIO_BLOCK_SIZE; i++) {
                        input[i].l = convert_sample_F32(pAudioBlock[i + mOffset]);
                        input[i].r = convert_sample_F32(pAudioBlock[i + mOffset]);
                    }
                    ShortFrame output[ACTUAL_AUDIO_BLOCK_SIZE];
                    processor.Process(input, output, ACTUAL_AUDIO_BLOCK_SIZE);
                    processor.Prepare();
                    for (uint16_t i = 0; i < ACTUAL_AUDIO_BLOCK_SIZE; i++) {
                        pAudioBlock[i + mOffset] = convert_sample_I16(output[i].l);
                    }
                    mIterations++;
                } while (mIterations < AUDIO_BLOCK_ITERATIONS);
            }
        }

        Parameters* get_parameters() {
            return p;
        }

        void set_playback_mode(PlaybackMode playback_mode) {
            processor.set_playback_mode(playback_mode);
        }

        void set_num_channels(int32_t num_channels) {
            processor.set_num_channels(num_channels);
        }

        void set_low_fidelity(bool low_fidelity) {
            processor.set_low_fidelity(low_fidelity);
        }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        Connection*       mInput_CH_IN_SIGNAL = nullptr;
        GranularProcessor processor;
        Parameters*       p;
        uint8_t           large_buffer[118784];
        uint8_t           small_buffer[65536 - 128];
        const uint8_t     MAX_CLOUDS_AUDIO_BLOCK_SIZE = 32;
        const uint8_t     AUDIO_BLOCK_ITERATIONS      = KLANG_SAMPLES_PER_AUDIO_BLOCK / MAX_CLOUDS_AUDIO_BLOCK_SIZE;
        const uint8_t     ACTUAL_AUDIO_BLOCK_SIZE     = (uint32_t)MAX_CLOUDS_AUDIO_BLOCK_SIZE > (uint32_t)KLANG_SAMPLES_PER_AUDIO_BLOCK ? MAX_CLOUDS_AUDIO_BLOCK_SIZE : (uint8_t)KLANG_SAMPLES_PER_AUDIO_BLOCK;

        inline float convert_sample_I16(const int16_t pRawSample) {
            const static float mScale  = 1.0 / ((1 << 16) - 1);
            const float        mOffset = pRawSample + (1 << 15);
            const float        mRange  = mOffset * mScale;
            return mRange * 2.0 - 1.0;
        }

        inline int16_t convert_sample_F32(const float pRawSample) {
            // @todo(this is a bit rough ... check if it works properly)
            if (pRawSample >= 1.0) {
                return (1 << 15) - 1;
            } else {
                return pRawSample * (1 << 15);
            }
        }
    };
}  // namespace klang

#endif /* NodeMutableInstrumentsClouds_h */
