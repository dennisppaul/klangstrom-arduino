//
//  KLST--ProgrammerReset
//  Klangstrom – a node+text-based synthesizer library
//
//
//

void setup() {
    Serial.begin(115200);
    klst::led(LED_00, true);
    klst::led(LED_01, false);
    klst::led(LED_02, true);
    /*  by default the programmer button is configured to trigger
        a reset and send the board into DFU mode once pressed.
        the option below deactivates the automatic behavior. */
    klst::option(KLST_OPTION_PROGRAMMMER_BUTTON, false);
}

void loop() {
    /*  the programmer button is evaluated manually. a system reset
        is triggered with `KLST_jump_to_bootloader` */
    if (klst::button_state(KLST_BUTTON_PROGRAMMER)) {
        Serial.println("### jumping to bootloader manually");
        KLST_jump_to_bootloader();
    }
    klst::led_toggle(LED_00);
    klst::led_toggle(LED_01);
    klst::led_toggle(LED_02);
    delay(250);
}
