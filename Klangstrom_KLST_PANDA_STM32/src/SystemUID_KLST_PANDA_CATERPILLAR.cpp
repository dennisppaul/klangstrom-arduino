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

#include "KlangstromEnvironment.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

#include <stdint.h>
#include "SystemUID.h"

#ifdef __cplusplus
extern "C" {
#endif

/* UIDs */

uint32_t KLST_BSP_UID_address() {
    //    RM0433 Reference manual STM32H742, STM32H743/753 and STM32H750, p3271ff
    //    - Base address   : 0x1FF1E800
    //    - address offset : 0x00 U_ID(31:00)
    //    - address offset : 0x04 U_ID(63:32)
    //    - address offset : 0x08 U_ID(95:64)
    return 0x1FF1E800;
}

unsigned long system_get_UID(const uint8_t offset) {
    static const uint32_t UID_ADDR = KLST_BSP_UID_address();
    return *(reinterpret_cast<unsigned long*>(UID_ADDR) + offset * 0x04);
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32 || KLST_CATERPILLAR_STM32
