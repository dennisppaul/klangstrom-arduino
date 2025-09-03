/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
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

#include <functional>
#include <stdint.h>
#include "Timer.h"

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

#ifdef __cplusplus
extern "C" {
#endif

WEAK void beat_event(uint8_t beat_id, uint16_t beat_counter);

#ifdef __cplusplus
}
#endif

typedef void (*Callback_2_UI8_UI16)(uint8_t, uint16_t);

class Beat {
public:
    explicit Beat() : callback_beat(nullptr),
                      beat_counter(0),
                      fIsRunning(false) {
        set_callback(beat_event);
    }

    void init(const uint8_t beat_id) {
        device_id = beat_id;
        timer     = timer_create(device_id);
        if (timer) {
            timer->callback = std::bind(&Beat::beat_timer_event, this, std::placeholders::_1);
        }
    }

    void beat_timer_event(const Timer* timer) {
        beat_counter++;
        if (callback_beat != nullptr) {
            callback_beat(device_id, beat_counter);
        }
    }

    void set_bpm(const float beats_per_minute) const {
        if (timer == nullptr) {
            return;
        }
        if (beats_per_minute == 0) {
            return;
        }
        const uint32_t duration_us = (60.0f / beats_per_minute) * 1000000;
        timer_set_overflow(timer, duration_us);
    }

    void pause() {
        if (timer == nullptr) {
            return;
        }
        if (!fIsRunning) {
            return;
        }
        fIsRunning = false;
        timer_pause(timer);
    }

    void start() {
        reset();
        resume();
    }

    void resume() {
        if (timer == nullptr) {
            return;
        }
        if (fIsRunning) {
            return;
        }
        fIsRunning = true;
        timer_resume(timer);
    }

    bool is_running() const {
        return fIsRunning;
    }

    void reset() {
        beat_counter = 0;
    }

    void set_callback(const Callback_2_UI8_UI16& callback) {
        callback_beat = callback;
    }

private:
    Timer*              timer;
    uint8_t             device_id;
    Callback_2_UI8_UI16 callback_beat;
    uint32_t            beat_counter;
    bool                fIsRunning;
};
