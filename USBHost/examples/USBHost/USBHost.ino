/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "USBHost.h"
#include "System.h"
#include "Console.h"

void setup() {
    system_init();
    usb_host_init();
    usb_host_callback_mouse_moved(mouse_moved);

    console_timestamp(false);
    console_println("USBHost");
}

uint32_t counter = 0;

void loop() {
    counter++;
    if ((counter % (65536 * 128) ) == 0) {
        console_println(".");
    }
    usb_host_process();
}

void mouse_moved(const bool *buttons, const int8_t x, const int8_t y) {
    console_println("mouse_moved: x:%i y:%i", x, y);    
}