---
layout: libdoc
title: Display
permalink: /display/
index: 14
---

KLST_PANDA features a 4.3" TFT display with a 480×272px resolution ( see [Technical Specifications](/tech-specs) ) and a capacitive touch surface:

- ④ :: 4.3" TFT display and capacitive touch surface

the internal color model is ARGB. a single `color` is represented by a 32-bit unsigned integer `uint32_t`:

```
uint32_t m_color = 0xFF7F00FF
//                   A R G B  -> 255, 127, 0, 255
```

## `Display`

```c
void      display_update_event()
void      display_touch_event(TouchEvent* touchevent)

bool      display_init(bool double_buffered, TouchPanelMode touch_panel_mode)
void      display_deinit()
int16_t   display_get_width()
int16_t   display_get_height()
void      display_set_backlight(float brightness)
void      display_enable_automatic_update(bool sync_to_v_blank)
void      display_swap_buffer()
void      display_switch_on()
void      display_switch_off()
bool      display_is_double_buffered()
void      display_set_update_callback(Callback_0_VOID callback)
void      display_set_touch_callback(Callback_1_TOUCHEVENTPTR callback)
uint32_t  display_get_buffer_address()
uint32_t* display_get_buffer()
void      touch_init(TouchPanelMode touch_panel_mode)
void      touch_read(TouchEvent* touchevent)
bool      touch_has_event()
```

## `Draw`

```c
uint32_t  color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
uint32_t  color(const uint8_t r, const uint8_t g, const uint8_t b)
uint32_t  color(const uint8_t gray, const uint8_t a)
uint32_t  color(const uint8_t gray)
void      draw_clear(uint32_t color)
void      draw_fill(uint32_t color)
void      draw_no_fill()
void      draw_stroke(uint32_t color)
void      draw_no_stroke()
void      draw_set_background_color(uint32_t color)
void      draw_set_pixel(int16_t x, int16_t y, uint32_t color)
void      draw_set_pixel_alpha(int16_t x, int16_t y, uint32_t color)
uint32_t  draw_get_pixel(int16_t x, int16_t y)
void      draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
void      draw_line_arbitrary(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)
void      draw_line_horizontal(int16_t x, int16_t y, uint16_t length, uint32_t color)
void      draw_line_horizontal_pattern(int16_t x, int16_t y, uint16_t length, uint8_t pattern)
void      draw_line_vertical(int16_t x, int16_t y, uint16_t length, uint32_t color)
void      draw_line_vertical_pattern(int16_t x, int16_t y, uint16_t length, uint8_t pattern)
void      draw_rect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color, bool filled)
void      draw_rect_stroke(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color)
void      draw_rect_fill(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color)
void      draw_circle(int16_t x, int16_t y, uint16_t radius)
void      draw_circle_stroke(int16_t x, int16_t y, uint16_t radius, uint32_t color) // TODO
void      draw_circle_fill(int16_t x, int16_t y, uint16_t radius, uint32_t color)
void      draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
void      draw_triangle_stroke(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
void      draw_triangle_fill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color)
void      draw_polygon(const std::vector<Point>& points, bool closed)
void      draw_polygon_stroke(const std::vector<Point>& points, bool closed, uint32_t color)
void      draw_polygon_fill(const std::vector<Point>& points, uint32_t color) // TODO WIP
void      draw_ellipse(int16_t x, int16_t y, uint16_t radius_x, uint16_t radius_y)
void      draw_ellipse_stroke(int16_t x, int16_t y, uint16_t radius_x, uint16_t radius_y, uint32_t color)
void      draw_ellipse_fill(int16_t x, int16_t y, uint16_t radius_x, uint16_t radius_y, uint32_t color)
void      draw_image(uint32_t* data, int16_t x, int16_t y, uint16_t width, uint16_t height)
void      draw_set_text(KLSTFont* font)
KLSTFont* draw_get_text()
void      draw_char(int16_t x, int16_t y, char ascii_char)
void      draw_char(int16_t x, int16_t y, char ascii_char, uint8_t scale)
void      draw_text(int16_t x, int16_t y, const std::string& text)
void      draw_text(int16_t x, int16_t y, const std::string& text, uint8_t scale)
uint32_t  draw_blend_colors(uint32_t color_a, uint32_t color_b, uint8_t alpha)
```

## Example `Display`

```cpp
{% include_relative code/Display.ino %}
```
