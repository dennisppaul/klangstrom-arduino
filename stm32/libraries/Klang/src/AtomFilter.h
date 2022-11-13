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

#ifndef _KLANG_ATOM_FILTER_H_
#define _KLANG_ATOM_FILTER_H_

#include "Klang.h"
#include "Signal.h"

namespace klang {
    class AtomFilter {
    public:

        AtomFilter() {
            set_cutoff(0.5);
            set_resonance(0.5);
        }

        void set_cutoff_Hz(const float pCutoffHz) {
            /* cutoff frequency in Hz [0, sampling_rate/2] */
            const float a = KLANG_AUDIO_RATE_UINT16 * 0.5;
            mCutoff = pCutoffHz / a;
            mCutoff = mCutoff > 1.0f ? 1.0f : (mCutoff < 0.0f ? 0.0f : mCutoff);
        }

        inline void set_cutoff(const float pCutoff) {
            mCutoff = pCutoff > 1.0f ? 1.0f : (pCutoff < 0.0f ? 0.0f : pCutoff);
        }

        [[nodiscard]] float get_cutoff_Hz() const {
            const float a = KLANG_AUDIO_RATE_UINT16 * 0.5;
            return mCutoff * a;
        }

        [[nodiscard]] float get_cutoff() const {
            return mCutoff;
        }

        inline void set_resonance(const float pResonance) {
            mResonance = pResonance > 1.0f ? 1.0f : (pResonance < 0.0f ? 0.0f : pResonance);
        }

        [[nodiscard]] float get_resonance() const {
            return mResonance;
        }

        /* --- process functions --- */

        float process(float pSignal) {
            return mCutoff > 0.0f ? kernel(pSignal) : 0.0f;
        }

        float *process(float *pSignalBlock, uint16_t pNumSamples = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint16_t i = 0; i < pNumSamples; i++) {
                pSignalBlock[i] = process(pSignalBlock[i]);
            }
            return pSignalBlock;
        }

        const Signal &process(const Signal &pSignal) {
            for (uint8_t i = 0; i < pSignal.channels; ++i) {
                const float mSignal = process(pSignal.signal[i]);
                pSignal.signal[i] = mSignal;
            }
            return pSignal;
        }

        const Signal *&process(const Signal *&pSignalBlock, uint16_t pNumSamples = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint16_t i = 0; i < pNumSamples; i++) {
                process(pSignalBlock[i]);
            }
            return pSignalBlock;
        }

    protected:
        float mCutoff = 0.0f;
        float mResonance = 0.0f;

        inline virtual float kernel(float input) = 0;
    };
}  // namespace klang

#endif //_KLANG_ATOM_FILTER_H_
