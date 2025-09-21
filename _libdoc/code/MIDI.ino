/**
 * this example demonstrates how to use the ( analog ) MIDI interface.
*/

#include <string>

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "MIDI.h"

SerialDevice* serial_IDC_01;

void midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    console_println("Note on: Channel %i, Note %i, Velocity %i", channel, note, velocity);
}

void setup() {
    system_init();
    midi_init();
    serial_IDC_01 = serialdevice_create(SERIAL_DEVICE_TYPE_IDC_01, 64, 115200);
}

void loop() {
    uint8_t note_on_data[3] = {0x90, 0x3C, 0x40};
    //    serialdevice_send(serial_IDC_01, (uint8_t*) note_on_data, 3);
    serialdevice_send(serial_IDC_01, (uint8_t*) note_on_data, 1);
    serialdevice_send(serial_IDC_01, (uint8_t*) (note_on_data + 1), 1);
    serialdevice_send(serial_IDC_01, (uint8_t*) (note_on_data + 2), 1);

    uint8_t note_off_data[3] = {0x80, 0x3C, 0x00};
    midi_send((uint8_t*) note_off_data, 3);

    console_println("...");
    delay(1000);
}
