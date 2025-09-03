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

#include "KlangstromEnvironment.h"
#include "KlangstromConfiguration.h"

/* include client configuration, if present */

#if __has_include("PeripheralConfiguration.h")

#warning "Klangstrom peripherals are configured by client via external file."
#include "KlangstromPeripheralConfiguration.h"
#define KLST_CLIENT_PERIPHERAL_CONFIGURATION

#endif // KlangstromPeripheralConfiguration.h

/* include architecture / plattform specific implementation + configuration */

#ifdef KLST_ARCH_IS_STM32
#include "PeripheralConfiguration_STM32.h"
#elif defined(KLST_ARCH_IS_EMU)
#include "PeripheralConfiguration_EMU.h"
#else
#warning "no implementation for Klangstrom found ( this might be intentional )"
#endif
