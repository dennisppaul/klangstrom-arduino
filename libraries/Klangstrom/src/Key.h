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

static constexpr uint8_t KEY_LEFT            = 0;
static constexpr uint8_t KEY_RIGHT           = 1;
static constexpr uint8_t KEY_00              = KEY_LEFT;
static constexpr uint8_t KEY_01              = KEY_RIGHT;
static constexpr uint8_t KEY_TYPE_CUSTOM     = 127;
static constexpr uint8_t KEY_TYPE_UNDEFINED  = 255;
static constexpr uint8_t KEY_INIT_INCOMPLETE = 254;
static constexpr uint8_t KEY_ID_UNDEFINED    = 255;
static constexpr uint8_t KEY_RELEASE         = 0;
static constexpr uint8_t KEY_PRESSED         = 1;

struct KeyPeripherals; /* BSP or ASP implementation */
struct Key;

typedef void (*Callback_1_MECHANICALKEYPTR_C)(const Key*);

typedef struct Key {
    KeyPeripherals*               peripherals = nullptr;
    uint8_t                       device_type = KEY_TYPE_UNDEFINED;
    uint8_t                       device_id   = KEY_ID_UNDEFINED;
    Callback_1_MECHANICALKEYPTR_C callback    = nullptr;
    bool                          pressed     = false;
} Key;

DEFINE_ARRAYLIST(Key*, KeyPtr)

WEAK void key_event(const Key* key); // TODO not sure if it is good to make this const
                                     //     i.e might be confusing or will lead to implementation errors.
                                     //     if this remains const it should be consistent with the other callback functions.

void              key_init(uint8_t number_of_keys = KLST_DEFAULT_NUMBER_OF_KEYS);
Key*              key_create(uint8_t device_type);
void              key_delete(Key* key);
void              key_register_listener(Key* key);
ArrayList_KeyPtr* key_get_listeners();

bool key_init_BSP(Key* key);
// void key_deinit_BSP(Key* key); // TODO maybe remove this.
bool key_init_peripherals_BSP(Key* key);
void key_deinit_peripherals_BSP(Key* key);
void key_callback_BSP(uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif
