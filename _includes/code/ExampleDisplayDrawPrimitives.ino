#include "Klangstrom.h"
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
KlangstromDisplay    &Display = KlangstromDisplay::create();

void draw_primitves() {
    uint16_t x = 10;
    uint16_t y = 10;

    Display.textFont(&Font_7x9);
    Display.text(x, y, "DRAW PRIMITIVES");

    /* fonts can be created with the font generator tool */

    Display.textFont(&Font_5x8);
    Display.text(10, y += 20, "FONT 5x8 ( Hamburgefonts )");

    Display.textFont(&Font_7x9);
    Display.text(10, y += 10, "FONT 7x9 ( Hamburgefonts )");

    Display.textFont(&Font_7x10);
    Display.text(10, y += 10, "FONT 7x10 ( Hamburgefonts )");
    Display.text_scaled(10, y += 40, 4, "123");

    /* fonts can be created with the image generator tool */

    Display.image(10, y += 20, KLST__ICON__128x128_WIDTH, KLST__ICON__128x128_HEIGHT, data_KLST__ICON__128x128);

    /* draw a rect on screen edge */

    Display.rect(0, 0, Display.width(), Display.height(), false);
    Display.rect(1, 1, Display.width() - 2, Display.height() - 2, false);

    /* draw horizontal and vertical lines */

    y += KLST__ICON__128x128_HEIGHT + 10;
    uint16_t mShapeSize = 32;

    Display.line(x, y, x, y + mShapeSize);
    Display.line(x + mShapeSize, y, x + mShapeSize, y + mShapeSize);

    Display.line(x, y, x + mShapeSize, y);
    Display.line(x, y + mShapeSize, x + mShapeSize, y + mShapeSize);

    /* draw arbitrary lines */

    Display.line(x, y, x + mShapeSize, y + mShapeSize);
    Display.line(x + mShapeSize, y, x, y + mShapeSize);

    /* draw filled rects in RGB HEX colors + store/restore current color */

    uint16_t mColor = Display.get_color_16i();
    uint16_t xx     = x;
    x += 32 + 8;
    Display.color_32i(0xFF0000);
    Display.rect(x, y, mShapeSize, mShapeSize, true);
    x += 32 + 8;
    Display.color_32i(0x00FF00);
    Display.rect(x, y, mShapeSize, mShapeSize, true);
    x += 32 + 8;
    Display.color_32i(0x0000FF);
    Display.rect(x, y, mShapeSize, mShapeSize, true);
    x += 32 + 8;
    Display.color_16i(mColor);
    Display.rect(x, y, mShapeSize, mShapeSize, true);
    x = xx;
    y += mShapeSize + 8;

    /* set pixel */
    for (uint16_t i = 0; i < mShapeSize * 4; i++) {
        uint16_t mX = random(mShapeSize * 6);
        uint16_t mY = random(mShapeSize);
        Display.pixel(x + mX, y + mY);
    }
    y += mShapeSize + 8;

    /* display duration of last screen clear */
    Display.textFont(&Font_7x9);
    Display.text(x, y, fClearScreenDurationSec.c_str());
}

void setup() {
    Display.begin();
    klst_enable_cycle_counter();
}

void draw_screen(uint8_t r0, uint8_t g0, uint8_t b0, uint8_t r1, uint8_t g1, uint8_t b1) {
    Display.background(r0, g0, b0);
    Display.clear();
    Display.color(r1, g1, b1);
    draw_primitves();
}

void loop() {
    draw_screen(0, 0, 0, 255, 255, 255);
    delay(fWait);
    draw_screen(255, 255, 255, 0, 0, 0);
    delay(fWait);
    draw_screen(255, 127, 0, 0, 127, 255);
    delay(fWait);

    const uint32_t start = klst_get_cycles();
    for (uint16_t i = 0; i < 20; i++) {
        Display.background(0, 0, 0);
        Display.clear();
        Display.background(255, 0, 0);
        Display.clear();
        Display.background(0, 255, 0);
        Display.clear();
        Display.background(0, 0, 255);
        Display.clear();
        Display.background(255, 255, 255);
        Display.clear();
    }
    const uint32_t delta    = klst_get_cycles() - start;
    fClearScreenDurationSec = String(klst_cyclesToMicros(delta) / 1000000);
    fClearScreenDurationSec += " sec to clear screen 100x";
}
