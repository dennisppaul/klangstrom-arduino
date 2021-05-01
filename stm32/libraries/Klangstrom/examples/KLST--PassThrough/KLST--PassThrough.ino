//
//  KLST--PassThrough
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "Klangstrom.h"

void setup() {
  pinMode(PB5, OUTPUT);
  digitalWrite(PB5, HIGH);
  Serial.begin(115200);
  Serial.println("KLST_TINY");
}

void loop() {
  digitalWrite(PB5, HIGH);
  delay(1000);
  digitalWrite(PB5, LOW);
  delay(1000);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
  for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
    pOutputRight[i] = pInputRight[i];
    pOutputLeft[i] = pInputLeft[i];
  }
}
