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

#include "KlangstromEnvironment.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

#include <stdint.h>
#include "SystemUID.h"

#ifdef __cplusplus
extern "C" {
#endif

/* UIDs */

#define KLST_NUM_OF_U_ID (1 + 10 + 5 + 10 + 10) // NO BOARD + 10 TINY + 5 CORE + 10 SHEEP + 10 PANDA
static constexpr int16_t KLST_NO_ID                     = -1;
static const uint32_t    KLST_U_ID[KLST_NUM_OF_U_ID][3] = {
    {0x00000000, 0x00000000, 0x00000000}, // NO BOARD
    {0x0040001D, 0x0200C000, 0x445243EC}, // KLST_TINY_01
    {0x003D0010, 0x0200C000, 0x445243EC}, // KLST_TINY_02
    {0x0040000E, 0x0200C000, 0x445243EC}, // KLST_TINY_03
    {0x00000000, 0x00000000, 0x00000000}, // KLST_TINY_04*
    {0x003F000D, 0x0200C000, 0x445243EC}, // KLST_TINY_05
    {0x00000000, 0x00000000, 0x00000000}, // KLST_TINY_06*
    {0x007F6999, 0x02762964, 0x44C7AD50}, // KLST_TINY_07
    {0x0025003E, 0x0200C000, 0x445243EC}, // KLST_TINY_08
    {0x0040001C, 0x0200C000, 0x445243EC}, // KLST_TINY_09
    {0x003E000A, 0x0200C000, 0x445243EC}, // KLST_TINY_10
    {0x00190036, 0xFFFFFFFF, 0x0000306A}, // KLST_CORE_11
    {0x001C0030, 0xFFFFFFFF, 0x00003051}, // KLST_CORE_12
    {0x00190031, 0xFFFFFFFF, 0x00002FC2}, // KLST_CORE_13
    {0x0019002C, 0xFFFFFFFF, 0x00002F9A}, // KLST_CORE_14
    {0x001A0025, 0xFFFFFFFF, 0x0000302E}, // KLST_CORE_15
    {0x001B003E, 0xFFFFFFFF, 0x00003037}, // KLST_SHEEP_16
    {0x00470024, 0xFFFFFFFF, 0x00003029}, // KLST_SHEEP_17
    {0x001A003F, 0xFFFFFFFF, 0x00002F81}, // KLST_SHEEP_18
    {0x00300047, 0xFFFFFFFF, 0x0000302A}, // KLST_SHEEP_19
    {0x0046001D, 0xFFFFFFFF, 0x00003093}, // KLST_SHEEP_20
    {0x00300045, 0xFFFFFFFF, 0x00003013}, // KLST_SHEEP_21 (90°+NO_USBMICROB)
    {0x002E0028, 0xFFFFFFFF, 0x00003057}, // KLST_SHEEP_22 (90°)
    {0x002E0022, 0xFFFFFFFF, 0x00002FE8}, // KLST_SHEEP_23
    {0x001A003D, 0xFFFFFFFF, 0x00003031}, // KLST_SHEEP_24
    {0x00300040, 0xFFFFFFFF, 0x00003073}, // KLST_SHEEP_25 (BUTTONS)
    {0x0021003E, 0xFFFFFFFF, 0x00000304}, // KLST_PANDA_26
    {0x000A0032, 0xFFFFFFFF, 0x000002FF}, // KLST_PANDA_27
    {0x00210038, 0xFFFFFFFF, 0x00000309}, // KLST_PANDA_28
    {0x00210039, 0xFFFFFFFF, 0x000002F8}, // KLST_PANDA_29
    {0x0021001E, 0xFFFFFFFF, 0x00000305}, // KLST_PANDA_30
    {0x00050033, 0xFFFFFFFF, 0x00000309}, // KLST_PANDA_31
    {0x00210032, 0xFFFFFFFF, 0x00000301}, // KLST_PANDA_32
    {0x00050032, 0xFFFFFFFF, 0x00000303}, // KLST_PANDA_33
    {0x000A0033, 0xFFFFFFFF, 0x000002FB}, // KLST_PANDA_34
    {0x00140043, 0xFFFFFFFF, 0x000002FE}, // KLST_PANDA_35
};

int16_t system_get_UID_index() {
    for (uint8_t i = 0; i < KLST_NUM_OF_U_ID; ++i) {
        if (system_check_UID(KLST_U_ID[i])) {
            return i;
        }
    }
    return KLST_NO_ID;
}

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

bool system_check_UID(const uint32_t UID[]) {
    return UID[0] == system_get_UID(0) &&
           UID[1] == system_get_UID(1) &&
           UID[2] == system_get_UID(2);
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32 || KLST_CATERPILLAR_STM32
