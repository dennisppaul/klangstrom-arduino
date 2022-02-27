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
 *       [ NODE_ENVELOPE       ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| TRIGGER     TRIGGER |--OUT01
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeEnvelope_hpp
#define NodeEnvelope_hpp

#include <vector>

#include "NodeKernel.hpp"

namespace klang {
    class EnvelopeStage {
    public:
        EnvelopeStage(float pDurationMillis, float pValue) : duration_ms(pDurationMillis), value(pValue) {}
        EnvelopeStage() : duration_ms(0.0), value(0.0) {}
        float duration_ms;
        float value;
    };

    class NodeEnvelope : public Node {
    public:
        static const CHANNEL_ID CH_IN_TRIGGER = 1;
        static const CHANNEL_ID NUM_CH_IN     = 2;

        static const CHANNEL_ID CH_OUT_TRIGGER = 1;
        static const CHANNEL_ID NUM_CH_OUT     = 2;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            } else if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            } else if (pInChannel == CH_IN_TRIGGER) {
                mConnection_CH_IN_TRIGGER = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                mBlock_TRIGGER = AudioBlockPool::NO_ID;
                if (mConnection_CH_IN_TRIGGER != nullptr) {
                    mBlock_TRIGGER = AudioBlockPool::instance().request();
                    mConnection_CH_IN_TRIGGER->update(mBlock_TRIGGER);
                    SIGNAL_TYPE* mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        const float mCurrentSample = mBlockData_TRIGGER[i];
                        mBlockData_TRIGGER[i]      = evaluateEdge(mPreviousSample, mCurrentSample);
                        mPreviousSample            = mCurrentSample;
                    }
                }
                flag_updated();
            }

            if (pChannel == CH_OUT_SIGNAL) {
                const bool   mHasTriggerSignal  = (mBlock_TRIGGER != AudioBlockPool::NO_ID);
                SIGNAL_TYPE* mBlockData_TRIGGER = nullptr;
                if (mHasTriggerSignal) {
                    mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                }
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    if (mHasTriggerSignal) {
                        const SIGNAL_TYPE mTriggerState = mBlockData_TRIGGER[i];
                        if (mTriggerState == RAMP_RISING_EDGE) {
                            start();
                        } else if (mTriggerState == RAMP_FALLING_EDGE) {
                            stop();
                        }
                    }
                    pAudioBlock[i] = kernel(pAudioBlock[i]);
                }
            } else if (pChannel == CH_OUT_TRIGGER) {
                const bool mHasTriggerSignal = (mBlock_TRIGGER != AudioBlockPool::NO_ID);
                if (mHasTriggerSignal) {
                    SIGNAL_TYPE* mBlockData_TRIGGER = AudioBlockPool::instance().data(mBlock_TRIGGER);
                    KLANG_COPY_AUDIO_BUFFER(pAudioBlock, mBlockData_TRIGGER);
                } else {
                    KLANG_FILL_AUDIO_BUFFER(pAudioBlock, 0.0);
                }
            }
        }

        std::vector<EnvelopeStage>& stages() {
            return mEnvelopeStages;
        }

        void add_stage_ms(float pDuration, float pValue) {
            mEnvelopeStages.push_back(EnvelopeStage(pDuration, pValue));
        }

        void add_stage(float pDuration, float pValue) {
            add_stage_ms(pDuration * M_TIME_SCALE, pValue);
        }

        void clear_stages() {
            mEnvelopeStages.clear();
        }

        void start() {
            mEnvelopeDone = false;
            mValue        = mStartValue;
            prepareNextStage(0);
        }

        void stop() {
            mEnvelopeDone = true;
        }

        void set_time_scale(SIGNAL_TYPE pTimeScale) {
            mTimeScale = pTimeScale;
        }

        SIGNAL_TYPE get_time_scale() {
            return mTimeScale;
        }

        void set_value_scale(SIGNAL_TYPE pValueScale) {
            mValueScale = pValueScale;
        }

        SIGNAL_TYPE get_value_scale() {
            return mValueScale;
        }

        void set_start_value(SIGNAL_TYPE pStartValue) {
            mStartValue = pStartValue;
        }

        SIGNAL_TYPE get_start_value() {
            return mStartValue;
        }

        void set_loop(bool pLoop) {
            fLoop = pLoop;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_TIME_SCALE_F32:
                    set_time_scale(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_ADD_STAGE_F32_F32:
                    add_stage(KlangMath::FLOAT_32(pPayLoad, 0), KlangMath::FLOAT_32(pPayLoad, 4));
                    break;
                case KLANG_SET_CLEAR_STAGES:
                    clear_stages();
                    break;
                case KLANG_SET_START:
                    start();
                    break;
                case KLANG_SET_STOP:
                    stop();
                    break;
            }
        }

    private:
        Connection* mConnection_CH_IN_SIGNAL  = nullptr;
        Connection* mConnection_CH_IN_TRIGGER = nullptr;

        AUDIO_BLOCK_ID mBlock_TRIGGER = AudioBlockPool::NO_ID;

        static constexpr SIGNAL_TYPE RAMP_NO_EDGE                     = 0.0;
        static constexpr SIGNAL_TYPE RAMP_RISING_EDGE                 = 1.0;
        static constexpr SIGNAL_TYPE RAMP_FALLING_EDGE                = -1.0;
        static constexpr float       mThreshold                       = 0.0;  // @todo(could be made configurable)
        static constexpr float       M_TIME_SCALE                     = 1000;
        static constexpr float       KLANG_AUDIO_RATE_UINT16_FRAC_INV = M_TIME_SCALE / KLANG_AUDIO_RATE_UINT16;

        std::vector<EnvelopeStage> mEnvelopeStages;

        uint16_t    mEnvStage       = 0;
        float       mStartValue     = 0.0;
        float       mValue          = 0.0;
        float       mDelta          = 0.0;
        float       mTimeScale      = 1.0;
        float       mValueScale     = 1.0;
        float       mStageDuration  = 0.0;
        bool        mEnvelopeDone   = true;
        bool        fLoop           = false;
        SIGNAL_TYPE mPreviousSample = mThreshold;

        inline SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            if (!mEnvelopeDone) {
                if (mEnvStage < mEnvelopeStages.size()) {
                    mValue += mTimeScale * mDelta;
                    mStageDuration += mTimeScale * KLANG_AUDIO_RATE_UINT16_FRAC_INV;
                    if (mStageDuration > mEnvelopeStages[mEnvStage].duration_ms) {
                        mEnvStage++;
                        if (mEnvStage < mEnvelopeStages.size()) {
                            prepareNextStage(mEnvStage);
                        } else {
                            if (fLoop) {
                                start();
                            } else {
                                stop();
                            }
                        }
                    }
                }
            }
            const bool        mHasInputSignal = (mConnection_CH_IN_SIGNAL != nullptr);
            const SIGNAL_TYPE a               = mHasInputSignal ? s * mValue : mValue;
            return a * mValueScale;
        }

        void prepareNextStage(uint16_t pEnvStage) {
            mEnvStage      = pEnvStage;
            mStageDuration = 0.0;
            if (mEnvelopeStages.size() > 0) {
                setDelta(mEnvStage);
            }
        }

        void setDelta(uint16_t pEnvStage) {
            const float mTotalDelta = mEnvelopeStages[pEnvStage].value - mValue;
            mDelta                  = compute_delta_fraction(mTotalDelta, mEnvelopeStages[mEnvStage].duration_ms);
        }

        float compute_delta_fraction(const float pDelta, const float pDuration) {
            if (pDuration > 0) {
                const float a = pDelta * KLANG_AUDIO_RATE_UINT16_FRAC_INV;
                return a / pDuration;
            } else {
                return pDelta;
            }
        }

        const SIGNAL_TYPE evaluateEdge(const SIGNAL_TYPE pPreviousSample, const SIGNAL_TYPE pCurrentSample) {
            if ((pPreviousSample <= mThreshold) && (pCurrentSample > mThreshold)) {
                return RAMP_RISING_EDGE;
            } else if ((pPreviousSample >= mThreshold) && (pCurrentSample < mThreshold)) {
                return RAMP_FALLING_EDGE;
            } else {
                return RAMP_NO_EDGE;
            }
        }
    };
}  // namespace klang

#endif /* NodeEnvelope_hpp */
