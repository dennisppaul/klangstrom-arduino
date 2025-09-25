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
#ifdef KLST_ARCH_IS_EMU

#include "stm32_hal_callbacks.h"

#ifdef __cplusplus
extern "C" {
#endif

WEAK void HAL_GPIO_EXTI_Callback(const uint16_t GPIO_Pin) { (void) GPIO_Pin; }
WEAK void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) { (void) htim; }

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
