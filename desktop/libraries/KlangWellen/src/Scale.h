/*
 * Wellen
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#pragma once

#include <stdint.h>

#include <initializer_list>

namespace klangwellen {
    class Scale {
    public:
        static const Scale CHROMATIC;
        static const Scale FIFTH;
        static const Scale MINOR;
        static const Scale MAJOR;
        static const Scale MINOR_CHORD;
        static const Scale MAJOR_CHORD;
        static const Scale MINOR_CHORD_7;
        static const Scale MAJOR_CHORD_7;
        static const Scale MINOR_PENTATONIC;
        static const Scale MAJOR_PENTATONIC;
        static const Scale OCTAVE;
        static const Scale DIMINISHED;

        Scale(const std::initializer_list<uint8_t>& note_list) : length(note_list.size()) {
            notes      = new uint8_t[length];
            uint16_t i = 0;
            for (uint8_t note : note_list) {
                notes[i] = note;
                i++;
            }
        }

        ~Scale() {
            delete[] notes;
        }

        const uint8_t length;
        uint8_t*      notes;

        static int note(const Scale& pScale, const int pBaseNote, const int pNoteStepOffset) {
            if (pNoteStepOffset > 0) {
                const int mSize       = pScale.length;
                const int mOctave     = pNoteStepOffset / mSize;
                const int mIndex      = pNoteStepOffset % mSize;
                const int mScaleEntry = pScale.notes[mIndex];
                const int mNote       = pBaseNote + (mOctave * 12) + mScaleEntry;
                return mNote;
            } else if (pNoteStepOffset < 0) {
                const int mSize                     = pScale.length;
                const int mNormalizedNoteStepOffset = -pNoteStepOffset + (mSize - 1);
                const int mOctave                   = -mNormalizedNoteStepOffset / mSize;
                const int mIndex                    = (mSize - 1) - (mNormalizedNoteStepOffset % mSize);
                const int mScaleEntry               = pScale.notes[mIndex];
                const int mNote                     = pBaseNote + (mOctave * 12) + mScaleEntry;
                return mNote;
            } else {
                return pBaseNote;
            }
        }
    };
}  // namespace klangwellen
