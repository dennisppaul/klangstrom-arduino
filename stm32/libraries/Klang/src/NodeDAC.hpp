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
 *       [ NODE_DAC            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL(_LEFT)       |-> PHYSICAL
 * IN01--| SIGNAL_RIGHT        |-> OUTPUT
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *           the DAC ((D)igital (A)nalog (C)onverter) node represents the connection to the
 *           underlying hardware. oftentimes it is the last node in a synthesizer patch. it
 *           accepts mono or stereo inputs.
 *       )
 */

#ifndef NodeDAC_hpp
#define NodeDAC_hpp

#include <algorithm>

#include "KlangNode.hpp"

namespace klang {
    class NodeDAC : public Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL       = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_LEFT  = 0;
        static const CHANNEL_ID CH_IN_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_IN          = 2;

        static const CHANNEL_ID CH_OUT_SIGNAL_LEFT  = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_OUT          = 2;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_LEFT) {
                mConnection_CH_IN_LEFT = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_RIGHT) {
                mConnection_CH_IN_RIGHT = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL_LEFT) {
                mConnection_CH_IN_LEFT = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_SIGNAL_RIGHT) {
                mConnection_CH_IN_RIGHT = nullptr;
                return true;
            }
            return false;
        }

        void process_frame(SIGNAL_TYPE* mLeftBlock, SIGNAL_TYPE* mRightBlock) {
            //@TODO("move mono/stereo switch to method (+set_value)")
            Klang::instance().frame_begin();
            if (!Klang::instance().islocked() || (mConnection_CH_IN_LEFT == nullptr && mConnection_CH_IN_RIGHT == nullptr)) {
                if (mConnection_CH_IN_LEFT != nullptr) {
                    mConnection_CH_IN_LEFT->update(mLeftBlock);
                }
                if (mStereo) {
                    if (mConnection_CH_IN_RIGHT != nullptr) {
                        mConnection_CH_IN_RIGHT->update(mRightBlock);
                    }
                } else {
                    std::copy(mLeftBlock, mLeftBlock + KLANG_SAMPLES_PER_AUDIO_BLOCK, mRightBlock);
                }
            } else {
                memset(mLeftBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
                memset(mRightBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
#ifdef DEBUG_DAC
                KLANG_LOG("@NodeDAC synthesizer is locked");
#endif
            }
            Klang::instance().frame_end();
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            /* the method `process_frame` is preferred over this method as it writes directly into the incoming buffers */
            if (is_not_updated()) {
                mBlock_LEFT = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_LEFT != nullptr) {
                    mBlock_LEFT = AudioBlockPool::instance().request();
                    mConnection_CH_IN_LEFT->update(mBlock_LEFT);
                }
                mBlock_RIGHT = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_RIGHT != nullptr) {
                    mBlock_RIGHT = AudioBlockPool::instance().request();
                    mConnection_CH_IN_RIGHT->update(mBlock_RIGHT);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL_LEFT) {
                if (mBlock_LEFT != AudioBlockPool::NO_ID) {
                    SIGNAL_TYPE* mBlockData_LEFT = AudioBlockPool::instance().data(mBlock_LEFT);
                    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        pAudioBlock[i] = mBlockData_LEFT[i];
                    }
                }
            }
            if (pChannel == CH_OUT_SIGNAL_RIGHT) {
                // @TODO("could copy left channel to right channel if right is not connected. similar to option in `process_frame`. maybe there is a global option to have DAC automatically copy mono to stereo")
                if (mBlock_RIGHT != AudioBlockPool::NO_ID) {
                    SIGNAL_TYPE* mBlockData_RIGHT = AudioBlockPool::instance().data(mBlock_RIGHT);
                    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        pAudioBlock[i] = mBlockData_RIGHT[i];
                    }
                }
            }
        }

        void set_stereo(bool pStereo) {
            mStereo = pStereo;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_STEREO_I8:
                    set_stereo(pPayLoad[0]);
                    break;
            }
        }

    private:
        AUDIO_BLOCK_ID mBlock_LEFT  = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_RIGHT = AudioBlockPool::NO_ID;

        Connection* mConnection_CH_IN_LEFT  = nullptr;
        Connection* mConnection_CH_IN_RIGHT = nullptr;

        bool mStereo = false;
    };
}  // namespace klang

#endif /* NodeDAC_hpp */
