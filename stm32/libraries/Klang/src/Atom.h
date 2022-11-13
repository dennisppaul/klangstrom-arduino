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

#ifndef _KLANG_ATOM_H_
#define _KLANG_ATOM_H_

#include <cstdint>
#include "Klang.h"
#include "Signal.h"

namespace klang {
    class Atom {
    public:
        virtual float process(float pSignal) = 0;

        virtual float *process(float *pSignalBlock, uint16_t pNumSamples) = 0;

        virtual const Signal &process(const Signal &pSignal) = 0;

        virtual const Signal *&process(const Signal *&pSignalBlock, uint16_t pNumSamples) = 0;

        /* default parameter methods */

        inline float process() {
            return process(0.0f);
        }

        inline float *process(float *pSignalBlock) {
            return process(pSignalBlock, KLANG_SAMPLES_PER_AUDIO_BLOCK);
        }

        inline const Signal *&process(const Signal *&pSignalBlock) {
            return process(pSignalBlock, KLANG_SAMPLES_PER_AUDIO_BLOCK);
        }
    };
}  // namespace klang

#endif //_KLANG_ATOM_H_
