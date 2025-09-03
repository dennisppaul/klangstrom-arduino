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

#include <cstdint>
#include "ArrayList.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

static constexpr uint8_t ENCODER_LEFT            = 0;
static constexpr uint8_t ENCODER_RIGHT           = 1;
static constexpr uint8_t ENCODER_00              = ENCODER_LEFT;
static constexpr uint8_t ENCODER_01              = ENCODER_RIGHT;
static constexpr uint8_t ENCODER_TYPE_CUSTOM     = 127;
static constexpr uint8_t ENCODER_TYPE_UNDEFINED  = 255;
static constexpr uint8_t ENCODER_INIT_INCOMPLETE = 254;
static constexpr uint8_t ENCODER_ID_UNDEFINED    = 255;
static constexpr uint8_t ENCODER_EVENT_ROTATION  = 0;
static constexpr uint8_t ENCODER_EVENT_BUTTON    = 1;
static constexpr uint8_t ENCODER_EVENT_NONE      = 255;
static constexpr uint8_t ENCODER_RELEASE         = 0;
static constexpr uint8_t ENCODER_PRESSED         = 1;

struct EncoderPeripherals; /* BSP or ASP implementation */
struct Encoder;

typedef void (*Callback_2_ENCODERPTR_UINT8)(Encoder*, uint8_t);

typedef struct Encoder {
    EncoderPeripherals*         peripherals    = nullptr;
    uint8_t                     device_type    = ENCODER_TYPE_UNDEFINED;
    uint8_t                     device_id      = ENCODER_ID_UNDEFINED;
    Callback_2_ENCODERPTR_UINT8 callback       = nullptr;
    bool                        started        = false;
    bool                        button_pressed = false;
    int32_t                     rotation       = 0;
} Encoder;

DEFINE_ARRAYLIST(Encoder*, EncoderPtr)

WEAK void encoder_event(Encoder* encoder, uint8_t event);

Encoder*              encoder_create(uint8_t device_type);
void                  encoder_delete(Encoder* encoder);
// int32_t               encoder_get_rotation(const Encoder* encoder); // implemented as BSP
void                  encoder_start(Encoder* encoder);              // implemented as BSP
void                  encoder_stop(Encoder* encoder);               // implemented as BSP
void                  encoder_register_listener(Encoder* encoder);
ArrayList_EncoderPtr* encoder_get_listeners();

bool encoder_init_BSP(Encoder* encoder);
void encoder_deinit_BSP(Encoder* encoder);
bool encoder_init_peripherals_BSP(Encoder* encoder);
void encoder_deinit_peripherals_BSP(Encoder* encoder);

#ifdef __cplusplus
}
#endif
