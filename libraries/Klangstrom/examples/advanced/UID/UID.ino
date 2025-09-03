/**
 * this example demonstrates how to use the display with touch panel
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"
#include "Draw.h"
#include "KLSTFont_5x8.h"

int16_t x = display_get_width() / 2;
int16_t y = display_get_height() / 2;

std::string formatHex(int num) {
    std::stringstream ss;
    ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << num;
    return ss.str();
}

void print_UID() {
    console_print("{");
    for (int i = 0; i < 3; i++) {
        const int line_height = 40;
        const std::string mUID = formatHex(system_get_UID(i));
        console_print("%s", mUID.c_str());
        if (i < 2) {
            console_print(", ");
        }
    }
    console_print("}, ");
}


void setup() {
    system_init();

    display_init(true);
    display_enable_automatic_update(true);
    draw_set_text(&Font_5x8);

    print_UID();
    console_println("// KLST_..._%i", system_get_UID_index());
}

void loop() {}

void display_update_event() {
    draw_clear(GRAY(0x00));
    draw_fill(color(0xFF));
    draw_set_background_color(color(0xFF, 0x00));
    const int line_height = 40;
    for (int i = 0; i < 3; i++) {
        const std::string mUID = formatHex(system_get_UID(i));
        draw_text(20, 20 + line_height * i, mUID, 4);
    }
    // to string
    draw_text(20, 20 + line_height * 3, std::to_string(system_get_UID_index()), 8);
}
