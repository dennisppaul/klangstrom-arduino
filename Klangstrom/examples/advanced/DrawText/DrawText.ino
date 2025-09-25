/**
 * this example demonstrates how to use the display with touch panel
 */


#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"
#include "Draw.h"
#include "KLSTFont_5x8.h"

int16_t x = display_get_width() / 2;
int16_t y = display_get_height() / 2;

void setup() {
    system_init();

    display_init(true);
    display_enable_automatic_update(true);

    draw_set_text(&Font_5x8);
}

void loop() {}

void display_update_event() {
    draw_clear(color_from_gray(0.3));

    draw_fill(color_from_gray(1.0f));
    draw_set_background_color(color_from_gray_alpha(1.0f, 0.0f));
    draw_text(20, display_get_height() / 2 - 25, "HELLO WORLD", 8);

    draw_fill(color_from_gray_alpha(0.0f, 0.0f));
    draw_set_background_color(color_from_rgb(0.0f, 0.5f, 1.0f));
    draw_char(x, y, '2', 4);
    draw_set_background_color(color_from_rgba(1.0f, 0.5f, 0.0f, 0.5f)); // TODO alpha blending does not work
    draw_text(x + Font_5x8.width * 4, y, "3 or 42 ... not sure");
}

void display_touch_event(const TouchEvent* touchevent) {
    if (touchevent->number_of_touches == 1) {
        if (touchevent->x[0] >= 0 && touchevent->y[0] >= 0) {
            x = touchevent->x[0];
            y = touchevent->y[0];
        }
    }
}
