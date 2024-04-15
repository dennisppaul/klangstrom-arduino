#include "Klangstrom.h"

void print_button_states() {
    Serial.print("BUTTON STATE ENCODER_00       : ");
    Serial.println(klangstrom::button_state(ENCODER_00) ? "PRESSED" : "RELEASED");
    Serial.print("BUTTON STATE ENCODER_01       : ");
    Serial.println(klangstrom::button_state(ENCODER_01) ? "PRESSED" : "RELEASED");
    Serial.print("BUTTON STATE ENCODER_02       : ");
    Serial.println(klangstrom::button_state(ENCODER_02) ? "PRESSED" : "RELEASED");
}

void setup() {
    Serial.begin(115200);
    Serial.println("-----------------");
    Serial.println("QueryButtonStates");
    Serial.println("-----------------");

    Serial.println("in `setup()`: ");
    Serial.println("---");
    print_button_states();
    Serial.println("---");
    Serial.println("in `loop()`: ");
}

void loop() {
    Serial.println("---");
    print_button_states();
    delay(1000);
}
