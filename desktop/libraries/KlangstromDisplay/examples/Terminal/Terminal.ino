#include "Klangstrom.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayFont_5x8.h"
#include "KlangstromDisplayTerminal.h"

using namespace klangstrom;

KlangstromDisplay&         Display = KlangstromDisplay::create();
KlangstromDisplayTerminal* mTerminal;

void setup() {
    Serial.begin(115200);
    Serial.println("--- KLST TERMINAL ---");

    Display.begin();
    Display.background(0, 0, 0);
    Display.clear();
    Display.color(255, 255, 255);

    mTerminal         = new KlangstromDisplayTerminal(&Display, &Font_5x8);
    mTerminal->width  = 44;  // 240 / 5 = 48
    mTerminal->height = 38;  // 320 / 8 = 40
    mTerminal->x      = 5 * 2;
    mTerminal->y      = 8;
    mTerminal->background_color.set(0, 0, 0);
    mTerminal->foreground_color.set(255, 255, 255);
    mTerminal->println("--- KLST TERMINAL ---");
}

void loop() {
}

void serialEvent() {
    while (Serial.available()) {
        int mRead = Serial.read();
        mTerminal->print((char)mRead);
        if (mRead == 'x') {
            mTerminal->clear();
        }
    }
}
