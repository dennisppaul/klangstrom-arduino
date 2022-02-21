//
// ExamplePassThrough
//

using namespace klangstrom;

void setup() {
    option(KLST_OPTION_AUDIO_INPUT, KLST_MIC);
}

void loop() {
    led(LED_00, true);
    delay(1000);
    led(LED_00, false);
    delay(1000);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
        pOutputRight[i] = pInputRight[i];
        pOutputLeft[i] = pInputLeft[i];
    }
}
