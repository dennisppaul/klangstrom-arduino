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
    display_set_backlight(0.9f);
}

void loop() {
    console_println("...");
    delay(1000);
}

void display_touch_event(const TouchEvent* touchevent) {
    for (int i = 0; i < touchevent->number_of_touches; ++i) {
        draw_rect_fill(touchevent->x[i], touchevent->y[i], 8, 8, color_from_gray(1.0f));
    }

    if (touchevent->number_of_touches == 4) {
        draw_clear(color_from_gray(0.0f));
    }

    if (touchevent->number_of_touches == 5) {
        draw_clear(color_from_rgb(1.0f, 0.5f, 0.0f));
    }

    console_clear();
    console_println("TOUCH EVENT: %d", touchevent->number_of_touches);
    for (int i = 0; i < touchevent->number_of_touches; ++i) {
        console_println("      %d    : %d,%d", i, touchevent->x[i], touchevent->y[i]);
    }
}
