//
// ExampleToolPrintUID
//

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println(klangstrom::U_ID());
    if (klangstrom::ID() != KLST_NO_ID) {
        Serial.print("KLST ID: ");
        Serial.println(klangstrom::ID());
    }
    delay(1000);
}
