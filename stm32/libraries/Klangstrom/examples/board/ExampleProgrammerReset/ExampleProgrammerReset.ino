//
// ExampleProgrammerReset
//

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    led(LED_00, true);
    led(LED_01, false);
    led(LED_02, true);
    /*  by default the programmer button is configured to trigger
        a reset and send the board into DFU mode once pressed.
        the option below deactivates the automatic behavior. */
    option(KLST_OPTION_PROGRAMMER_BUTTON, false);
}

void loop() {
    /*  the programmer button is evaluated manually. a system reset
        can be triggered with `KLST_jump_to_bootloader` */
    if (button_state(KLST_BUTTON_PROGRAMMER)) {
        Serial.println("### jumping to bootloader manually");
        KLST_jump_to_bootloader();
    }
    led_toggle(LED_00);
    led_toggle(LED_01);
    led_toggle(LED_02);
    delay(250);
}
