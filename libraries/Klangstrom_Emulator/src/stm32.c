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

#include "stm32.h"

static GPIO_TypeDef GPIOA_storage = { .ID = 0x000 };
GPIO_TypeDef *GPIOA = &GPIOA_storage;
static GPIO_TypeDef GPIOB_storage = { .ID = 0x001 };
GPIO_TypeDef *GPIOB = &GPIOB_storage;
static GPIO_TypeDef GPIOC_storage = { .ID = 0x002 };
GPIO_TypeDef *GPIOC = &GPIOC_storage;
static GPIO_TypeDef GPIOD_storage = { .ID = 0x003 };
GPIO_TypeDef *GPIOD = &GPIOD_storage;
static GPIO_TypeDef GPIOE_storage = { .ID = 0x004 };
GPIO_TypeDef *GPIOE = &GPIOE_storage;
static GPIO_TypeDef GPIOF_storage = { .ID = 0x005 };
GPIO_TypeDef *GPIOF = &GPIOF_storage;
static GPIO_TypeDef GPIOG_storage = { .ID = 0x006 };
GPIO_TypeDef *GPIOG = &GPIOG_storage;
static GPIO_TypeDef GPIOH_storage = { .ID = 0x007 };
GPIO_TypeDef *GPIOH = &GPIOH_storage;
