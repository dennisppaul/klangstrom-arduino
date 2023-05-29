/*
 * Klangstrom
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

#include "KlangstromApplicationInterface.h"

static CallbackType3_UI8_I16_I16 p_callback_encoder_rotated = nullptr;

void klangstrom::register_encoder_rotated(CallbackType3_UI8_I16_I16 callback) {
    p_callback_encoder_rotated = callback;
}

void klangstrom::call_encoder_rotated(const uint8_t index, const int16_t ticks, const int16_t delta) {
    if (p_callback_encoder_rotated) {
        p_callback_encoder_rotated(index, ticks, delta);
    }
}

static CallbackType1_UI8 p_callback_encoder_pressed = nullptr;

void klangstrom::register_encoder_pressed(CallbackType1_UI8 callback) {
    p_callback_encoder_pressed = callback;
}

void klangstrom::call_encoder_pressed(const uint8_t index) {
    if (p_callback_encoder_pressed) {
        p_callback_encoder_pressed(index);
    }
}

static CallbackType1_UI8 p_callback_encoder_released = nullptr;

void klangstrom::register_encoder_released(CallbackType1_UI8 callback) {
    p_callback_encoder_released = callback;
}

void klangstrom::call_encoder_released(const uint8_t index) {
    if (p_callback_encoder_released) {
        p_callback_encoder_released(index);
    }
}

static CallbackType2_UI8_VOIDPTR p_callback_encoder_event_receive = nullptr;

void klangstrom::register_encoder_event_receive(CallbackType2_UI8_VOIDPTR callback) {
    p_callback_encoder_event_receive = callback;
}

void klangstrom::call_encoder_event_receive(const uint8_t event, const void *data) {
    if (p_callback_encoder_event_receive) {
        p_callback_encoder_event_receive(event, data);
    }
}

static CallbackType2_FLOATPTRPTR_FLOATPTRPTR p_callback_audioblock = nullptr;

void klangstrom::register_audioblock(CallbackType2_FLOATPTRPTR_FLOATPTRPTR callback) {
    p_callback_audioblock = callback;
}

void klangstrom::call_audioblock(float **input_buffer, float **output_buffer) {
    if (p_callback_audioblock) {
        p_callback_audioblock(input_buffer, output_buffer);
    }
}