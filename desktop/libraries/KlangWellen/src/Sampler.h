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

/**
 * PROCESSOR INTERFACE
 *
 * - [x] float process()
 * - [ ] float process(float)‌
 * - [ ] void process(Signal&)
 * - [x] void process(float*, uint32_t) *overwrite*
 * - [ ] void process(float*, float*, uint32_t)
 */

/*
 * TODO
 * - LINE 325: check for memory leak and change recording mechanism in a way that the maximum length can be specified beforehand as well as a pre-allocated buffer
 * - LINE 153: "huuui, this is not nice and might cause some trouble somewhere"
 * - LINE 220: "evaluate direction?"
 */

#pragma once

#include <vector>

#include "KlangWellen.h"

namespace klangwellen {

    class SamplerListener {
    public:
        virtual void is_done() = 0;
    };

    /**
     * plays back an array of samples at different speeds.
     */
    template <class BUFFER_TYPE>
    class SamplerT {
    public:
        static const int8_t NO_LOOP_POINT = -1;

        SamplerT() : SamplerT(0) {}

        SamplerT(int32_t buffer_length, uint32_t sampling_rate = KlangWellen::DEFAULT_SAMPLING_RATE) : SamplerT(new BUFFER_TYPE[buffer_length], buffer_length, sampling_rate) {
            fAllocatedBuffer = true;
        }

        SamplerT(BUFFER_TYPE* buffer,
                 int32_t      buffer_length,
                 uint32_t     sampling_rate = KlangWellen::DEFAULT_SAMPLING_RATE) : fDirectionForward(true),
                                                                                fInPoint(0),
                                                                                fOutPoint(0),
                                                                                fSpeed(0) {
            fSamplingRate = sampling_rate;
            set_buffer(buffer, buffer_length);
            fBufferIndex        = 0;
            fInterpolateSamples = false;
            fEdgeFadePadding    = 0;
            fIsPlaying          = false;
            set_in(0);
            set_out(fBufferLength - 1);
            fFrequencyScale = 1.0f;
            set_speed(1.0f);
            set_amplitude(1.0f);
            fIsRecording     = false;
            fAllocatedBuffer = false;
        }

        ~SamplerT() {
            if (fAllocatedBuffer) {
                delete[] fBuffer;
            }
        }

        void add_listener(SamplerListener* sampler_listener) {
            fSamplerListeners.push_back(sampler_listener);
        }

        bool remove_listener(SamplerListener* sampler_listener) {
            for (auto it = fSamplerListeners.begin(); it != fSamplerListeners.end(); ++it) {
                if (*it == sampler_listener) {
                    fSamplerListeners.erase(it);
                    return true;
                }
            }
            return false;
        }

        int32_t get_in() {
            return fInPoint;
        }

        void set_in(int32_t in_point) {
            if (in_point > fOutPoint) {
                in_point = fOutPoint;
            }
            fInPoint = in_point;
        }

        int32_t get_out() {
            return fOutPoint;
        }

        void set_out(int32_t out_point) {
            fOutPoint = out_point > last_index() ? last_index() : (out_point < fInPoint ? fInPoint : out_point);
        }

        float get_speed() {
            return fSpeed;
        }

        void set_speed(float speed) {
            fSpeed            = speed;
            fDirectionForward = speed > 0;
            set_frequency(KlangWellen::abs(speed) * fSamplingRate / fBufferLength); /* aka `step_size = speed` */
        }

        void set_frequency(float frequency) {
            fFrequency = frequency;
            fStepSize  = fFrequency / fFrequencyScale * ((float)fBufferLength / fSamplingRate);
        }

        float get_frequency() {
            return fFrequency;
        }

        void set_amplitude(float amplitude) {
            fAmplitude = amplitude;
        }

        float get_amplitude() {
            return fAmplitude;
        }

        BUFFER_TYPE* get_buffer() {
            return fBuffer;
        }

        int32_t get_buffer_length() {
            return fBufferLength;
        }

        void set_buffer(BUFFER_TYPE* buffer, int32_t buffer_length) {
            fAllocatedBuffer = false;  // TODO huuui, this is not nice and might cause some trouble somewhere
            fBuffer          = buffer;
            fBufferLength    = buffer_length;
            rewind();
            set_speed(fSpeed);
            set_in(0);
            set_out(fBufferLength - 1);
            fLoopIn  = NO_LOOP_POINT;
            fLoopOut = NO_LOOP_POINT;
        }

        void interpolate_samples(bool interpolate_samples) {
            fInterpolateSamples = interpolate_samples;
        }

        bool interpolate_samples() {
            return fInterpolateSamples;
        }

        int32_t get_position() {
            return (int32_t)fBufferIndex;
        }

        float get_position_normalized() {
            return fBufferLength > 0 ? fBufferIndex / fBufferLength : 0.0f;
        }

        float get_position_fractional_part() {
            return fBufferIndex - get_position();
        }

        bool is_playing() {
            return fIsPlaying;
        }

        float process() {
            if (fBufferLength == 0) {
                notifyListeners();  // "buffer is empty"
                return 0.0f;
            }

            if (!fIsPlaying) {
                notifyListeners();  // "not playing"
                return 0.0f;
            }

            validateInOutPoints();

            fBufferIndex += fDirectionForward ? fStepSize : -fStepSize;
            const int32_t mRoundedIndex = (int32_t)fBufferIndex;

            const float   mFrac         = fBufferIndex - mRoundedIndex;
            const int32_t mCurrentIndex = wrapIndex(mRoundedIndex);
            fBufferIndex                = mCurrentIndex + mFrac;

            if (fDirectionForward ? (mCurrentIndex >= fOutPoint) : (mCurrentIndex <= fInPoint)) {
                notifyListeners();  // "reached end"
                return 0.0f;
            } else {
                fIsFlaggedDone = false;
            }

            float mSample = convert_sample(fBuffer[mCurrentIndex]);

            /* interpolate */
            if (fInterpolateSamples) {
                // TODO evaluate direction?
                const int32_t mNextIndex  = wrapIndex(mCurrentIndex + 1);
                const float   mNextSample = fBuffer[mNextIndex];
                mSample                   = mSample * (1.0f - mFrac) + mNextSample * mFrac;
            }
            mSample *= fAmplitude;

            /* fade edges */
            if (fEdgeFadePadding > 0) {
                const int32_t mRelativeIndex = fBufferLength - mCurrentIndex;
                if (mCurrentIndex < fEdgeFadePadding) {
                    const float mFadeInAmount = (float)mCurrentIndex / fEdgeFadePadding;
                    mSample *= mFadeInAmount;
                } else if (mRelativeIndex < fEdgeFadePadding) {
                    const float mFadeOutAmount = (float)mRelativeIndex / fEdgeFadePadding;
                    mSample *= mFadeOutAmount;
                }
            }
            return mSample;
        }

        void process(float* signal_buffer, const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint16_t i = 0; i < buffer_length; i++) {
                signal_buffer[i] = process();
            }
        }

        int32_t get_edge_fading() {
            return fEdgeFadePadding;
        }

        void set_edge_fading(int32_t edge_fade_padding) {
            fEdgeFadePadding = edge_fade_padding;
        }

        void rewind() {
            fBufferIndex = fDirectionForward ? fInPoint : fOutPoint;
        }

        void forward() {
            fBufferIndex = fDirectionForward ? fOutPoint : fInPoint;
        }

        bool is_looping() {
            return fEvaluateLoop;
        }

        void enable_loop(bool loop) {
            fEvaluateLoop = loop;
        }

        void set_loop_all() {
            fEvaluateLoop = true;
            fLoopIn       = 0;
            fLoopOut      = fBufferLength > 0 ? (fBufferLength - 1) : 0;
        }

        void play() {
            fIsPlaying = true;
            fRecording.clear();
        }

        void stop() {
            fIsPlaying = false;
        }

        void start_recording() {
            fIsRecording = true;
        }

        void resume_recording() {
            fIsRecording = true;
        }

        void pause_recording() {
            fIsRecording = false;
        }

        void delete_recording() {
            fRecording.clear();
        }

        void record(float sample) {
            if (fIsRecording) {
                fRecording.push_back(sample);
            }
        }

        void record(float* samples, int32_t num_samples) {
            if (fIsRecording) {
                for (int32_t i = 0; i < num_samples; i++) {
                    const float sample = samples[i];
                    fRecording.push_back(sample);
                }
            }
        }

        bool is_recording() {
            return fIsRecording;
        }

        int get_length_recording() {
            return fRecording.size();
        }

        uint32_t end_recording() {
            fIsRecording                = false;
            const int32_t mBufferLength = fRecording.size();
            float*        mBuffer       = new float[mBufferLength];
            for (int32_t i = 0; i < mBufferLength; i++) {
                mBuffer[i] = fRecording[i];
            }
            fRecording.clear();
            if (fAllocatedBuffer) {
                delete[] fBuffer;
            }
            set_buffer(mBuffer, mBufferLength);
            fAllocatedBuffer = true;
            return mBufferLength;
        }

        int32_t get_loop_in() {
            return fLoopIn;
        }

        void set_loop_in(int32_t loop_in_point) {
            fLoopIn = KlangWellen::clamp(loop_in_point, NO_LOOP_POINT, fBufferLength - 1);
        }

        float get_loop_in_normalized() {
            if (fBufferLength < 2) {
                return 0.0f;
            }
            return (float)fLoopIn / (fBufferLength - 1);
        }

        void set_loop_in_normalized(float loop_in_point_normalized) {
            set_loop_in((int32_t)(loop_in_point_normalized * fBufferLength - 1));
        }

        int32_t get_loop_out() {
            return fLoopOut;
        }

        void set_loop_out(int32_t loop_out_point) {
            fLoopOut = KlangWellen::clamp(loop_out_point, NO_LOOP_POINT, fBufferLength - 1);
        }

        float get_loop_out_normalized() {
            if (fBufferLength < 2) {
                return 0.0f;
            }
            return (float)fLoopOut / (fBufferLength - 1);
        }

        void set_loop_out_normalized(float loop_out_point_normalized) {
            set_loop_out((int32_t)(loop_out_point_normalized * fBufferLength - 1));
        }

        void note_on() {
            rewind();
            play();
            enable_loop(true);
        }

        void note_on(uint8_t note, uint8_t velocity) {
            fIsPlaying = true;
            set_frequency(KlangWellen::note_to_frequency(note));
            set_amplitude(KlangWellen::clamp127(velocity) / 127.0f);
            note_on();
        }

        void note_off() {
            enable_loop(false);
        }

        /**
         * this function can be used to tune a loaded sample to a specific frequency. after the sampler has been tuned the
         * method <code>set_frequency(float)</code> can be used to play the sample at a desired frequency.
         *
         * @param frequency_scale the assumed frequency of the sampler buffer in Hz
         */
        void tune_frequency_to(float tune_frequency) {
            fFrequencyScale = tune_frequency;
        }

        void set_duration(float seconds) {
            if (fBufferLength == 0 || seconds == 0.0f) {
                return;
            }
            const float mNormDurationSec = ((float)fBufferLength / (float)fSamplingRate);
            const float mSpeed           = mNormDurationSec / seconds;
            set_speed(mSpeed);
        }

        float get_duration() {
            if (fBufferLength == 0 || fSpeed == 0.0f) {
                return 0;
            }
            const float mNormDurationSec = ((float)fBufferLength / (float)fSamplingRate);
            return mNormDurationSec / fSpeed;
        }

    private:
        std::vector<SamplerListener*> fSamplerListeners;
        std::vector<BUFFER_TYPE>      fRecording;
        uint32_t                      fSamplingRate;
        float                         fAmplitude;
        BUFFER_TYPE*                  fBuffer;
        int32_t                       fBufferLength;
        float                         fBufferIndex;
        bool                          fDirectionForward;
        int32_t                       fEdgeFadePadding;
        bool                          fEvaluateLoop;
        float                         fFrequency;
        float                         fFrequencyScale;
        int32_t                       fInPoint;
        int32_t                       fOutPoint;
        int32_t                       fLoopIn;
        int32_t                       fLoopOut;
        bool                          fInterpolateSamples;
        bool                          fIsPlaying;
        float                         fSpeed;
        float                         fStepSize;
        bool                          fIsFlaggedDone;
        bool                          fIsRecording;
        bool                          fAllocatedBuffer;

        int32_t last_index() {
            return fBufferLength - 1;
        }

        void notifyListeners() {
            if (!fIsFlaggedDone) {
                for (SamplerListener* l : fSamplerListeners) {
                    l->is_done();
                }
            }
            fIsFlaggedDone = true;
        }

        void validateInOutPoints() {
            if (fInPoint < 0) {
                fInPoint = 0;
            } else if (fInPoint > fBufferLength - 1) {
                fInPoint = fBufferLength - 1;
            }
            if (fOutPoint < 0) {
                fOutPoint = 0;
            } else if (fOutPoint > fBufferLength - 1) {
                fOutPoint = fBufferLength - 1;
            }
            if (fOutPoint < fInPoint) {
                fOutPoint = fInPoint;
            }
            if (fLoopIn < fInPoint) {
                fLoopIn = fInPoint;
            }
            if (fLoopOut > fOutPoint) {
                fLoopOut = fOutPoint;
            }
        }

        int32_t wrapIndex(int32_t i) {
            /* check if in loop concept viable i.e loop in- and output points are set */
            if (fEvaluateLoop) {
                if (fLoopIn != NO_LOOP_POINT && fLoopOut != NO_LOOP_POINT) {
                    if (fDirectionForward) {
                        if (i > fLoopOut) {
                            i = fLoopIn;
                        }
                    } else {
                        if (i < fLoopIn) {
                            i = fLoopOut;
                        }
                    }
                }
            }

            /* check if within bounds */
            if (i > fOutPoint) {
                i = fOutPoint;
            } else if (i < fInPoint) {
                i = fInPoint;
            }
            return i;
        }

        inline float convert_sample(const BUFFER_TYPE pRawSample) {
            return pRawSample;
        }
    };

    template <>
    float klangwellen::SamplerT<uint8_t>::convert_sample(const uint8_t pRawSample) {
        const static float mScale = 1.0 / ((1 << 8) - 1);
        const float        mRange = pRawSample * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    float klangwellen::SamplerT<int8_t>::convert_sample(const int8_t pRawSample) {
        const static float mScale  = 1.0 / ((1 << 8) - 1);
        const float        mOffset = pRawSample + (1 << 7);
        const float        mRange  = mOffset * mScale;
        return mRange * 2.0 - 1.0;
    }

    template <>
    float klangwellen::SamplerT<uint16_t>::convert_sample(const uint16_t pRawSample) {
        const static float mScale = 1.0 / ((1 << 16) - 1);
        const float        mRange = pRawSample * mScale;
        return mRange * 2.0 - 1.0;
        // @note(below: less precise but faster)
        // const float s      = pRawSample;
        // static const float mScale = 1.0 / (1 << 15);
        // const float a      = s * mScale - 1.0;
        // return a;
    }

    template <>
    float klangwellen::SamplerT<int16_t>::convert_sample(const int16_t pRawSample) {
        const static float mScale  = 1.0 / ((1 << 16) - 1);
        const float        mOffset = pRawSample + (1 << 15);
        const float        mRange  = mOffset * mScale;
        return mRange * 2.0 - 1.0;
    }

    using SamplerUI8  = SamplerT<uint8_t>;
    using SamplerI8   = SamplerT<int8_t>;
    using SamplerUI16 = SamplerT<uint16_t>;
    using SamplerI16  = SamplerT<int16_t>;
    using SamplerF32  = SamplerT<float>;
    using Sampler     = SamplerT<float>;
}  // namespace klangwellen
