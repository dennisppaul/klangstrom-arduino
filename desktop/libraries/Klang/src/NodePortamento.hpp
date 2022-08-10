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
 *       [ NODE_PORTAMENTO     ]
 *       +---------------------+
 *       |                     |
 * IN00--| VALUE         VALUE |--OUT00
 * IN05--| SPEED               |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodePortamento_hpp
#define NodePortamento_hpp

#include <cmath>

#include "NodeKernel.hpp"

#ifndef EPSILON
#define EPSILON 0.1
#endif

namespace klang {
    class NodePortamento : public Node {
    public:
        static const CHANNEL_ID CH_IN_VALUE = 0;
        static const CHANNEL_ID CH_IN_SPEED = 1;
        static const CHANNEL_ID NUM_CH_IN   = 2;

        static const CHANNEL_ID CH_OUT_VALUE = 0;
        static const CHANNEL_ID NUM_CH_OUT   = 1;

        NodePortamento() {
            set_speed(10);
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_VALUE) {
                mConnection_CH_IN_VALUE = pConnection;
                return true;
            }
            if (pInChannel == CH_IN_SPEED) {
                mConnection_CH_IN_SPEED = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_VALUE) {
                mConnection_CH_IN_VALUE = nullptr;
                return true;
            }
            if (pInChannel == CH_IN_SPEED) {
                mConnection_CH_IN_SPEED = nullptr;
                return true;
            }
            return false;
        }

        void set_value(float pValue) {
            if (pValue != mValueCurrent) {
                mValueDesired = pValue;
                mDirty        = true;
            }
        }

        void set_speed(float pSpeed) {
            mSpeed = pSpeed / KLANG_AUDIO_RATE_UINT16;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                mBlock_VALUE = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_VALUE != nullptr) {
                    mBlock_VALUE = AudioBlockPool::instance().request();
                    mConnection_CH_IN_VALUE->update(mBlock_VALUE);
                }
                mBlock_SPEED = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_SPEED != nullptr) {
                    mBlock_SPEED = AudioBlockPool::instance().request();
                    mConnection_CH_IN_SPEED->update(mBlock_SPEED);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_VALUE) {
                SIGNAL_TYPE* mBlockData_FREQ;
                if (mBlock_VALUE != AudioBlockPool::NO_ID) {
                    mBlockData_FREQ = AudioBlockPool::instance().data(mBlock_VALUE);
                } else {
                    mBlockData_FREQ = NULL;
                }
                SIGNAL_TYPE* mBlockData_SPEED;
                if (mBlock_SPEED != AudioBlockPool::NO_ID) {
                    mBlockData_SPEED = AudioBlockPool::instance().data(mBlock_SPEED);
                } else {
                    mBlockData_SPEED = NULL;
                }

                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    if (mBlock_VALUE != AudioBlockPool::NO_ID) {
                        set_value(mBlockData_FREQ[i]);
                    }
                    if (mBlock_SPEED != AudioBlockPool::NO_ID) {
                        set_speed(mBlockData_SPEED[i]);
                    }
                    if (mDirty) {
                        const float mDiff = mValueDesired - mValueCurrent;
                        if (abs(mDiff) > EPSILON) {
                            const float a = mDiff * mSpeed;
                            mValueCurrent += a;
                        } else {
                            mValueCurrent = mValueDesired;
                            mDirty        = false;
                        }
                    }
                    pAudioBlock[i] = mValueCurrent;
                }
            }
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_VALUE_F32:
                    set_value(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_SPEED_F32:
                    set_speed(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    private:
        AUDIO_BLOCK_ID mBlock_VALUE = AudioBlockPool::NO_ID;
        AUDIO_BLOCK_ID mBlock_SPEED = AudioBlockPool::NO_ID;

        Connection* mConnection_CH_IN_VALUE = nullptr;
        Connection* mConnection_CH_IN_SPEED = nullptr;

        bool        mDirty        = false;
        SIGNAL_TYPE mSpeed        = 0.0;
        SIGNAL_TYPE mValueCurrent = 0.0;
        SIGNAL_TYPE mValueDesired = 0.0;
    };
}  // namespace klang

#endif /* NodePortamento_hpp */
