#include "Klangstrom.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayFont_5x8.h"
#include "KlangstromDisplayFont_7x10.h"
#include "KlangstromDisplayFont_7x9.h"

using namespace klangstrom;

static const uint32_t fWait   = 100;
KlangstromDisplay&    Display = KlangstromDisplay::create();
uint8_t               fNumber = 0;

void draw_primitves() {
    uint16_t x = 10;
    uint16_t y = 10;

    Display.textFont(&Font_5x8);
    Display.text(x, y, "Friends");
    y += 8;

    Display.textFont(&Font_7x9);
    Display.text(x, y, "with");
    y += 9;

    Display.textFont(&Font_7x10);
    Display.text(x, y, "Technology");
    y += 10;

    Display.text_scaled(x, y, 4, "23");
    y += 10 * 4;

    Display.text_scaled(x, y, 8, "=");
    y += 10 * 8;

    Display.textFont(&Font_7x9);
    fNumber++;
    fNumber %= 100;
    String s = String(fNumber);
    Display.text_scaled(x, y, 12, s.c_str());
}

void setup() {
    Display.begin();
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
}
