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

#ifndef _KLANG_ATOM_OSCILLATOR_FUNCTION_H_
#define _KLANG_ATOM_OSCILLATOR_FUNCTION_H_

#include "KlangMath.hpp"
#include "Signal.h"

// @TODO(see if it is a good or a bad idea to derive all atoms classes from base class.
//       while it seem structurally tempting to do so, this might impact performance quite a
//       lot as all process functions need to be virtual ( i.e a location read from vtable ).
//       the only real functional benefit i can see from using an atom base class would be
//       that atoms can be collected and processed in groups. this however, might not even
//       be a likely scenario for klang applications. hmmmm ... )

//#define KLANG_USE_ATOM_BASE_CLASS
#ifdef KLANG_USE_ATOM_BASE_CLASS

#include "Atom.h"

#define KLANG_OVERRIDE_ATOM_BASE_CLASS override
#else
#define KLANG_OVERRIDE_ATOM_BASE_CLASS
#endif

namespace klang {
    class AtomOscillatorFunction
#ifdef KLANG_USE_ATOM_BASE_CLASS
        : public Atom
#endif
    {
    public:
        enum WAVEFORM {
            SINE = 0,
            TRIANGLE,
            SAWTOOTH,
            SQUARE
            // @TODO("add `EXPONENT`")
        };

        AtomOscillatorFunction() {
            set_frequency(OSC_DEFAULT_FREQUENCY);
            set_amplitude(OSC_DEFAULT_AMPLITUDE);
        }

        void set_waveform(WAVEFORM pWaveform) { mWaveform = pWaveform; }

        void set_amplitude(float pAmplitude) {
            mAmplitude = pAmplitude;
        }

        [[nodiscard]] float get_amplitude() const {
            return mAmplitude;
        }

        void set_offset(float pOffset) {
            mOffset = pOffset;
        }

        [[nodiscard]] float get_offset() const {
            return mOffset;
        }

        /**
         * set phase of oscillator
         * @param pPhase range from 0..1 where 0 is the beginning of a phase and 1 is the end of a full phase
         */
        void set_phase(float pPhase) {
            switch (mWaveform) {
                case SINE:
                    mPhase = pPhase * TWO_PI;
                    break;
                case TRIANGLE:
                case SAWTOOTH:
                case SQUARE:
                    mPhase = pPhase * KLANG_AUDIO_RATE;
            }
        }

        void set_frequency(float pFrequency) {
            if (mFrequency != pFrequency) {
                mFrequency = pFrequency;
                mStepSize = mFrequency * TWO_PI / KLANG_AUDIO_RATE_UINT16;
            }
        }

        [[nodiscard]] float get_frequency() const { return mFrequency; }

        /* --- process functions --- */

#ifdef KLANG_USE_ATOM_BASE_CLASS
        using Atom::process;
#else

        inline float process() {
            return process(0.0f);
        }

        inline float *process(float *pSignalBlock) {
            return process(pSignalBlock, KLANG_SAMPLES_PER_AUDIO_BLOCK);
        }

        inline const Signal *&process(const Signal *&pSignalBlock) {
            return process(pSignalBlock, KLANG_SAMPLES_PER_AUDIO_BLOCK);
        }

#endif

        float process(float pSignal) KLANG_OVERRIDE_ATOM_BASE_CLASS {
            switch (mWaveform) {
                // @TODO("align phase offset with `NodeVCOWavetable`")
                case SINE:
                    pSignal = process_sine();
                    break;
                case TRIANGLE:
                    pSignal = process_triangle();
                    break;
                case SAWTOOTH:
                    pSignal = process_sawtooth();
                    break;
                case SQUARE:
                    pSignal = process_square();
                    break;
            }
            pSignal *= mAmplitude;
            pSignal += mOffset;
            return pSignal;
        }

        float *process(float *pSignalBlock, uint16_t pNumSamples) KLANG_OVERRIDE_ATOM_BASE_CLASS {
            for (uint16_t i = 0; i < pNumSamples; i++) {
                pSignalBlock[i] = process();
            }
            return pSignalBlock;
        }

        const Signal &process(const Signal &pSignal) KLANG_OVERRIDE_ATOM_BASE_CLASS {
            const float mSignal = process();
            for (uint8_t i = 0; i < pSignal.channels; ++i) {
                pSignal.signal[i] = mSignal;
            }
            return pSignal;
        }

        const Signal *&process(const Signal *&pSignalBlock, uint16_t pNumSamples) KLANG_OVERRIDE_ATOM_BASE_CLASS {
            for (uint16_t i = 0; i < pNumSamples; i++) {
                process(pSignalBlock[i]);
            }
            return pSignalBlock;
        }

    private:
        float mFrequency = 0.0;
        double mStepSize = 0.0;
        double mPhase = 0.0;  // @NOTE("single precision introduces drift")
        float mAmplitude = SIGNAL_MAX;
        float mOffset = 0.0;
        WAVEFORM mWaveform = WAVEFORM::SINE;

        inline float process_sine() {
            mPhase += mStepSize;
            while (mPhase > TWO_PI) {
                mPhase -= TWO_PI;
            }
            return klang_math_sin((float) mPhase);
        }

        inline float process_triangle() {
            mPhase += mFrequency;
            mPhase = KlangMath::mod((float) mPhase, KLANG_AUDIO_RATE);
            const float mPhaseShifted = (float) mPhase - (KLANG_AUDIO_RATE / 2.0f);
            const float mPhaseShiftedAbs = mPhaseShifted > 0 ? mPhaseShifted : -mPhaseShifted;
            return (mPhaseShiftedAbs - (KLANG_AUDIO_RATE / 4.0f)) / (KLANG_AUDIO_RATE / 4.0f);
        }

        inline float process_sawtooth() {
            mPhase += mFrequency;
            mPhase = KlangMath::mod((float) mPhase, KLANG_AUDIO_RATE);
            return (float) (mPhase / (KLANG_AUDIO_RATE / 2.0)) + SIGNAL_MIN;
        }

        inline float process_square() {
            mPhase += mFrequency;
            mPhase = KlangMath::mod((float) mPhase, KLANG_AUDIO_RATE);
            return mPhase > (KLANG_AUDIO_RATE / 2.0) ? SIGNAL_MAX : SIGNAL_MIN;
        }
    };
}  // namespace klang

#endif /* _KLANG_ATOM_OSCILLATOR_FUNCTION_H_ */
