/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"

int counter = 0;

void setup() {
    system_init();

    console_timestamp(false);
    console_print("Hello,");
    console_println("World!");

    console_error("error message   : %i", 23);
    console_status("status message  : %i", 42);
}

void loop() {
    console_println("...");

    /* clear screen evert 16 iterations */
    counter++;
    if (counter % 16 == 0) {
        console_clear();
        console_set_color_red();
        console_println("counter");
        console_reset_color();
        console_set_color_green();
        console_println("%d", counter);
        console_reset_color();
    }

    delay(1000);
}
