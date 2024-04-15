#include "Klangstrom.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("------");
    Serial.println("Serial");
    Serial.println("------");

    LED(LED_00, LED_ON);
    LED(LED_01, LED_OFF);
    beats_per_minute(120);
}

void beat(uint32_t beat) {
    LED(LED_00, LED_TOGGLE);
    uint8_t data[] = "hello world\n\r";
    data_transmit(SERIAL_00, data, 13);
}

void loop() {}

void data_receive(const uint8_t receiver, uint8_t* data, uint8_t length) {
    if (receiver == SERIAL_01) {
        for (int i = 0; i < length; i++) {
            Serial.print((char)data[i]);
            LED(LED_01, LED_TOGGLE);
        }
    }
}
