#include <EEPROM.h>

#include "Klangstrom.h"

using namespace klangstrom;

/* define struct to hold presets */
struct Presets {
    float frequency = 440.0;
    float amplitude = 0.4;
};

Presets mPresets;

void setup() {
    Serial.begin(115200);
    Serial.println("-------");
    Serial.println("Presets");
    Serial.println("-------");

    register_encoder_pressed(encoder_pressed);

    /* hold button 00 at start up to reset presets */
    if (button_state(ENCODER_00)) {
        Serial.print("reset presets: ");
        EEPROM.put(0, mPresets);
    } else {
        Serial.print("read presets: ");
        EEPROM.get(0, mPresets);
    }
    print_presets();
}

void loop() {}

void print_presets() {
    Serial.print(mPresets.amplitude);
    Serial.print(", ");
    Serial.print(mPresets.frequency);
    Serial.println();
}

void encoder_pressed(const uint8_t index) {
    if (index == ENCODER_00) {
        Serial.print("write presets 0: ");
        mPresets.amplitude = 0.5;
        mPresets.frequency = 440.0;
        EEPROM.put(0, mPresets);
    } else if (index == ENCODER_01) {
        Serial.print("write presets 1: ");
        mPresets.amplitude = 0.6;
        mPresets.frequency = 220.0;
        EEPROM.put(0, mPresets);
    }
    print_presets();
}
