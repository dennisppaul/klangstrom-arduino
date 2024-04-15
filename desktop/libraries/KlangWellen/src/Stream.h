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
 * - [ ] float process(float)
 * - [ ] void process(Signal&)
 * - [*] void process(float*, uint32_t)
 * - [ ] void process(float*, float*, uint32_t)
 */

#pragma once

namespace klangwellen {
    class StreamDataProvider {
    public:
        virtual ~StreamDataProvider() = default;

        virtual void fill_buffer(float *buffer, uint32_t length) {
            std::fill_n(buffer, length, 0.0f);
        }
    };

    class Stream final {
    public:
        Stream(StreamDataProvider *stream_data_provider,
               const uint32_t      stream_buffer_size,
               const uint8_t       stream_buffer_division      = 4,
               const uint8_t       stream_buffer_update_offset = 1,
               const uint32_t      sampling_rate               = KlangWellen::DEFAULT_SAMPLING_RATE)
            : fStreamDataProvider(stream_data_provider),
              fBufferLength(stream_buffer_size),
              fBuffer(new float[stream_buffer_size]),
              fBufferDivision(stream_buffer_division),
              fBufferSegmentOffset(stream_buffer_update_offset % stream_buffer_division),
              fSamplingRate(sampling_rate),
              fAmplitude(1.0f),
              fStepSize(1.0f),
              fInterpolateSamples(true),
              fBufferIndex(0.0f),
              fBufferIndexPrev(0.0f),
              fCompleteEvent(NO_EVENT) {
            fStreamDataProvider->fill_buffer(fBuffer, fBufferLength);
        }

        ~Stream() {
            delete[] fBuffer;
        }

        float process() {
            fBufferIndex += fStepSize;
            const int32_t mRoundedIndex = static_cast<int32_t>(fBufferIndex);
            const float   mFrac         = fBufferIndex - mRoundedIndex;
            const int32_t mCurrentIndex = wrapIndex(mRoundedIndex);
            fBufferIndex                = mCurrentIndex + mFrac;

            float mSample = convert_sample(fBuffer[mCurrentIndex]);

            /* linear interpolation */
            if (fInterpolateSamples) {
                const int32_t mNextIndex  = wrapIndex(mCurrentIndex + 1);
                const float   mNextSample = convert_sample(fBuffer[mNextIndex]);
                const float   a           = mSample * (1.0f - mFrac);
                const float   b           = mNextSample * mFrac;
                mSample                   = a + b;
            }
            mSample *= fAmplitude;

            /* load next block */
            int8_t mCompleteEvent = checkCompleteEvent(fBufferDivision);
            if (mCompleteEvent > NO_EVENT) {
                fCompleteEvent = mCompleteEvent;
                mCompleteEvent -= fBufferSegmentOffset;
                mCompleteEvent += fBufferDivision;
                mCompleteEvent %= fBufferDivision;
                replace_segment(fBufferDivision, mCompleteEvent);
            }
            fBufferIndexPrev = fBufferIndex;

            return mSample;
        }

        void replace_segment(const uint32_t numberOfSegments, const uint32_t segmentIndex) const {
            if (segmentIndex >= numberOfSegments) {
                std::cerr << "Segment index out of range" << std::endl;
                return;
            }

            const uint32_t lengthOfSegment = fBufferLength / numberOfSegments;
            float *        startOfSegment  = fBuffer + (segmentIndex * lengthOfSegment);

            fStreamDataProvider->fill_buffer(startOfSegment, lengthOfSegment);
        }

        float *get_buffer() const {
            return fBuffer;
        }

        int8_t get_sector() const {
            return fCompleteEvent;
        }

        uint8_t num_sectors() const {
            return fBufferDivision;
        }

        uint32_t get_buffer_length() const {
            return fBufferLength;
        }

        float get_current_buffer_position() const {
            return fBufferIndex;
        }

        void process(float *signal_buffer, const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint16_t i = 0; i < buffer_length; i++) {
                signal_buffer[i] = process();
            }
        }

        void interpolate_samples(bool const interpolate_samples) {
            fInterpolateSamples = interpolate_samples;
        }

        bool interpolate_samples() const {
            return fInterpolateSamples;
        }

        float get_speed() const {
            return fStepSize;
        }

        void set_speed(const float speed) {
            fStepSize = speed;
        }

    private:
        static constexpr int8_t NO_EVENT = -1;

        StreamDataProvider *fStreamDataProvider;

        uint32_t      fBufferLength;
        float *       fBuffer;
        const uint8_t fBufferDivision;
        const uint8_t fBufferSegmentOffset;
        float         fSamplingRate;
        float         fAmplitude;
        float         fStepSize;
        bool          fInterpolateSamples;
        float         fBufferIndex;
        float         fBufferIndexPrev;
        int8_t        fCompleteEvent;

        int32_t wrapIndex(int32_t i) const {
            if (i < 0) {
                i += fBufferLength;
            } else if (i >= fBufferLength) {
                i -= fBufferLength;
            }
            return i;
        }

        static float convert_sample(const float pRawSample) {
            return pRawSample;
        }

        int8_t checkCompleteEvent(const uint8_t num_events) const {
            for (int i = 0; i < num_events; ++i) {
                const float mBorder = fBufferLength * i / static_cast<float>(fBufferDivision);
                if (crossedBorder(fBufferIndexPrev, fBufferIndex, mBorder)) {
                    return i;
                }
            }
            return NO_EVENT;
        }

        static bool crossedBorder(const float prev, const float current, const float border) {
            return (border == 0 && prev > current) ||
                   (prev < border && current >= border) ||
                   (prev > current && current >= border);
        }
    };
} // namespace klangwellen
