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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#include "KlangstromEmulator.h"
#include "LED.h"

class DrawableLEDs : public Drawable {
public:
    DrawableLEDs(LED* leds) : mLEDs(leds) {}

    void draw(PGraphics* g) override {
        g->pushMatrix();
        g->translate(25, 80);
        const float mRadius  = 40;
        const float y        = 0;
        const float x_offset = mRadius / 2;
        for (uint8_t i = 0; i < led_total_BSP(); ++i) {
            const float mIntensity = mLEDs[i].intensity;
            const float x          = x_offset + i * mRadius * 1.5f;
            g->fill(1, mIntensity);
            g->noStroke();
            g->ellipse(x, y, mRadius, mRadius);
            g->noFill();
            g->stroke(1.0f);
            g->ellipse(x, y, mRadius, mRadius);
        }
        g->popMatrix();
    }

private:
    LED* mLEDs;
};

#ifdef __cplusplus
extern "C" {
#endif

void led_init_BSP() {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        KlangstromEmulator::instance()->register_drawable(new DrawableLEDs(led_data()));
    }
}

uint8_t led_total_BSP() {
#if defined(GENERIC_EMU)
    return 0;
#elif defined(KLST_CORE_EMU)
    return 3;
#elif defined(KLST_TINY_EMU)
    return 2;
#elif defined(KLST_SHEEP_EMU)
    return 16;
#elif defined(KLST_PANDA_EMU)
    return 2;
#elif defined(KLST_CATERPILLAR_EMU)
    return 2;
#else
    return 0;
#endif
}

void led_set_BSP(const int id, float intensity) {
    (void) id;
    (void) intensity;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
