#include "CycleCounter.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayDrawBuffer.h"
#include "KlangstromDisplayFont_5x8.h"
#include "KlangstromDisplayFont_7x10.h"
#include "KlangstromDisplayFont_7x9.h"
#include "KlangstromDisplayImage_KLST-ICON_128x128.h"

using namespace klangstrom;

static const uint32_t fWait = 2000;
String                fClearScreenDurationSec;
uint16_t              x          = 10;
uint16_t              y          = 10;
uint16_t              mShapeSize = 32;
volatile bool         mUpdate    = false;

void draw_primitves() {
    Display.rect(x, y, mShapeSize, mShapeSize, true);
}

void setup() {
    Display.begin();
    beats_per_minute(900); // == 15 Hz
    klst_enable_cycle_counter();
}

void beat(uint32_t pBeat) {
    static const uint8_t mSpeed = 3;
    x += random(mSpeed * 2) - mSpeed;
    x %= Display.width();
    y += random(mSpeed * 2) - mSpeed;
    y %= Display.height();
    mUpdate = true;
}

void draw_screen(uint8_t r0, uint8_t g0, uint8_t b0, uint8_t r1, uint8_t g1, uint8_t b1) {
    Display.background(r0, g0, b0);
    Display.clear();
    Display.color(r1, g1, b1);
    draw_primitves();
}

void loop() {
    if (mUpdate) {
        mUpdate              = false;
        const uint32_t start = klst_get_cycles();
        draw_screen(0, 0, 0, 255, 255, 255);
        const uint32_t delta    = klst_get_cycles() - start;
        fClearScreenDurationSec = String(klst_cyclesToMicros(delta) / 1000000);
        fClearScreenDurationSec += " sec to clear screen 100x";
    }
}
