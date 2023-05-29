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

#include "KlangstromDefinesArduino.h"

#if KLST_ARCH == KLST_ARCH_MCU
#include "KlangstromCardBSP_STM32.h"
klangstrom::KlangstromCard *CardPtr = new klangstrom::KlangstromCardBSP_STM32();
#elif KLST_ARCH == KLST_ARCH_CPU
#include "KlangstromCardBSP_SDL.h"
klangstrom::KlangstromCard *CardPtr = new klangstrom::KlangstromCardBSP_SDL();
#else
#warning "KLST_ARCH not defined"
#endif
