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
 * - [ ] float process(float)‌
 * - [ ] void process(Signal&)
 * - [x] void process(float*, uint32_t) *no overwrites*
 * - [ ] void process(float*, float*, uint32_t)
 */

#pragma once

#include <stdint.h>

#include <vector>

#include "KlangWellen.h"

/**
 * similar to {@link wellen.Beat} with the exception that events are triggered from {@link DSP}.
 */
namespace klangwellen {
    class BeatListener {
    public:
        virtual void beat(uint32_t beat_counter) = 0;
    };

    class BeatDSP {
    public:
        BeatDSP(uint32_t sampling_rate = KlangWellen::DEFAULT_SAMPLING_RATE) : fSamplingRate(sampling_rate), fBeat(-1) {
            set_bpm(120);
        }

        void add_listener(BeatListener* listener) {
            fListeners.push_back(listener);
        }

        bool remove_listener(BeatListener* listener) {
            for (auto it = fListeners.begin(); it != fListeners.end(); ++it) {
                if (*it == listener) {
                    fListeners.erase(it);
                    return true;
                }
            }
            return false;
        }

        void set_bpm(float BPM) {
            const float mPeriod = 60.0f / BPM;
            fTickInterval       = fSamplingRate * mPeriod;
        }

        /**
         * sets the interval between beat events in samples
         *
         * @param interval in samples
         */
        void set_interval(float interval) {
            fTickInterval = interval;
        }

        /**
         * sets the interval between beat events in seconds
         *
         * @param interval in seconds
         */
        void set_interval_sec(float interval) {
            fTickInterval = fSamplingRate * interval;
        }

        int get_beat_count() {
            return fBeat;
        }

        float process() {
            fTickCounter++;
            if (fTickCounter >= fTickInterval) {
                fireEvent();
                fTickCounter -= fTickInterval;
            }
            return 0.0;
        }

        void process(float* signal_buffer, const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint16_t i = 0; i < buffer_length; i++) {
                process();
            }
        }

        /* --- function callback --- */

    private:
        typedef void (*CallbackType1_UI32)(const uint32_t);
        CallbackType1_UI32 fCallbackEvent = nullptr;

    public:
        void set_callback(CallbackType1_UI32 callback) {
            fCallbackEvent = callback;
        }

        void remove_callback() {
            fCallbackEvent = nullptr;
        }

    private:
        const uint32_t             fSamplingRate;
        int                        fBeat;
        std::vector<BeatListener*> fListeners;
        uint32_t                   fTickCounter;
        float                      fTickInterval;

        void call_beat(const uint32_t beat_counter) {
            if (fCallbackEvent) {
                fCallbackEvent(beat_counter);
            }
        }

        void fireEvent() {
            fBeat++;
            for (BeatListener* l : fListeners) {
                l->beat(fBeat);
            }
            call_beat(fBeat);
        }
    };
}  // namespace klangwellen