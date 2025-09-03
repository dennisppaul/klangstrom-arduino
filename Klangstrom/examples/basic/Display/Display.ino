/**
 * this example demonstrates how to use the display with touch panel
 */

#include <vector>

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"
#include "Draw.h"

void setup() {
    system_init();

    display_init(false);
}

void loop() {
    console_println("...");
    delay(1000);
}

void display_touch_event(TouchEvent* touchevent) {
    for (int i = 0; i < touchevent->number_of_touches; ++i) {
        draw_rect_fill(touchevent->x[i], touchevent->y[i], 8, 8, GRAY(0xFF));
    }

    if (touchevent->number_of_touches == 4) {
        draw_clear(GRAY(0x00));
    }

    if (touchevent->number_of_touches == 5) {
        draw_clear(RGB(0xFF, 0x80, 0x00));
    }

    console_clear();
    console_println("TOUCH EVENT: %d", touchevent->number_of_touches);
    for (int i = 0; i < touchevent->number_of_touches; ++i) {
        console_println("      %d    : %d,%d", i, touchevent->x[i], touchevent->y[i]);
    }
}
