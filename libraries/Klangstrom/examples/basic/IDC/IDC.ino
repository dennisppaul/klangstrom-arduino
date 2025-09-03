/**
 * this example demonstrates how to use the debugging console to print messages to the serial console.
*/

#include <string>

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "SerialDevice.h"

SerialDevice* serial_IDC_00;
SerialDevice* serial_IDC_01;

void setup() {
    system_init();

    serial_IDC_00 = serialdevice_create(SERIAL_DEVICE_TYPE_IDC_00);
    serial_IDC_01 = serialdevice_create(SERIAL_DEVICE_TYPE_IDC_01, 64, 115200);
}

void loop() {
    console_println("...");
    serialdevice_send(serial_IDC_00, (uint8_t*) "Hello from serial_IDC_00", 24 + 1);
    //    const char* message = "Hello from serial_IDC_01";
    const uint8_t message[4] = {54, 55, 56, 57};
    serialdevice_send(serial_IDC_01, (uint8_t*) message, 4);
    delay(1000);
}

void serial_event(SerialDevice* serial_device) {
    console_println("client: serial event ID %i", serial_device->device_id);
    if (serial_device == serial_IDC_00) {
        if (serial_device->data != nullptr && serial_device->length > 0) {
            std::string data = std::string((char*) serial_device->data, serial_device->length);
            console_println("IDC 00 data ( as ASCII ): %s", data.c_str());
            console_print("IDC 00 data ( as HEX )  : ");
            for (int i = 0; i < serial_device->length; ++i) {
                console_print("0x%02X, ", serial_device->data[i]);
            }
            console_println("");
        }
    }
    if (serial_device == serial_IDC_01) {
        console_println("IDC 01 data ( as ASCII ): %s", serial_device->data);
    }
}
