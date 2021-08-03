//
// ExampleUSBHostMouseKeyboardMIDI
//

#include "USBHost.h"

using namespace klangstrom;

static uint32_t blink_interval_ms = 500;

void setup() {
    KLST_SERIAL_00.println("-------------");
    KLST_SERIAL_00.println("USB Host Demo");
    KLST_SERIAL_00.println("-------------");
    usb_host_init();
}

void loop() {
    usb_host_process();
    led_blinking_task();
}

void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool mLEDState = false;

    // Blink every interval ms
    if ( millis() - start_ms < blink_interval_ms) { return; }
    start_ms += blink_interval_ms;

    led(LED_01, mLEDState);
    mLEDState = 1 - mLEDState;
}

void receive_midi_note_on(uint8_t pNote, uint8_t pVelocity) {
    KLST_SERIAL_00.print("note_on .... : ");
    KLST_SERIAL_00.print(pNote);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(pVelocity);
    KLST_SERIAL_00.println();
}

void receive_midi_note_off(uint8_t pNote, uint8_t pVelocity) {
    KLST_SERIAL_00.print("note_off ... : ");
    KLST_SERIAL_00.print(pNote);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(pVelocity);
    KLST_SERIAL_00.println();
}

void receive_midi_CC(uint8_t pValue) {
    KLST_SERIAL_00.print("CC ......... : ");
    KLST_SERIAL_00.print(pValue);
    KLST_SERIAL_00.println();
}

void receive_midi_pitch_bend(int16_t pValue) {
    KLST_SERIAL_00.print("pitch bend . : ");
    KLST_SERIAL_00.print(pValue);
    KLST_SERIAL_00.println();
}

void receive_mouse(uint8_t x, uint8_t y, uint8_t* buttons) {
    KLST_SERIAL_00.print("mouse ...... : ");
    KLST_SERIAL_00.print(x);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(y);
    KLST_SERIAL_00.print(", ");
    KLST_SERIAL_00.print(buttons[0]);
    KLST_SERIAL_00.println();
}

void receive_keyboard(uint8_t* keys) {
    KLST_SERIAL_00.print("keyboard ... : ");
    KLST_SERIAL_00.print((char)(keys[0]+93));
    KLST_SERIAL_00.println();
}
