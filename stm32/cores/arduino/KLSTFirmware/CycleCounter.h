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

#ifndef _CycleCounter_h_
#define _CycleCounter_h_

#define ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

void klst_enable_cycle_counter() {
    if (ARM_CM_DWT_CTRL != 0) {   // See if DWT is available
        ARM_CM_DEMCR |= 1 << 24;  // Set bit 24
        ARM_CM_DWT_CYCCNT = 0;
        ARM_CM_DWT_CTRL |= 1 << 0;  // Set bit 0
    }
}

uint32_t klst_get_cycles() {
    return ARM_CM_DWT_CYCCNT;
}

void klst_reset_cycles() {
    ARM_CM_DWT_CYCCNT = 0;
}

float klst_cyclesToMicros(uint32_t pCycles) {
    return pCycles / (float)(SystemCoreClock / 1000000);
}

#endif  // _CycleCounter_h_
