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
 * generates an event from an oscillating input signal.
 */
namespace klangwellen {
    class TriggerListener {
    public:
        virtual void trigger(uint8_t event) = 0;
    };

    class Trigger {
    public:
        static const uint8_t EVENT_FALLING_EDGE = -1;
        static const uint8_t EVENT_RISING_EDGE  = 1;

        Trigger() : fPreviousSignal(0.0), fEnableFallingEdge(true), fEnableRisingEdge(true) {}

        void add_listener(TriggerListener* listener) {
            fListeners.push_back(listener);
        }

        bool remove_listener(TriggerListener* listener) {
            for (auto it = fListeners.begin(); it != fListeners.end(); ++it) {
                if (*it == listener) {
                    fListeners.erase(it);
                    return true;
                }
            }
            return false;
        }

        void trigger_rising_edge(bool enable_trigger_rising_edge) {
            fEnableRisingEdge = enable_trigger_rising_edge;
        }

        void trigger_falling_edge(bool enable_trigger_falling_edge) {
            fEnableFallingEdge = enable_trigger_falling_edge;
        }

        float process(float signal) {
            if (fEnableRisingEdge && (fPreviousSignal <= 0 && signal > 0)) {
                fireEvent(EVENT_RISING_EDGE);
            }
            if (fEnableFallingEdge && (fPreviousSignal >= 0 && signal < 0)) {
                fireEvent(EVENT_FALLING_EDGE);
            }
            fPreviousSignal = signal;
            return signal;
        }

        void process(float* signal_buffer, const uint32_t buffer_length = KLANG_SAMPLES_PER_AUDIO_BLOCK) {
            for (uint16_t i = 0; i < buffer_length; i++) {
                process(signal_buffer[i]);
            }
        }

    private:
        typedef void (*CallbackType1_UI8)(const uint8_t);
        CallbackType1_UI8 fCallbackEvent = nullptr;

    public:
        void set_callback(CallbackType1_UI8 callback) {
            fCallbackEvent = callback;
        }

        void remove_callback() {
            fCallbackEvent = nullptr;
        }

    private:
        float                         fPreviousSignal;
        bool                          fEnableFallingEdge;
        bool                          fEnableRisingEdge;
        std::vector<TriggerListener*> fListeners;

        void call_trigger(const uint8_t event) {
            if (fCallbackEvent) {
                fCallbackEvent(event);
            }
        }

        void fireEvent(int event) {
            for (TriggerListener* l : fListeners) {
                l->trigger(event);
            }
            call_trigger(event);
        }
    };
}  // namespace klangwellen
