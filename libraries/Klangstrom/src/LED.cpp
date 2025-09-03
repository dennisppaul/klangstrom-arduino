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

#include <cstdint>
#include <cmath>

#include "LED.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

static LED* leds;

void led_init() {
    leds = new LED[led_total_BSP()];
    led_init_BSP();
}

void led_on(const int id) {
    led_set(id, 1);
}

void led_off(const int id) {
    led_set(id, 0);
}

void led_toggle(const int id) {
    if (id < 0) {
        for (uint8_t i = 0; i < led_total_BSP(); ++i) {
            if (leds[i].intensity > 0) {
                leds[i].intensity = 0;
            } else {
                leds[i].intensity = 1;
            }
            led_set_BSP(i, leds[i].intensity);
        }
    } else if (id < led_total_BSP()) {
        if (leds[id].intensity > 0) {
            leds[id].intensity = 0;
        } else {
            leds[id].intensity = 1;
        }
        led_set_BSP(id, leds[id].intensity);
    }
}

void led_set(const int id, float intensity) {
    intensity = static_cast<float>(fmax(fmin(intensity, 1), 0));
    if (id < 0) {
        for (uint8_t i = 0; i < led_total_BSP(); ++i) {
            leds[i].intensity = intensity;
            led_set_BSP(i, intensity);
        }
    } else if (id < led_total_BSP()) {
        leds[id].intensity = intensity;
        led_set_BSP(id, intensity);
    }
}

float led_get(const int id) {
    if (id >= 0 && id < led_total_BSP()) {
        return leds[id].intensity;
    }
    return 0;
}

LED* led_data() {
    return leds;
}

#ifdef __cplusplus
}
#endif
