/**
 * this example demonstrates how to use the display with touch panel
 */

#include <vector>

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Display.h"
#include "Draw.h"
#include "BitmapFont.h"

#include "KLST--ICON--128x128.h"

class PVector {
public:
    PVector(const uint16_t x, const uint16_t y)
        : x(x),
          y(y) {}

    PVector()
        : x(0),
          y(0) {}

    uint16_t x;
    uint16_t y;
};

// std::vector<PVector> points;
constexpr uint16_t MAX_POINTS     = 64;
uint16_t           points_counter = 0;
PVector            points[MAX_POINTS];

int16_t x = 100;
int16_t y = 100;

void setup() {
    system_init();

    display_init(true, POLLING);
    display_set_backlight(0.5f);
    display_enable_automatic_update(true);
}

void loop() {
    if (touch_has_event()) {
        TouchEvent touch_event;
        touch_read(&touch_event);
        display_touch_event(&touch_event);
    }
    delay(10);
}

uint8_t move = 0;

void display_update_event() {
    draw_clear(GRAY(0x00));

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

    for (int i = 0; i < MAX_POINTS; ++i) {
        draw_set_pixel(points[i].x, points[i].y, GRAY(0xFF));
    }

    draw_image(IMAGE_DATA,
               move++,
               IMAGE_HEIGHT / 2, IMAGE_WIDTH,
               IMAGE_HEIGHT);

    const std::vector<Point> points = {{x, y},
                                       {200, 130},
                                       {200, 200},
                                       {80, 200}};
    draw_polygon_stroke(points, true, RGB(0xFF, 0xFF, 0xFF));

    draw_text(&Font12,
              x, y,
              "hello",
              TextAlign::LEFT,
              RGB(0xFF, 0x00, 0x00),
              GRAY_ALPHA(0x00, 0x00));
}

void display_touch_event(TouchEvent* touchevent) {
    console_clear();
    console_println("TOUCH EVENT: %d", touchevent->number_of_touches);
    for (int i = 0; i < touchevent->number_of_touches; ++i) {
        if (touchevent->x[i] >= 0 && touchevent->y[i] >= 0) {
            console_println("TOUCH %d    : %d,%d", i, touchevent->x[i], touchevent->y[i]);
            points[points_counter].x = touchevent->x[i];
            points[points_counter].y = touchevent->y[i];
            points_counter++;
            points_counter %= MAX_POINTS;
            if (i == 0) {
                x = touchevent->x[i];
                y = touchevent->y[i];
            }
            if (i == 1) {
                const float brightness = static_cast<float>(touchevent->x[i]) / 480.0f;
                display_set_backlight(brightness);
            }
        }
    }
}
