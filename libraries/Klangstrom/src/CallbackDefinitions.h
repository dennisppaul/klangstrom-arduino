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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct AudioBlock;

typedef void (*Callback_0_VOID)();
typedef void (*Callback_1_I8)(int8_t);
typedef void (*Callback_1_UI8)(uint8_t);
typedef void (*Callback_1_UI16)(uint16_t);

typedef void (*Callback_2_UI8)(uint8_t, uint8_t);
typedef void (*Callback_2_UI8_UI16)(uint8_t, uint16_t);
typedef void (*Callback_2_UI8_VOIDPTR)(const uint8_t, const void*);

typedef void (*Callback_3_UI8)(uint8_t, uint8_t, uint8_t);
typedef void (*Callback_3_FLOATPTRPTR_FLOATPTRPTR_UI16)(float**, float**, uint16_t);
typedef void (*Callback_1_AUDIOBLOCKPTR)(AudioBlock*);

#ifdef __cplusplus
}
#endif