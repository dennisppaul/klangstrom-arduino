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
 *       [ NODE_DELAY          ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| FEEDBACK            |
 * IN02--| WET                 |
 * IN03--| DURATION            |
 *       |                     |
 *       +---------------------+
 * 
 * use `#define NODE_DELAY_BUFFER_SIZE` to globally define maximum 
 * delay ( line ) length in samples.
 * 
 */
// @TODO(implement `FEEDBACK` + `WET` + `DURATION` as inputs)

#ifndef NodeDelay_hpp
#define NodeDelay_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeDelay : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN = 1;
        //        static const CHANNEL_ID CH_IN_FEEDBACK    = 1;
        //        static const CHANNEL_ID CH_IN_WET         = 2;
        //        static const CHANNEL_ID CH_IN_DURATION    = 3;
        //        static const CHANNEL_ID NUM_CH_IN         = 4;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeDelay() {
            /* initialize pointers positions for delay effect */
            shift    = INIT_DELAY;
            readpos  = delayline;
            writepos = delayline + INIT_DELAY;
            set_duration(mDuration);
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = kernel(pAudioBlock[i]);
                }
            }
        }

        void set_duration(float pDuration) {  // duration in percent of maximum delay. TODO convert this to seconds
            float* pos;
            mDuration = KlangMath::clamp(pDuration, 0.0, 1.0);
            shift     = (uint32_t)lrintf((DELAYLINE_LEN - 1 - MIN_DELAY) * mDuration + MIN_DELAY);
            pos       = writepos - shift;
            if (pos >= delayline) {
                readpos = pos;
            } else {
                readpos = pos + DELAYLINE_LEN - 1;
            }
        }

        void set_wet(float pWet) {  // ratio between original signal and delayed signal
            mWet = KlangMath::clamp(pWet, 0.0, 1.0);
        }

        void set_feedback(float pFeedback) {
            mFeedback = pFeedback;
        }

        void set_filter(float pFilter) {  // coefficient for one pole low-pass filter. 0…1 = no filtering … max filtering
            mFilter = KlangMath::clamp(pFilter, 0.0, 1.0);
        }

        float get_duration() { return mDuration; }
        float get_wet() { return mWet; }
        float get_feedback() { return mFeedback; }
        float get_filter() { return mFilter; }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_DURATION_F32:
                    set_duration(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_WET_F32:
                    set_wet(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_FEEDBACK_F32:
                    set_feedback(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_FILTER_F32:
                    set_filter(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    private:
        Connection* mConnection_CH_IN_SIGNAL = nullptr;
        //        Connection* mConnection_FEEDBACK;
        //        Connection* mConnection_WET;
        //        Connection* mConnection_DURATION;

#ifdef NODE_DELAY_BUFFER_SIZE
        static const uint16_t DELAYLINE_LEN = NODE_DELAY_BUFFER_SIZE;
#else
        static const uint16_t DELAYLINE_LEN = KLANG_AUDIO_RATE_UINT16 / 2;  // max delay in samples
#endif
        static const uint16_t INIT_DELAY = DELAYLINE_LEN / 4;
        static const uint16_t MIN_DELAY  = 250;

        /* Delay effect variables  */
        float    delayline[DELAYLINE_LEN + 2];
        float*   readpos;   // output pointer of delay line
        float*   writepos;  // input pointer of delay line
        uint32_t shift;
        float    mFilter = 0.6;
        float    old_dy;  // previous delayed sample
        float    mFeedback = 0.4;
        float    mWet      = 0.5;
        float    mDuration = 0.25;

        float kernel(float pInput) {
            float y, dy;

            // (*readpos) : delayed sample read at the output of the delay line
            dy     = (1.f - mFilter) * (*readpos) + mFilter * old_dy;  // apply lowpass filter in the loop
            old_dy = dy;
            y      = pInput + mFeedback * dy;

            y         = KlangMath::clamp(y, -1.0, 1.0);
            *writepos = y;  // write new computed sample at the input of the delay line

            /* update the delay line pointers : */
            writepos++;
            readpos++;

            if ((writepos - delayline) >= DELAYLINE_LEN) {
                writepos = delayline;  // wrap pointer
            }
            if ((readpos - delayline) >= DELAYLINE_LEN) {
                readpos = delayline;  // wrap pointer
            }
            return (mWet * y + (1 - mWet) * pInput);
        }
    };
}  // namespace klang

#endif /* NodeDelay_hpp */
