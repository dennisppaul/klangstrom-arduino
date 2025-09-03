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

#include "Klangstrom.h"
#ifdef KLST_ARCH_IS_STM32
#ifndef KLST_CLIENT_PERIPHERAL_CONFIGURATION

#if defined(KLST_PANDA_STM32)
#include "PeripheralConfiguration_KLST_PANDA_STM32.h"
#elif defined(KLST_CATERPILLAR_STM32)
#include "PeripheralConfiguration_KLST_CATERPILLAR_STM32.h"
#endif // KLST_PANDA_STM32

#else // KLST_CLIENT_PERIPHERAL_CONFIGURATION

#warning "Klangstrom peripherals are configured by client via external file."

#endif // KLST_CLIENT_PERIPHERAL_CONFIGURATION
#endif // KLST_ARCH_IS_STM32
