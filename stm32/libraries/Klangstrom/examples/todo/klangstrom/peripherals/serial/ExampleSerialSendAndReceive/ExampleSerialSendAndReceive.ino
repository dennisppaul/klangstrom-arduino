#include "Klangstrom.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("Example Serial Send And Receive");
}

void loop() {
    delay(10);
}

void data_receive(const uint8_t receiver, uint8_t* data, uint8_t length) {
    Serial.print("+++ data_receive: ");
    if (receiver == SERIAL_00) {
        LED(LED_01, LED_TOGGLE);
        Serial.print("SERIAL_00: ");
        for (int i = 0; i < length; i++) {
            Serial.print(data[i]);
            Serial.print(",");
        }
        Serial.println();
    }
    if (receiver == SERIAL_01) {
        LED(LED_14, LED_TOGGLE);
        Serial.print("SERIAL_01: ");
        for (int i = 0; i < length; i++) {
            Serial.print(data[i]);
            Serial.print(",");
        }
        Serial.println();
    }
}

void event_receive(const EVENT_TYPE event, const void* data) {
    if (event == EVENT_ENCODER_BUTTON_PRESSED) {
        if (encoder_event(data).index == ENCODER_00) {
            uint8_t mData[1] = {23};
            data_transmit(SERIAL_00, mData, 1);
            LED(LED_00, LED_TOGGLE);
            Serial.println("+++ data_transmit: SERIAL_00");
        }
        if (encoder_event(data).index == ENCODER_01) {
            uint8_t mData[1] = {42};
            data_transmit(SERIAL_01, mData, 1);
            LED(LED_15, LED_TOGGLE);
            Serial.println("+++ data_transmit: SERIAL_01");
        }
    }
}
