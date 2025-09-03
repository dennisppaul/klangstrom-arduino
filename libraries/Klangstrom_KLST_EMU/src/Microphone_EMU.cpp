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

#include "Klangstrom.h"
#ifdef KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC
#ifdef KLST_ARCH_IS_EMU

#include <string>

#include "Microphone.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

void microphone_init_BSP(Microphone* microphone) {
    console_error("Microphone: not implemented");
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
#endif // KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC