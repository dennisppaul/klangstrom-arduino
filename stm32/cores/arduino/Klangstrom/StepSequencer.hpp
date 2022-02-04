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

// @TODO(move to strom)

#ifndef StepSequencer_h
#define StepSequencer_h

#include <stdint.h>

#define USE_ASSERT 0

namespace klang {
    template <typename T>
    class StepSequencer {
        typedef uint16_t I_TYPE;

    public:
        StepSequencer(int pLength = 16) : mLength(pLength), mData(new T[pLength]) {}

        ~StepSequencer() {
            delete[] mData;
        }

        I_TYPE length() const {
            return mLength;
        }

        const T& operator[](I_TYPE i) const {
            return mData[check(i)];
        }

        T& operator[](I_TYPE i) {
            return mData[check(i)];
        }

        StepSequencer(const StepSequencer<T>&);
        StepSequencer(StepSequencer<T>&&);
        StepSequencer<T>& operator=(const StepSequencer<T>&);
        StepSequencer<T>& operator=(StepSequencer<T>&&);

        T next() {
            mStep++;
            while (mStep >= mLength) {
                mStep -= mLength;
            }
            return mData[mStep];
        }

        I_TYPE get_current_step() {
            return mStep;
        }

        T* sequence() {
            return mData;
        }

        void set(const T* pData) {
            for (I_TYPE i = 0; i < length(); i++) {
                mData[i] = pData[i];
            }
        }

        void shift_left() {
            const I_TYPE mLastIndex = length() - 1;
            const T      mFirst     = mData[0];
            for (I_TYPE i = 0; i < mLastIndex; i++) {
                mData[i] = mData[i + 1];
            }
            mData[mLastIndex] = mFirst;
        }

        void shift_right() {
            const I_TYPE mLastIndex = length() - 1;
            const T      mLast      = mData[mLastIndex];
            for (I_TYPE i = mLastIndex; i > 0; i--) {
                mData[i] = mData[i - 1];
            }
            mData[0] = mLast;
        }

        void shift(int pShiftSteps) {
            if (pShiftSteps > 0) {
                for (int i = 0; i < pShiftSteps; i++) {
                    shift_right();
                }
            } else if (pShiftSteps < 0) {
                const int mShiftStepsInv = pShiftSteps * -1;
                for (int i = 0; i < mShiftStepsInv; i++) {
                    shift_left();
                }
            }
        }

        void backwards() {
            T* mDataCopy(new T[mLength]);
            for (I_TYPE i = 0; i < length(); i++) {
                mDataCopy[i] = mData[i];
            }
            const I_TYPE mLastIndex = length() - 1;
            for (I_TYPE i = 0; i < length(); i++) {
                mData[i] = mDataCopy[mLastIndex - i];
            }
            delete[] mDataCopy;
        }

        void print(int pWrap = 4) {
            // TODO integrate into debugging output system
            // ... aka do not use `printf` directly
            for (int i = 0; i < length(); i++) {
                printf("%i", sequence()[i]);
                if ((i % pWrap) == (pWrap - 1)) {
                    printf("\n");
                } else {
                    printf(",");
                }
            }
            printf("\n");
        }

    private:
        I_TYPE mLength;
        T*     mData;

        I_TYPE mStep = -1;

        I_TYPE check(I_TYPE i) const {
#if (USE_ASSERT)
            assert(i >= 0 && i < mLength);
#endif
            return i;
        }
    };
}  // namespace klang

#endif /* StepSequencer_h */
