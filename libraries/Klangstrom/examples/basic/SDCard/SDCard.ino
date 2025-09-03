/**
 * this example demonstrates how to use an SD card.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "SDCard.h"

void setup() {
    system_init();
    console_init();
    sdcard_init();

    sdcard_status();

    if (sdcard_detected()) {
        sdcard_mount();
        sdcard_file_create("KLST.TXT");
        uint8_t buffer[26] = "KLST writing to SD card!\n";
        sdcard_file_write(buffer, 26);
        sdcard_file_close();
        sdcard_unmount();
    }
}

void loop() {
    if (sdcard_detected()) {
        sdcard_mount();
        std::vector<std::string> files;
        std::vector<std::string> directories;
        sdcard_list("", files, directories);
        console_println("SD card detected");
        console_println("Files      : %i", files.size());
        for (std::string file: files) {
            console_println("             %s", file.c_str());
        }
        console_println("Directories: %i", directories.size());
        for (std::string directory: directories) {
            console_println("             %s", directory.c_str());
        }
        console_println("...");
        sdcard_unmount();
    } else {
        console_println("SD card not detected");
    }
    delay(1000);
}
