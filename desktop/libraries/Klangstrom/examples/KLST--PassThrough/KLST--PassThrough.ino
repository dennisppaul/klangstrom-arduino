//
//  KLST--PassThrough
//  Klangstrom – a node+text-based synthesizer library
//
//
//

void setup() {}

void loop() {
    klst::led(LED_00, true);
    delay(1000);
    klst::led(LED_00, false);
    delay(1000);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        pOutputRight[i] = pInputRight[i];
        pOutputLeft[i] = pInputLeft[i];
    }
}
