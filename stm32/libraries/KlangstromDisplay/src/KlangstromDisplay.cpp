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

#include "KlangstromDisplay.h"

#include "KlangstromDefines.hpp"

#ifndef KLST_ARCH_DESKTOP
#warning KLST_ARCH_DESKTOP not defined
#endif
/* ^^^ TODO remove this ASAP */

#if (KLST_ARCH == KLST_ARCH_DESKTOP)
#include "KlangstromDisplay_SDL-BSP.h"
// #warning will create display for KLST_ARCH_DESKTOP
#elif (KLST_ARCH == KLST_ARCH_MCU)
#include "KlangstromDisplay_KLST-BSP.h"
// #warning will create display for KLST_ARCH_MCU
#endif

klangstrom::KlangstromDisplay* klangstrom::KlangstromDisplay::create_ptr() {
#if (KLST_ARCH == KLST_ARCH_DESKTOP)
    return new KlangstromDisplay_SDL_BSP();
#elif (KLST_ARCH == KLST_ARCH_MCU)
    return new KlangstromDisplay_KLST_BSP();
#else
#warning will not create a display
    return null;
#endif
}
