//
// ToolPrintUID
//

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println(klangstrom::U_ID());
    delay(1000);
}
