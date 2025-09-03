/*
* Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

// TODO this can be made much more platform specific
#include <stdint.h>
#include "stm32.h"

#include "KLST_EMU_PANDA_Peripheral_Defines.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KeyPeripherals {
    uint16_t      gpio_pin;
    GPIO_TypeDef* gpio_port;
} KeyPeripherals;

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
