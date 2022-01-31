/*
 * Klangstrom
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

#ifndef sample_buffer_h
#define sample_buffer_h

namespace klang {
    template <class BUFFER_TYPE>
    class SampleBufferT : public Node {
    public:
        BUFFER_TYPE* get_length() { return mLength; }

        BUFFER_TYPE* get_data() { return mData; }

        void set_data(BUFFER_TYPE* pData, uint32_t pLength) {
            mData   = pData;
            mLength = pLength;
        }

    private:
        BUFFER_TYPE* mData;
        uint32_t     mLength;
    };
    using SampleBufferUI8  = SampleBufferT<uint8_t>;
    using SampleBufferI8   = SampleBufferT<int8_t>;
    using SampleBufferUI16 = SampleBufferT<uint16_t>;
    using SampleBufferI16  = SampleBufferT<int16_t>;
    using SampleBufferF32  = SampleBufferT<float>;
    using SampleBuffer     = SampleBufferT<SIGNAL_TYPE>;

}  // namespace klang

#endif  // sample_buffer_h