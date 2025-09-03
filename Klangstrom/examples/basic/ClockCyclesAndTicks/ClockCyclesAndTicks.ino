/**
 * this example demonstrates how to measure time in ticks and clock cycles.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"

void setup() {
    system_init();
}

void loop() {
    console_print("measuring clock cycles since counter was enabled: ");
    system_enable_cycle_counter(true);
    delay(1000);
    uint32_t cycles = system_get_cycles();
    system_enable_cycle_counter(false);
    console_print("%lu cycles are equal to %f ms ", cycles, system_cycles_to_micros(cycles) / 1000);
    console_println("at a clock frequency of %lu Hz, ", system_clock_frequency());

    console_print("measuring time in ticks                         : ");
    uint32_t ticks = system_get_ticks();
    delay(1000);
    console_println("%lu ticks are equal to 1000 ms in this configuration", (system_get_ticks() - ticks));
}
