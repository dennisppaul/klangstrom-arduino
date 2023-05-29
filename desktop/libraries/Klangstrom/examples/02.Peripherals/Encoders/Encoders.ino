#include "Klangstrom.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("--------");
    Serial.println("Encoders");
    Serial.println("--------");
    register_encoder_rotated(encoder_rotated);
    register_encoder_pressed(encoder_pressed);
    register_encoder_released(encoder_released);
    register_encoder_event_receive(event_receive);
}

void loop() {}

void encoder_rotated(const uint8_t index, const int16_t ticks, const int16_t delta) {
    LED(LED_00, LED_TOGGLE);
    Serial.print("encoder rotated : ");
    Serial.print(" index(");
    Serial.print(index);
    Serial.print(") ticks(");
    Serial.print(ticks);
    Serial.print(") delta(");
    Serial.print(delta);
    Serial.println(")");
}

void encoder_pressed(const uint8_t index) {
    LED(LED_01, LED_ON);
    Serial.print("encoder pressed : ");
    Serial.print(" index(");
    Serial.print(index);
    Serial.println(")");
}

void encoder_released(const uint8_t index) {
    LED(LED_01, LED_OFF);
    Serial.print("encoder released: ");
    Serial.print(" index(");
    Serial.print(index);
    Serial.println(")");
}

void event_receive(const EVENT_TYPE event, const void* data) {
    if (event == EVENT_ENCODER) {
        Serial.print("encoder event   : ");
        Serial.print(" index(");
        Serial.print((int)encoder_event(data).index);
        Serial.print(") ticks(");
        Serial.print(encoder_event(data).ticks);
        Serial.print(") delta(");
        Serial.print(encoder_event(data).delta);
        Serial.print(") button(");
        Serial.print(encoder_event(data).button);
        Serial.print(")");
        Serial.println();
    }
}
