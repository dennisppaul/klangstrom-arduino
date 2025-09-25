/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2025 Dennis P Paul.
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
#include "PeriodicTimer.h"
#include "KlangstromConfiguration.h"

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

#ifdef __cplusplus
extern "C" {
#endif

void beat_event(uint8_t beat_id, uint16_t beat_counter);

#ifdef __cplusplus
}
#endif

typedef void (*Callback_2_UI8_UI16)(uint8_t, uint16_t);

class Beat {
public:
    explicit Beat() : _callback_beat(nullptr),
                      _beat_counter(0),
                      _is_running(false) {
        set_callback(beat_event);
    }

    void init(const uint8_t beat_id = KLST_DEFAULT_BEAT_TIMER) {
        _device_id = beat_id;
        _timer     = periodic_timer_create(_device_id);
        if (_timer) {
            _timer->callback = std::bind(&Beat::beat_timer_event, this, std::placeholders::_1);
        }
    }

    void set_bpm(const float beats_per_minute) const {
        if (_timer == nullptr) {
            return;
        }
        if (beats_per_minute == 0) {
            return;
        }
        const uint32_t duration_us = (60.0f / beats_per_minute) * 1000000;
        periodic_timer_set_overflow(_timer, duration_us);
    }

    void pause() {
        if (_timer == nullptr) {
            return;
        }
        if (!_is_running) {
            return;
        }
        _is_running = false;
        periodic_timer_pause(_timer);
    }

    void start() {
        reset();
        resume();
    }

    void resume() {
        if (_timer == nullptr) {
            return;
        }
        if (_is_running) {
            return;
        }
        _is_running = true;
        periodic_timer_resume(_timer);
    }

    bool is_running() const {
        return _is_running;
    }

    void reset() {
        _beat_counter = 0;
    }

    void set_callback(const Callback_2_UI8_UI16& callback) {
        _callback_beat = callback;
    }

private:
    PeriodicTimer*      _timer;
    uint8_t             _device_id;
    Callback_2_UI8_UI16 _callback_beat;
    uint32_t            _beat_counter;
    bool                _is_running;

    void beat_timer_event(const PeriodicTimer* timer) {
        _beat_counter++;
        if (_callback_beat != nullptr) {
            _callback_beat(_device_id, _beat_counter);
        }
    }
};
