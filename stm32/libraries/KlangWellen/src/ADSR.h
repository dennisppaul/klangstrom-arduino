/*
 * KlangWellen
 *
 * This file is part of the *KlangWellen* library (https://github.com/dennisppaul/klangwellen).
 * Copyright (c) 2023 Dennis P Paul
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
 * PROCESSOR INTERFACE
 *
 * - [x] float process()
 * - [x] float process(float)‌
 * - [x] void process(Signal&)
 * - [x] void process(float*, uint32_t)
 * - [x] void process(float*, float*, uint32_t)
 */

#pragma once

#include "KlangWellen.h"
#include "Signal.h"

namespace klangwellen {
    class ADSR {
        /*
         *       @description(
         *
         *          |----->|-->|   |-->|
         *          |---A--|-D-|-S-|-R-|
         *          |      /\
         *          |     /  \
         *          |    /    \_____
         *          |   /        ^  \
         *          |  /         |   \
         *          | /          |    \
         *          |/___________|_____\
         *          |
         *          |Press          |Release
         *       )
         *
         */
    public:
        ADSR(uint32_t pSamplingRate) : mSamplingRate(pSamplingRate) {
            mAmp                   = 0.0f;
            mAttack                = KlangWellen::DEFAULT_ATTACK;
            mDecay                 = KlangWellen::DEFAULT_DECAY;
            mDelta                 = 0.0f;
            mRelease               = KlangWellen::DEFAULT_RELEASE;
            mSustain               = KlangWellen::DEFAULT_SUSTAIN;
            FADE_TO_ZERO_RATE_SEC  = 0.01f;
            USE_FADE_TO_ZERO_STATE = false;
            mState                 = ENVELOPE_STATE::IDLE;
            setState(ENVELOPE_STATE::IDLE);
        }

        ADSR() : ADSR(KlangWellen::DEFAULT_SAMPLING_RATE) {}

        float process() {
            step();
            return mAmp;
        }

        float process(float signal) {
            step();
            return signal * mAmp;
        }

        void process(Signal& signal) {
            step();
            signal.left *= mAmp;
            signal.right *= mAmp;
        }

        void process(float*         signal_buffer_left,
                     float*         signal_buffer_right,
                     const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < buffer_length; i++) {
                step();
                signal_buffer_left[i] *= mAmp;
                signal_buffer_right[i] *= mAmp;
            }
        }

        void process(float* signal_buffer, const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint32_t i = 0; i < buffer_length; i++) {
                step();
                signal_buffer[i] *= mAmp;
            }
        }

        void start() {
            check_scheduled_attack_state();
        }

        void stop() {
            check_scheduled_release_state();
        }

        float get_attack() {
            return mAttack;
        }

        void set_attack(float pAttack) {
            mAttack = pAttack;
        }

        void set_adsr(float pAttack, float pDecay, float pSustain, float pRelease) {
            set_attack(pAttack);
            set_decay(pDecay);
            set_sustain(pSustain);
            set_release(pRelease);
        }

        float get_decay() {
            return mDecay;
        }

        void set_decay(float pDecay) {
            mDecay = pDecay;
        }

        float get_sustain() {
            return mSustain;
        }

        void set_sustain(float pSustain) {
            mSustain = pSustain;
        }

        float get_release() {
            return mRelease;
        }

        void set_release(float pRelease) {
            mRelease = pRelease;
        }

    private:
        enum class ENVELOPE_STATE {
            IDLE,
            ATTACK,
            DECAY,
            SUSTAIN,
            RELEASE,
            PRE_ATTACK_FADE_TO_ZERO
        };
        float          FADE_TO_ZERO_RATE_SEC;
        bool           USE_FADE_TO_ZERO_STATE;
        float          mAmp;
        float          mAttack;
        float          mDecay;
        float          mDelta;
        float          mRelease;
        uint32_t       mSamplingRate;
        ENVELOPE_STATE mState;
        float          mSustain;

        void check_scheduled_attack_state() {
            if (mAmp > 0.0f) {
                if (USE_FADE_TO_ZERO_STATE) {
                    if (mState != ENVELOPE_STATE::PRE_ATTACK_FADE_TO_ZERO) {
                        mDelta = compute_delta_fraction(-mAmp, FADE_TO_ZERO_RATE_SEC);
                        setState(ENVELOPE_STATE::PRE_ATTACK_FADE_TO_ZERO);
                    }
                } else {
                    mDelta = compute_delta_fraction(1.0f, mAttack);
                    setState(ENVELOPE_STATE::ATTACK);
                }
            } else {
                mDelta = compute_delta_fraction(1.0f, mAttack);
                setState(ENVELOPE_STATE::ATTACK);
            }
        }

        void check_scheduled_release_state() {
            if (mState != ENVELOPE_STATE::RELEASE) {
                mDelta = compute_delta_fraction(-mAmp, mRelease);
                setState(ENVELOPE_STATE::RELEASE);
            }
        }

        float compute_delta_fraction(float pDelta, float pDuration) {
            return pDuration > 0 ? (pDelta / mSamplingRate) / pDuration : pDelta;
        }

        void setState(ENVELOPE_STATE pState) {
            mState = pState;
        }

        void step() {
            switch (mState) {
                case ENVELOPE_STATE::IDLE:
                case ENVELOPE_STATE::SUSTAIN:
                    break;
                case ENVELOPE_STATE::ATTACK:
                    // increase amp to sustain_level in ATTACK sec
                    mAmp += mDelta;
                    if (mAmp >= 1.0f) {
                        mAmp   = 1.0f;
                        mDelta = compute_delta_fraction(-(1.0f - mSustain), mDecay);
                        setState(ENVELOPE_STATE::DECAY);
                    }
                    break;
                case ENVELOPE_STATE::DECAY:
                    // decrease amp to sustain_level in DECAY sec
                    mAmp += mDelta;
                    if (mAmp <= mSustain) {
                        mAmp = mSustain;
                        setState(ENVELOPE_STATE::SUSTAIN);
                    }
                    break;
                case ENVELOPE_STATE::RELEASE:
                    // decrease amp to 0.0 in RELEASE sec
                    mAmp += mDelta;
                    if (mAmp <= 0.0f) {
                        mAmp = 0.0f;
                        setState(ENVELOPE_STATE::IDLE);
                    }
                    break;
                case ENVELOPE_STATE::PRE_ATTACK_FADE_TO_ZERO:
                    mAmp += mDelta;
                    if (mAmp <= 0.0f) {
                        mAmp   = 0.0f;
                        mDelta = compute_delta_fraction(1.0f, mAttack);
                        setState(ENVELOPE_STATE::ATTACK);
                    }
                    break;
            }
        }
    };
}  // namespace klangwellen
