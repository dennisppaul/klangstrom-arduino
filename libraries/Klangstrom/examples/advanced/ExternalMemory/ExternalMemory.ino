/**
 * this example demonstrates how to use external memory.
 */

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "AudioDevice.h"

int    sample_buffer_size = 48000 * 4; // 4 seconds at 48kHz
int    bytes_per_float    = sizeof(float);
float* sample_buffer      = nullptr;

void setup() {
    system_init();
    console_init();

    sample_buffer = system_external_memory_allocate_float_array(sample_buffer_size);
    for (size_t i = 0; i < sample_buffer_size; i++) {
        sample_buffer[i] = (float)i / (float)sample_buffer_size;
    }
}

void loop() {
    console_println("%d bytes external memory available", system_external_memory_available());
    if (system_external_memory_available() > sample_buffer_size * bytes_per_float) {
        console_println("allocating %d bytes in external memory", sample_buffer_size * bytes_per_float);

        float* sample_buffer_two = system_external_memory_allocate_float_array(sample_buffer_size);

        if (sample_buffer_two) {
            console_println("success … copying data");
            for (size_t i = 0; i < sample_buffer_size; i++) {
                sample_buffer_two[i] = sample_buffer[i];
            }
        } else {
            console_println("failed");
        }
    } else {
        console_println("not enough external memory available … resetting memory");
        system_external_memory_reset();
    }
    console_println("---");
    delay(500);
}
