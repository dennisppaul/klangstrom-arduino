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

#ifndef _KLST_SDL_ADAPTER_H_
#define _KLST_SDL_ADAPTER_H_

#include <stdint.h>
#include "KLST_SDL-constants.h"

#ifdef __cplusplus
extern "C" {
#endif
void     delay(uint32_t pMS);
int      digitalRead(uint32_t pPin);
void     digitalWrite(uint32_t pPin, uint32_t pValue);
void     pinMode(uint32_t pPin, uint32_t pMode);
void     interrupts();
void     noInterrupts();
uint32_t micros();
uint32_t millis();
#ifdef __cplusplus
}
#endif

#endif /* _KLST_SDL_ADAPTER_H_ */
