/*
* Klangstrom
*
* This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static const int8_t KLST_LED_ALL = -1;

typedef struct LED {
    float intensity;
} LED;

void  led_init();
void  led_on(const int id);
void  led_off(const int id);
void  led_toggle(const int id);
void  led_set(const int id, float intensity);
float led_get(const int id);
LED*  led_data();

void    led_init_BSP();
uint8_t led_total_BSP();
void    led_set_BSP(const int id, float intensity);

#ifdef __cplusplus
}
#endif