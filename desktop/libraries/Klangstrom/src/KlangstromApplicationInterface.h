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

#ifndef KlangstromApplicationInterface_h
#define KlangstromApplicationInterface_h

#ifdef __cplusplus
#include "KlangstromDefines.hpp"
#include "KlangstromEvents.h"
#endif  // __cplusplus
#include "KlangstromDefinesArduino.h"

/* --------------------------------------------------------------------------------------------- */
/* --- application-level functions ------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

#ifdef __cplusplus

typedef void (*CallbackType1_UI8)(const uint8_t);
typedef void (*CallbackType2_UI8_VOIDPTR)(const uint8_t, const void*);
typedef void (*CallbackType3_UI8_I16_I16)(const uint8_t, const int16_t, const int16_t);
typedef void (*CallbackType2_FLOATPTRPTR_FLOATPTRPTR)(float**, float**);

namespace klangstrom {

    void register_encoder_rotated(CallbackType3_UI8_I16_I16 callback);
    void call_encoder_rotated(const uint8_t index, const int16_t ticks, const int16_t delta);
    void register_encoder_pressed(CallbackType1_UI8 callback);
    void call_encoder_pressed(const uint8_t index);
    void register_encoder_released(CallbackType1_UI8 callback);
    void call_encoder_released(const uint8_t index);
    void register_encoder_event_receive(CallbackType2_UI8_VOIDPTR callback);
    void call_encoder_event_receive(const uint8_t event, const void* data);
    void register_audioblock(CallbackType2_FLOATPTRPTR_FLOATPTRPTR callback);
    void call_audioblock(float** input_buffer, float** output_buffer);

    void     beats_per_minute(float BPM);
    void     beats_per_minute_ms(uint32_t micro_seconds);
    void     begin_serial_debug(bool wait_for_serial = false, uint32_t baud_rate = 115200);
    bool     button_state(uint8_t button);
    void     data_transmit(const uint8_t sender, uint8_t* data, uint16_t length);
    void     event_transmit(EVENT_TYPE event, float* payload);
    int16_t  ID();
    char*    U_ID();
    void     LED(uint16_t pin, uint8_t state);
    int      get_LED_pin(uint16_t index);
    void     option(uint8_t option, float value);
    float    get_option(uint8_t option);
    bool     pin_state(uint8_t button);
    uint16_t ADC(uint8_t ADC);
    void     DAC(uint8_t DAC, uint16_t value);
    uint32_t query_available_memory_block();
    uint8_t  get_input_channels();
    uint8_t  get_output_channels();
};  // namespace klangstrom

#endif  // __cplusplus

#endif /* KlangstromApplicationInterface_h */
