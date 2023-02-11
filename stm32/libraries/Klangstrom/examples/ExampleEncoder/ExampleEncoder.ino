#include "Klangstrom.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("--- example encoder ---");

    led(LED_00, LED_ON);
    led(LED_01, LED_ON);
}

void loop() {}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
}

void event_receive(const EVENT_TYPE event, const void* data) {
    switch (event) {
        case EVENT_ENCODER_BUTTON_PRESSED:
            led(LED_01, LED_TOGGLE);
            Serial.println("pressed");
            break;
        case EVENT_ENCODER_BUTTON_RELEASED:
            led(LED_00, LED_TOGGLE);
            Serial.println("released");
            break;
        case EVENT_ENCODER_ROTATED:
            led(LED_02, LED_TOGGLE);
            Serial.println("rotated");
            break;
    }
}
