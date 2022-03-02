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
 *       [ NODE_VOCODER        ]
 *       +---------------------+
 *       |                     |
 * IN00--| CARRIER      SIGNAL |--OUT00
 * IN01--| MODULATOR           |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeVocoder_h
#define NodeVocoder_h

#include "KlangNode.hpp"
#include "voclib.h"

namespace klang {
    class NodeVocoder : public Node {
    public:
        static const CHANNEL_ID CH_IN_CARRIER   = 0;
        static const CHANNEL_ID CH_IN_MODULATOR = 1;
        static const CHANNEL_ID NUM_CH_IN       = 2;
        static const CHANNEL_ID NUM_CH_OUT      = 1;

        NodeVocoder(const unsigned char bands            = 24,
                    const unsigned char filters_per_band = 4,
                    const uint16_t      pBufferSize      = 256) {
            mCarrierBuffer      = new float*[2];
            mCarrierBuffer[0]   = new float[pBufferSize];
            mCarrierBuffer[1]   = new float[pBufferSize];
            mModulatorBuffer    = new float*[2];
            mModulatorBuffer[0] = new float[pBufferSize];
            mModulatorBuffer[1] = new float[pBufferSize];
            mCarrierFrames      = pBufferSize;
            mModulatorFrames    = pBufferSize;
            mBufferSize         = pBufferSize;

            bool mResult = voclib_initialize(&mVocoder,
                                             bands,
                                             filters_per_band,
                                             carrier_sample_rate,
                                             carrier_channels);
            if (!mResult) {
                KLANG_LOG_ERR("+++ vocoder failed to `initialize`");
            }
            set_reaction_time(mSignal_REACTION_TIME);
            set_formant_shift(mSignal_FORMANT_SHIFT);
        }

        ~NodeVocoder() {
            free(mCarrierBuffer[0]);
            free(mCarrierBuffer[1]);
            free(mCarrierBuffer);
            free(mModulatorBuffer[0]);
            free(mModulatorBuffer[1]);
            free(mModulatorBuffer);
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_CARRIER) {
                mInput_CH_IN_CARRIER = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_MODULATOR) {
                mInput_CH_IN_MODULATOR = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_CARRIER) {
                mInput_CH_IN_CARRIER = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_MODULATOR) {
                mInput_CH_IN_MODULATOR = nullptr;
                return true;
            }
            return false;
        }

        /**
         * reaction time to volume change in modulator in milliseconds
         * default : 0.03
         * min     : 0.002
         * max     : 2.0
         */
        void set_reaction_time(float pReactionTime) {
            mSignal_REACTION_TIME = pReactionTime;
            bool mResult          = voclib_set_reaction_time(&mVocoder, mSignal_REACTION_TIME);
            if (!mResult) {
                KLANG_LOG_ERR("+++ vocoder failed to `set_reaction_time`: %f", pReactionTime);
            }
        }

        float get_reaction_time() { return mSignal_REACTION_TIME; }

        /**
         * formant shifting changes the size of the speaker's head.
         * default : 1.00
         * min     : 0.25
         * max     : 4.0
         */
        void set_formant_shift(float pFormantShift) {
            mSignal_FORMANT_SHIFT = pFormantShift;
            bool mResult          = voclib_set_formant_shift(&mVocoder, mSignal_FORMANT_SHIFT);
            if (!mResult) {
                KLANG_LOG_ERR("+++ vocoder failed to `set_formant_shift`: %f\n", pFormantShift);
            }
        }

        float get_formant_shift() { return mSignal_FORMANT_SHIFT; }

        void set_output_rescale(float pRescaleOutput) {
            mRescaleOutput = pRescaleOutput;
        }

        float get_output_rescale() { return mRescaleOutput; }

        void reset() {
            voclib_reset_history(&mVocoder);
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                mBlock_CARRIER = AudioBlockPool::NO_ID;
                if (mInput_CH_IN_CARRIER != nullptr) {
                    mBlock_CARRIER = AudioBlockPool::instance().request();
                    mInput_CH_IN_CARRIER->output_node.update(mInput_CH_IN_CARRIER->output_channel_id,
                                                             AudioBlockPool::instance().data(mBlock_CARRIER));
                }
                mBlock_MODULATOR = AudioBlockPool::NO_ID;
                if (mInput_CH_IN_MODULATOR != nullptr) {
                    mBlock_MODULATOR = AudioBlockPool::instance().request();
                    mInput_CH_IN_MODULATOR->output_node.update(mInput_CH_IN_MODULATOR->output_channel_id,
                                                               AudioBlockPool::instance().data(mBlock_MODULATOR));
                }
                flag_updated();
            }

            if (pChannel == CH_OUT_SIGNAL) {
                SIGNAL_TYPE* mBlockData_CARRIER;
                if (mBlock_CARRIER != AudioBlockPool::NO_ID) {
                    mBlockData_CARRIER = AudioBlockPool::instance().data(mBlock_CARRIER);
                } else {
                    mBlockData_CARRIER = nullptr;
                }

                SIGNAL_TYPE* mBlockData_MODULATOR;
                if (mBlock_MODULATOR != AudioBlockPool::NO_ID) {
                    mBlockData_MODULATOR = AudioBlockPool::instance().data(mBlock_MODULATOR);
                } else {
                    mBlockData_MODULATOR = nullptr;
                }

                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    if (mBlock_CARRIER != AudioBlockPool::NO_ID && mBlock_MODULATOR != AudioBlockPool::NO_ID) {
                        // @TODO("this is just a stupid hack. voclib uses blocks … so does klang")
                        pAudioBlock[i] = kernel(mBlockData_CARRIER[i], mBlockData_MODULATOR[i]);
                    }
                }
            }

            if (mBlock_MODULATOR != AudioBlockPool::NO_ID) {
                AudioBlockPool::instance().release(mBlock_MODULATOR);
            }

            if (mBlock_CARRIER != AudioBlockPool::NO_ID) {
                AudioBlockPool::instance().release(mBlock_CARRIER);
            }
        }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        AUDIO_BLOCK_ID mBlock_CARRIER   = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_MODULATOR = AudioBlockPool::NO_ID;

        Connection* mInput_CH_IN_CARRIER   = nullptr;
        Connection* mInput_CH_IN_MODULATOR = nullptr;  // "… usually the voice."

        SIGNAL_TYPE mSignal_REACTION_TIME = 0.04;
        SIGNAL_TYPE mSignal_FORMANT_SHIFT = 1.0;

        voclib_instance mVocoder;

        float mRescaleOutput = 16.0;

        static const unsigned char carrier_channels      = 1;
        static const unsigned int  carrier_sample_rate   = KLANG_AUDIO_RATE_UINT16;
        static const unsigned char modulator_channels    = 1;
        static const unsigned int  modulator_sample_rate = KLANG_AUDIO_RATE_UINT16;

        unsigned int mCarrierFrames;
        unsigned int mModulatorFrames;
        uint16_t     mBufferSize;

        float**  mCarrierBuffer;
        float**  mModulatorBuffer;
        uint16_t mBufferCounter = 0;
        bool     mBufferSelect  = false;

        inline float kernel(float mSignal_CARRIER, float mSignal_MODULATOR) {
            // @TODO("apparently this also works with a float buffer with just a single entry ( aka no array")
            // @TODO("do performance test if it is worth allocating all this memory")
            mCarrierBuffer[mBufferSelect][mBufferCounter]   = mSignal_CARRIER;  //mOsc.out(Node::CH_OUT_SIGNAL);
            mModulatorBuffer[mBufferSelect][mBufferCounter] = mSignal_MODULATOR;
            float mSignal_OUT                               = mCarrierBuffer[!mBufferSelect][mBufferCounter] * mRescaleOutput;

            mBufferCounter++;
            if (mBufferCounter >= mBufferSize) {
                process();
                mBufferCounter = 0;
                mBufferSelect  = !mBufferSelect;
            }

            return mSignal_OUT;
        }

        inline void process() {
            bool mResult = voclib_process(&mVocoder,
                                          mCarrierBuffer[mBufferSelect],
                                          mModulatorBuffer[mBufferSelect],
                                          mCarrierBuffer[mBufferSelect],
                                          mCarrierFrames);
            if (!mResult) {
                KLANG_LOG_ERR("+++ vocoder failed to `process`");
            }
        }
    };
}  // namespace klang

#endif /* NodeVocoder_h */
