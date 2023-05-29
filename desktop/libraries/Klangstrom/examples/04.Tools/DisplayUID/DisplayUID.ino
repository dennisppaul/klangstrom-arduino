#include "Klangstrom.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayFont_7x10.h"

using namespace klangstrom;

KlangstromDisplay& Display = KlangstromDisplay::create();

void display_uid() {
    uint16_t x = 10;
    uint16_t y = 10;
    Display.text(x, y, "KLST ID (ID): ");
    y += 10 * 2;
    Display.text(x, y, 10, String(klangstrom::ID()));
    y += 10 * 9;
    Display.text(x, y, "UNIQUE ID (U_ID): ");
    y += 10;
    Display.text(x, y, String(klangstrom::U_ID()));
}

void setup() {
    Serial.begin(115200);
    Serial.println("----------");
    Serial.println("DisplayUID");
    Serial.println("----------");

    Display.begin();
    Display.background(0, 0, 0);
    Display.clear();
    Display.color(255, 255, 255);
    Display.textFont(&Font_7x10);
    display_uid();
}

void loop() {}
