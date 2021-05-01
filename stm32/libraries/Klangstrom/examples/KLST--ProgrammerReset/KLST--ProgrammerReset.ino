void setup() {
  Serial.begin(115200);
  klst_led(LED_00, true);  
  klst_led(LED_01, false);  
  klst_led(LED_02, true);  
}

void loop() {
  if (klst_button_state(KLST_BUTTON_PROGRAMMER)) {
    Serial.println("KLST_jump_to_bootloader()");
    KLST_jump_to_bootloader();
  }
  klst_led_toggle(LED_00);  
  klst_led_toggle(LED_01);  
  klst_led_toggle(LED_02);  
  delay(250);
}
