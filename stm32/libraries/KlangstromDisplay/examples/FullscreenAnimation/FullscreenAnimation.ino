#include "Klangstrom.h"
#include "KlangstromDisplay.h"

using namespace klangstrom;

uint16_t           x          = 10;
uint16_t           y          = 10;
uint16_t           fShapeSize = 32;
volatile bool      fUpdate    = false;
KlangstromDisplay &Display    = KlangstromDisplay::create();

void draw_primitves() {
    Display.rect(x, y, fShapeSize, fShapeSize, true);
}

void setup() {
    Serial.begin(115200);
    Serial.println("-------------------");
    Serial.println("FullscreenAnimation");
    Serial.println("-------------------");

    Display.begin();
    beats_per_minute(900);  // == 15 Hz
}

void beat(uint32_t pBeat) {
    static const uint8_t mSpeed = 3;
    x += random(mSpeed * 2) - mSpeed;
    x %= Display.width();
    y += random(mSpeed * 2) - mSpeed;
    y %= Display.height();
    fUpdate = true; /* schedule display for update */
}

void draw_screen(uint8_t r0, uint8_t g0, uint8_t b0, uint8_t r1, uint8_t g1, uint8_t b1) {
    Display.background(r0, g0, b0);
    Display.clear();
    Display.color(r1, g1, b1);
    draw_primitves();
}

void loop() {
    if (fUpdate) { /* update display in loop */
        fUpdate = false;
        draw_screen(0, 0, 0, 255, 255, 255);
    }
}
