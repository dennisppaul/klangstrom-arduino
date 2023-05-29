#include "Klangstrom.h"

void print_uid() {
    if (klangstrom::ID() != KLST_NO_ID) {
        Serial.print("KLST ID (ID)    : ");
        Serial.println(klangstrom::ID());
    }
    Serial.print("UNIQUE ID (U_ID): ");
    Serial.println(klangstrom::U_ID());
}

void setup() {
    Serial.begin(115200);
    Serial.println("--------");
    Serial.println("PrintUID");
    Serial.println("--------");
}

void loop() {
    print_uid();
    Serial.println("---");
    delay(3000);
}
