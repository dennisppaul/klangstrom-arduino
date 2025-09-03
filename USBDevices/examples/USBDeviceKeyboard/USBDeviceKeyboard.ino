/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "USBDevices.h"

void setup() {
    system_init();
    usb_devices_init();

    console_timestamp(false);
    console_println("USBDevicesKeyboard");
}

uint32_t counter = 0;

void loop() {
    counter++;
    if ((counter % (65536 * 128) ) == 0) {
        console_println(".");
    }
}
