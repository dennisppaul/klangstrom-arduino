/**
 * this example demonstrates how to use the display with touch panel
 */

#include <vector>

#include "main.h"
#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"
#include "Draw.h"

int16_t x = 0;
int16_t y = 0;

void setup() {
    system_init();

    display_init(true);
    display_set_backlight(0.5f);
    display_enable_automatic_update(true);
}

void loop() {}

void display_update_event() {
    draw_clear(GRAY(0x20));

    draw_rect_fill(30, 30, 20, 20, RGB(0xFF, 0x00, 0x00));
    draw_rect_fill(50, 30, 20, 20, RGB(0x00, 0xFF, 0x00));
    draw_rect_fill(70, 30, 20, 20, RGB(0x00, 0x00, 0xFF));
    draw_rect_fill(display_get_width() - 40, display_get_height() - 40, 20, 20, GRAY(0xFF));
    draw_rect_fill(x, y, 20, 20, RGBA(0xFF, 0xFF, 0xFF, 0x80));
    draw_rect_fill(x + 5, y + 5, 40, 40, 0x80FF8000);

    for (int i = 0; i < 25; ++i) {
        draw_rect_fill(x + (i % 5) * 30, y + i / 5 * 30, 20, 20, RGB(0xFF, 0xFF, 0xFF));
    }

    draw_line_horizontal(0, display_get_height() / 2, display_get_width(), RGB(0xFF, 0xFF, 0xFF));
    draw_line_vertical(display_get_width() / 2, 0, display_get_height(), RGB(0xFF, 0xFF, 0xFF));
    draw_line(0, 0, display_get_width(), display_get_height(), RGB(0xFF, 0xFF, 0xFF));
    draw_line(0, display_get_height(), display_get_width(), 0, RGB(0xFF, 0xFF, 0xFF));
    draw_line_vertical_pattern(0, display_get_height() / 3, display_get_width(), 0b11001100);

    draw_stroke(0xFF0000FF);
    draw_line_vertical_pattern(display_get_width() / 3, 0, display_get_height(), 0b10101010);
    draw_stroke(0xFFFF0000);
    draw_line_horizontal_pattern(0, display_get_height() / 3, display_get_width(), 0b11001100);

    draw_circle_stroke(display_get_width() / 2, display_get_height() / 2, 50, RGB(0xFF, 0xFF, 0xFF));
    draw_circle_fill(display_get_width() / 2 + 50, display_get_height() / 2, 50, RGB(0x00, 0x80, 0xFF));
    draw_rect_stroke(display_get_width() / 2 - 50, display_get_height() / 2 - 50, 100, 100, RGB(0xFF, 0xFF, 0xFF));

    const std::vector<Point> points = {{x, y},
                                       {200, 130},
                                       {200, 200},
                                       {80, 200}};
    draw_polygon_stroke(points, true, RGB(0xFF, 0xFF, 0xFF));
}

void display_touch_event(TouchEvent* touchevent) {
    if (touchevent->number_of_touches == 1) {
        if (touchevent->x[0] >= 0 && touchevent->y[0] >= 0) {
            x = touchevent->x[0];
            y = touchevent->y[0];
        }
    }
}