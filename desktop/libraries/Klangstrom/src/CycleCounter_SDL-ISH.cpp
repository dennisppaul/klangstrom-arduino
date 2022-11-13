/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
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

#include "KlangstromDefines.hpp"

#if (KLST_ARCH==KLST_ARCH_DESKTOP)

// @todo(implement cycle counter)

#include "Arduino.h"

void klst_enable_cycle_counter() {
}

uint32_t klst_get_cycles() {
    return 0;
}

void klst_reset_cycles() {
}

float klst_cyclesToMicros(uint32_t pCycles) {
    return 0;
}

#endif // (KLST_ARCH==KLST_ARCH_DESKTOP)
