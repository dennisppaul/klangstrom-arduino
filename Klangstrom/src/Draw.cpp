/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Klangstrom.h"

#ifdef KLST_PERIPHERAL_ENABLE_DISPLAY

#include <stdint.h>
#include <vector>

#include "Draw.h"
#include "DisplayDrawInterface.h"
#include "Display.h"

#ifndef ABS
#define ABS(X) ((X) > 0 ? (X) : -(X))
#endif

#ifndef MIN
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

static uint32_t    fFillColor       = 0xFFFFFFFF;
static bool        fHasFill         = true;
static uint32_t    fStrokeColor     = 0xFFFFFFFF;
static bool        fHasStroke       = false;
static uint32_t    fBackgroundColor = 0x000000FF;
static BitmapFont* fFont            = nullptr;
static TextAlign   fTextAlign       = LEFT;
static KLSTFont*   fKLSTFont        = nullptr;

/* NOTE these functions are deliberately NOT placed in `extern "C" {}` block to allow overloading */

void draw_clear(const uint32_t color) {
    display_clear_BSP(color);
}

void draw_fill(const uint32_t color) {
    fFillColor = color;
    fHasFill   = true;
}

void draw_no_fill() {
    fHasFill = false;
}

void draw_stroke(const uint32_t color) {
    fStrokeColor = color;
    fHasStroke   = true;
}

void draw_no_stroke() {
    fHasStroke = false;
}

void draw_set_pixel(const int16_t  x,
                    const int16_t  y,
                    const uint32_t color) {
    display_set_pixel_BSP(x, y, color);
}

void draw_set_pixel_alpha(const int16_t x, const int16_t y, const uint32_t color) {
    const uint8_t alpha = GET_ALPHA(color);
    if (alpha == 0x00) {
    } else if (alpha == 0xFF) {
        display_set_pixel_BSP(x, y, color);
    } else if (alpha > 0x00) {
        const uint32_t pixel       = display_get_pixel_BSP(x, y);
        const uint32_t mBlendColor = draw_blend_colors(pixel, color, alpha);
        display_set_pixel_BSP(x, y, mBlendColor);
    }
}

uint32_t draw_get_pixel(const int16_t x, const int16_t y) {
    return display_get_pixel_BSP(x, y);
}

void draw_line(const int16_t  x0,
               const int16_t  y0,
               const int16_t  x1,
               const int16_t  y1,
               const uint32_t color) {
    if (x0 == x1) {
        draw_line_vertical(x0, MIN(y0, y1), ABS(y1 - y0), color);
    } else if (y0 == y1) {
        draw_line_horizontal(MIN(x0, x1), y0, ABS(x1 - x0), color);
    } else {
        // draw_line_arbitrary_clipped(x0, y0,
        //                             x1, y1,
        //                             0, 0,
        //                             display_get_width(), display_get_height(),
        //                             color);
        draw_line_arbitrary(x0, y0, x1, y1, color);
    }
}

// //  Cohen-Sutherland algorithm for line clipping
//
// #define LINE_INSIDE 0 // 0000
// #define LINE_LEFT 1   // 0001
// #define LINE_RIGHT 2  // 0010
// #define LINE_BOTTOM 4 // 0100
// #define LINE_TOP 8    // 1000
//
// // Function to compute the region code for a point (x, y)
// static uint8_t compute_out_code(const int16_t x, const int16_t y,
//                                 const int16_t min_x, const int16_t min_y,
//                                 const int16_t max_x, const int16_t max_y) {
//     uint8_t code = LINE_INSIDE; // initialized as being inside of clip window
//
//     if (x < min_x) { // to the left of the clip window
//         code |= LINE_LEFT;
//     } else if (x > max_x) { // to the right of the clip window
//         code |= LINE_RIGHT;
//     }
//     if (y < min_y) { // below the clip window
//         code |= LINE_BOTTOM;
//     } else if (y > max_y) { // above the clip window
//         code |= LINE_TOP;
//     }
//
//     return code;
// }
//
// void draw_line_arbitrary_clipped(const int16_t  x0,
//                                  const int16_t  y0,
//                                  const int16_t  x1,
//                                  const int16_t  y1,
//                                  const int16_t  min_x,
//                                  const int16_t  min_y,
//                                  const int16_t  max_x,
//                                  const int16_t  max_y,
//                                  const uint32_t color) {
//     int16_t x0_clipped = x0;
//     int16_t y0_clipped = y0;
//     int16_t x1_clipped = x1;
//     int16_t y1_clipped = y1;
//
//     int outcode0 = compute_out_code(x0_clipped, y0_clipped, min_x, min_y, max_x, max_y);
//     int outcode1 = compute_out_code(x1_clipped, y1_clipped, min_x, min_y, max_x, max_y);
//
//     bool accept = false;
//
//     while (true) {
//         if (!(outcode0 | outcode1)) { // Trivially accept, both points inside
//             accept = true;
//             console_println("Trivially accept, both points inside");
//             break;
//         } else if (outcode0 & outcode1) { // Trivially reject, both points share an outside zone
//             console_println("Trivially reject, both points share an outside zone");
//             break;
//         } else {
//             console_println("Trivially accept, one point inside, one outside");
//
//             const uint8_t outcode_out = outcode0 ? outcode0 : outcode1;
//             int16_t       x, y;
//
//             if (outcode_out & LINE_TOP) { // Point is above the clip rectangle
//                 x = x0_clipped + (x1_clipped - x0_clipped) * (max_y - y0_clipped) / (y1_clipped - y0_clipped);
//                 y = max_y;
//             } else if (outcode_out & LINE_BOTTOM) { // Point is below the clip rectangle
//                 x = x0_clipped + (x1_clipped - x0_clipped) * (min_y - y0_clipped) / (y1_clipped - y0_clipped);
//                 y = min_y;
//             } else if (outcode_out & LINE_RIGHT) { // Point is to the right of the clip rectangle
//                 y = y0_clipped + (y1_clipped - y0_clipped) * (max_x - x0_clipped) / (x1_clipped - x0_clipped);
//                 x = max_x;
//             } else if (outcode_out & LINE_LEFT) { // Point is to the left of the clip rectangle
//                 y = y0_clipped + (y1_clipped - y0_clipped) * (min_x - x0_clipped) / (x1_clipped - x0_clipped);
//                 x = min_x;
//             }
//
//             if (outcode_out == outcode0) {
//                 x0_clipped = x;
//                 y0_clipped = y;
//                 outcode0   = compute_out_code(x0_clipped, y0_clipped, min_x, min_y, max_x, max_y);
//             } else {
//                 x1_clipped = x;
//                 y1_clipped = y;
//                 outcode1   = compute_out_code(x1_clipped, y1_clipped, min_x, min_y, max_x, max_y);
//             }
//         }
//     }
//
//     if (accept) {
//         draw_line(x0_clipped, y0_clipped, x1_clipped, y1_clipped, color);
//     }
// }

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a         = b;      \
        b         = t;      \
    }
#endif

void _draw_line_arbitrary(int16_t        x0,
                          int16_t        y0,
                          int16_t        x1,
                          int16_t        y1,
                          const uint32_t color) {
    /*
     * @brief "Write a line.  Bresenham's algorithm - thx wikpedia" thx adafruit
     *
     */
    const bool steep = ABS(y1 - y0) > ABS(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    const int16_t dx = x1 - x0;
    const int16_t dy = ABS(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
            display_set_pixel_BSP(y0, x0, color);
        } else {
            display_set_pixel_BSP(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void draw_line_arbitrary(const int16_t  x0,
                         const int16_t  y0,
                         const int16_t  x1,
                         const int16_t  y1,
                         const uint32_t color) {
    // TODO check bounds and clamp values
    int16_t xinc1;
    int16_t xinc2;
    int16_t yinc1;
    int16_t yinc2;
    int16_t den;
    int16_t num;
    int16_t num_add;
    int16_t num_pixels;

    const int16_t deltax = ABS(x1 - x0); /* The difference between the x's */
    const int16_t deltay = ABS(y1 - y0); /* The difference between the y's */
    int16_t       x      = x0;           /* Start x off at the first pixel */
    int16_t       y      = y0;           /* Start y off at the first pixel */

    /* The x-values are increasing */
    if (x1 >= x0) {
        xinc1 = 1;
        xinc2 = 1;
    } else {
        /* The x-values are decreasing */
        xinc1 = -1;
        xinc2 = -1;
    }
    /* The y-values are increasing */
    if (y1 >= y0) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        /* The y-values are decreasing */
        yinc1 = -1;
        yinc2 = -1;
    }

    /* There is at least one x-value for every y-value */
    if (deltax >= deltay) {
        xinc1      = 0; /* Don't change the x when numerator >= denominator */
        yinc2      = 0; /* Don't change the y for every iteration */
        den        = deltax;
        num        = deltax / 2;
        num_add    = deltay;
        num_pixels = deltax; /* There are more x-values than y-values */
    } else {
        /* There is at least one y-value for every x-value */
        xinc2      = 0; /* Don't change the x for every iteration */
        yinc1      = 0; /* Don't change the y when numerator >= denominator */
        den        = deltay;
        num        = deltay / 2;
        num_add    = deltax;
        num_pixels = deltay; /* There are more y-values than x-values */
    }

    for (int16_t curpixel = 0; curpixel <= num_pixels; curpixel++) {
        display_set_pixel_BSP(x, y, color); /* Draw the current pixel */
        num += num_add;                     /* Increase the numerator by the top of the fraction */
        /* Check if numerator >= denominator */
        if (num >= den) {
            num -= den; /* Calculate the new numerator value */
            x += xinc1; /* Change the x as appropriate */
            y += yinc1; /* Change the y as appropriate */
        }
        x += xinc2; /* Change the x as appropriate */
        y += yinc2; /* Change the y as appropriate */
    }
}

void draw_line_horizontal(const int16_t  x,
                          const int16_t  y,
                          const uint16_t length,
                          const uint32_t color) {
    display_line_horizontal_BSP(x, y, length, color);
}

static bool evaluatePattern(const uint8_t pIndex, const uint8_t pPattern) {
    const uint8_t mMask = static_cast<uint8_t>(1 << pIndex);
    return (mMask & pPattern) > 0;
}

void draw_line_horizontal_pattern(const int16_t x, const int16_t y, const uint16_t length, const uint8_t pattern) {
    for (uint16_t i = 0; i < length; i++) {
        const uint8_t mPatternID = i % 8;
        const int16_t mX         = x + i;
        if (evaluatePattern(mPatternID, pattern)) {
            display_set_pixel_BSP(mX, y, fStrokeColor);
        } else {
            display_set_pixel_alpha_BSP(mX, y, fBackgroundColor);
        }
    }
}

void draw_line_vertical(const int16_t  x,
                        const int16_t  y,
                        const uint16_t length,
                        const uint32_t color) {
    display_line_vertical_BSP(x, y, length, color);
}

void draw_line_vertical_pattern(const int16_t x, const int16_t y, const uint16_t length, const uint8_t pattern) {
    for (uint16_t i = 0; i < length; i++) {
        const uint8_t mPatternID = i % 8;
        const int16_t mY         = y + i;
        if (evaluatePattern(mPatternID, pattern)) {
            display_set_pixel_BSP(x, mY, fStrokeColor);
        } else {
            display_set_pixel_alpha_BSP(x, mY, fBackgroundColor);
        }
    }
}

void draw_rect(const int16_t  x,
               const int16_t  y,
               const uint16_t width,
               const uint16_t height,
               const uint32_t color, const bool filled) {
    // TODO check bounds and clamp values
    // TODO remove color and filled parameters and use global color variables
    if (filled) {
        display_rect_fill_BSP(x, y, width, height, color);
    } else {
        draw_rect_stroke(x, y, width, height, color);
    }
}

void draw_rect_fill(const int16_t  x,
                    const int16_t  y,
                    const uint16_t width,
                    const uint16_t height,
                    const uint32_t color) {
    display_rect_fill_BSP(x, y, width, height, color);
}

void draw_rect_stroke(const int16_t  x,
                      const int16_t  y,
                      const uint16_t width,
                      const uint16_t height,
                      const uint32_t color) {
    /* Draw horizontal lines */
    display_line_horizontal_BSP(x, y, width, color);
    display_line_horizontal_BSP(x, (y + height), width, color);

    /* Draw vertical lines */
    display_line_vertical_BSP(x, y, height, color);
    display_line_vertical_BSP((x + width), y, height, color);
}

void draw_circle(const int16_t  x,
                 const int16_t  y,
                 const uint16_t radius) {
    if (fHasStroke) {
        draw_circle_stroke(x, y, radius, fStrokeColor);
    }
    if (fHasFill) {
        draw_circle_fill(x, y, radius, fFillColor);
    }
}

void draw_circle_stroke(const int16_t  x,
                        const int16_t  y,
                        const uint16_t radius,
                        const uint32_t color) {
    int32_t  decision  = 3 - (radius << 1);
    uint32_t current_x = 0;
    uint32_t current_y = radius;

    while (current_x <= current_y) {
        display_set_pixel_BSP((x + current_x), (y - current_y), color);
        display_set_pixel_BSP((x - current_x), (y - current_y), color);
        display_set_pixel_BSP((x + current_y), (y - current_x), color);
        display_set_pixel_BSP((x - current_y), (y - current_x), color);
        display_set_pixel_BSP((x + current_x), (y + current_y), color);
        display_set_pixel_BSP((x - current_x), (y + current_y), color);
        display_set_pixel_BSP((x + current_y), (y + current_x), color);
        display_set_pixel_BSP((x - current_y), (y + current_x), color);
        if (decision < 0) {
            decision += (current_x << 2) + 6;
        } else {
            decision += ((current_x - current_y) << 2) + 10;
            current_y--;
        }
        current_x++;
    }
}

void draw_circle_fill(int16_t x0, int16_t y0, uint16_t r,
                      const uint32_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    display_set_pixel_BSP(x0, y0 + r, color);
    display_set_pixel_BSP(x0, y0 - r, color);
    display_set_pixel_BSP(x0 + r, y0, color);
    display_set_pixel_BSP(x0 - r, y0, color);
    display_line_horizontal_BSP(x0 - r, y0, 2 * r, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        display_line_horizontal_BSP(x0 - x, y0 + y, 2 * x, color);
        display_line_horizontal_BSP(x0 - x, y0 - y, 2 * x, color);

        display_line_horizontal_BSP(x0 - y, y0 + x, 2 * y, color);
        display_line_horizontal_BSP(x0 - y, y0 - x, 2 * y, color);
    }
}

void draw_triangle(const int16_t x0, const int16_t y0,
                   const int16_t x1, const int16_t y1,
                   const int16_t x2, const int16_t y2) {
    if (fHasStroke) {
        draw_triangle_stroke(x0, y0, x1, y1, x2, y2, fStrokeColor);
    }
    if (fHasFill) {
        draw_triangle_fill(x0, y0, x1, y1, x2, y2, fFillColor);
    }
}

void draw_triangle_stroke(const int16_t x0, const int16_t y0,
                          const int16_t x1, const int16_t y1,
                          const int16_t x2, const int16_t y2,
                          uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_triangle_fill(const int16_t x0, const int16_t y0,
                        const int16_t x1, const int16_t y1,
                        const int16_t x2, const int16_t y2,
                        const uint32_t color) {
    int16_t xinc1;
    int16_t xinc2;
    int16_t yinc1;
    int16_t yinc2;
    int16_t den;
    int16_t num;
    int16_t num_add;
    int16_t num_pixels;

    const int16_t deltax = ABS(x1 - x0); /* The difference between the x's */
    const int16_t deltay = ABS(y1 - y0); /* The difference between the y's */
    int16_t       x      = x0;           /* Start x off at the first pixel */
    int16_t       y      = y0;           /* Start y off at the first pixel */

    /* The x-values are increasing */
    if (x1 >= x0) {
        xinc1 = 1;
        xinc2 = 1;
    } else {
        /* The x-values are decreasing */
        xinc1 = -1;
        xinc2 = -1;
    }

    /* The y-values are increasing */
    if (y1 >= y0) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        /* The y-values are decreasing */
        yinc1 = -1;
        yinc2 = -1;
    }

    /* There is at least one x-value for every y-value */
    if (deltax >= deltay) {
        xinc1      = 0; /* Don't change the x when numerator >= denominator */
        yinc2      = 0; /* Don't change the y for every iteration */
        den        = deltax;
        num        = deltax / 2;
        num_add    = deltay;
        num_pixels = deltax; /* There are more x-values than y-values */
    } else {
        /* There is at least one y-value for every x-value */
        xinc2      = 0; /* Don't change the x for every iteration */
        yinc1      = 0; /* Don't change the y when numerator >= denominator */
        den        = deltay;
        num        = deltay / 2;
        num_add    = deltax;
        num_pixels = deltay; /* There are more y-values than x-values */
    }

    for (int16_t curpixel = 0; curpixel <= num_pixels; curpixel++) {
        draw_line(x, y, x2, y2, color);

        num += num_add; /* Increase the numerator by the top of the fraction */
        if (num >= den) /* Check if numerator >= denominator */
        {
            num -= den; /* Calculate the new numerator value */
            x += xinc1; /* Change the x as appropriate */
            y += yinc1; /* Change the y as appropriate */
        }
        x += xinc2; /* Change the x as appropriate */
        y += yinc2; /* Change the y as appropriate */
    }
}

void draw_polygon(const std::vector<Point>& points, const bool closed = true) {
    if (fHasStroke) {
        draw_polygon_stroke(points, closed, fStrokeColor);
    }
    if (fHasFill) {
        draw_polygon_fill(points, fFillColor);
    }
}

void draw_polygon_stroke(const std::vector<Point>& points, const bool closed, const uint32_t color) {
    if (points.size() < 2) {
        return;
    }
    for (uint16_t i = 0; i < points.size() - 1; i++) {
        draw_line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
    }
    if (closed) {
        draw_line(points[0].x, points[0].y, points[points.size() - 1].x, points[points.size() - 1].y, color);
    }
}

void draw_polygon_fill(const std::vector<Point>& points, const uint32_t color) {
    int16_t x1 = 0;
    int16_t y1 = 0;
    int16_t image_right;
    int16_t image_bottom;
    int16_t image_left = image_right = points[0].x;
    int16_t image_top = image_bottom = points[0].y;

    for (int16_t counter = 1; counter < points.size(); counter++) {
        const int16_t pixelX = points[counter].x;
        if (pixelX < image_left) {
            image_left = pixelX;
        }
        if (pixelX > image_right) {
            image_right = pixelX;
        }

        const int16_t pixelY = points[counter].y;
        if (pixelY < image_top) {
            image_top = pixelY;
        }
        if (pixelY > image_bottom) {
            image_bottom = pixelY;
        }
    }

    if (points.size() < 2) {
        return;
    }

    const int16_t X_center = (image_left + image_right) / 2;
    const int16_t Y_center = (image_bottom + image_top) / 2;

    const int16_t X_first = points[0].x;
    const int16_t Y_first = points[0].y;

    for (uint16_t i = 0; i < points.size() - 1; i++) {
        const int16_t X = points[i].x;
        const int16_t Y = points[i].y;
        x1              = points[i + 1].x;
        y1              = points[i + 1].y;

        draw_triangle_fill(X, x1, X_center, Y, y1, Y_center, color);
        draw_triangle_fill(X, X_center, x1, Y, Y_center, y1, color);
        draw_triangle_fill(X_center, x1, X, Y_center, y1, Y, color);
    }

    draw_triangle_fill(X_first, x1, X_center, Y_first, y1, Y_center, color);
    draw_triangle_fill(X_first, X_center, x1, Y_first, Y_center, y1, color);
    draw_triangle_fill(X_center, x1, X_first, Y_center, y1, Y_first, color);
}

void draw_ellipse(const int16_t  x,
                  const int16_t  y,
                  const uint16_t radius_x,
                  const uint16_t radius_y) {
    if (fHasStroke) {
        draw_ellipse_stroke(x, y, radius_x, radius_y, fStrokeColor);
    }
    if (fHasFill) {
        draw_ellipse_fill(x, y, radius_x, radius_y, fFillColor);
    }
}

void draw_ellipse_stroke(const int16_t  x,
                         const int16_t  y,
                         const uint16_t radius_x,
                         const uint16_t radius_y,
                         const uint32_t color) {
    int16_t     xx   = 0;
    int16_t     yy   = -radius_y;
    int16_t     err  = 2 - 2 * radius_x;
    const float rad1 = radius_x;
    const float rad2 = radius_y;
    const float k    = rad2 / rad1;

    do {
        display_set_pixel_BSP(x - xx / k, y + yy, color);
        display_set_pixel_BSP(x + xx / k, y + yy, color);
        display_set_pixel_BSP(x + xx / k, y - yy, color);
        display_set_pixel_BSP(x - xx / k, y - yy, color);

        int16_t e2 = err;
        if (e2 <= xx) {
            err += ++xx * 2 + 1;
            if (-yy == xx && e2 <= yy) {
                e2 = 0;
            }
        }
        if (e2 > yy) {
            err += ++yy * 2 + 1;
        }
    } while (yy <= 0);
}

void draw_ellipse_fill(const int16_t  x,
                       const int16_t  y,
                       const uint16_t radius_x,
                       const uint16_t radius_y,
                       const uint32_t color) {
    int16_t     xx   = 0;
    int16_t     yy   = -radius_y;
    int16_t     err  = 2 - 2 * radius_x;
    const float rad1 = radius_x;
    const float rad2 = radius_y;
    const float k    = rad2 / rad1;

    do {
        display_line_horizontal_BSP(x - xx / k, y + yy, 2 * (xx / k) + 1, color);
        display_line_horizontal_BSP(x - xx / k, y - yy, 2 * (xx / k) + 1, color);
        int16_t e2 = err;
        if (e2 <= xx) {
            err += ++xx * 2 + 1;
            if (-yy == xx && e2 <= yy) {
                e2 = 0;
            }
        }
        if (e2 > yy) {
            err += ++yy * 2 + 1;
        }
    } while (yy <= 0);
}

void draw_image(uint32_t*      data,
                const int16_t  x,
                const int16_t  y,
                const uint16_t width,
                const uint16_t height) {
    display_image_BSP(data, x, y, width, height);
}

void draw_set_text(BitmapFont* font) {
    fFont = font;
}

KLSTFont* draw_get_text() {
    return fKLSTFont;
}

void draw_set_background_color(const uint32_t color) {
    fBackgroundColor = color;
}

void draw_set_text_align(const TextAlign align) {
    fTextAlign = align;
}

void draw_char(BitmapFont*    font,
               const int16_t  x,
               const int16_t  y,
               const uint8_t  ascii_char,
               const uint32_t color,
               const uint32_t background_color) {
    display_char_BSP(font, x, y, ascii_char, color, background_color);
}

void draw_text(BitmapFont*     font,
               const int16_t   x,
               const int16_t   y,
               const char*     text,
               const TextAlign align,
               const uint32_t  color,
               const uint32_t  background_color) {
    uint16_t ref_column;
    uint16_t i    = 0;
    uint32_t size = 0;
    auto     ptr  = text;

    /* Get the text size */
    while (*ptr++) {
        size++;
    }

    /* Characters number per line */
    const uint32_t xsize = display_get_width() / font->Width;

    switch (align) {
        case CENTERED: {
            ref_column = x + (xsize - size) * font->Width / 2;
            break;
        }
        case LEFT: {
            ref_column = x;
            break;
        }
        case RIGHT: {
            ref_column = -x + (xsize - size) * font->Width;
            break;
        }
        default: {
            ref_column = x;
            break;
        }
    }

    /* Check that the Start column is located in the screen */
    if ((ref_column < 1) || (ref_column >= 0x8000)) {
        ref_column = 1;
    }

    /* Send the string character by character on LCD */
    while (*text != 0 & (display_get_width() - i * font->Width & 0xFFFF) >= font->Width) {
        /* Display one character on LCD */
        draw_char(font, ref_column, y, *text, color, background_color);
        /* Decrement the column position by 16 */
        ref_column += font->Width;
        /* Point on the next character */
        text++;
        i++;
    }
}

void draw_set_text(KLSTFont* font) {
    fKLSTFont = font;
}

void draw_char(const int16_t x, const int16_t y, const char ascii_char) {
    if (ascii_char < 32 || ascii_char > 126) {
        return;
    }
    if (fKLSTFont == nullptr) {
        return;
    }
    for (uint32_t i = 0; i < fKLSTFont->height; i++) {
        const uint32_t b = fKLSTFont->data[(ascii_char - 32) * fKLSTFont->height + i];
        for (uint32_t j = 0; j < fKLSTFont->width; j++) {
            const uint32_t mColor = b << j & 0x8000 ? fFillColor : fBackgroundColor;
            display_set_pixel_alpha_BSP(x + j, y + i, mColor);
        }
    }
}

void draw_char(const int16_t x, const int16_t y, const char ascii_char, const uint8_t scale) {
    if (scale == 1) {
        draw_char(x, y, ascii_char);
        return;
    }
    if (ascii_char < 32 || ascii_char > 126) {
        return;
    }
    if (fKLSTFont == nullptr) {
        return;
    }
    for (uint32_t i = 0; i < fKLSTFont->height; i++) {
        const uint32_t b = fKLSTFont->data[(ascii_char - 32) * fKLSTFont->height + i];
        for (uint32_t j = 0; j < fKLSTFont->width; j++) {
            const uint32_t mColor = b << j & 0x8000 ? fFillColor : fBackgroundColor;
            const uint8_t  alpha  = GET_ALPHA(mColor);
            if (alpha != 0x00) {
                display_rect_fill_BSP(x + j * scale, y + i * scale, scale, scale, mColor);
            }
        }
    }
}

void draw_text(const int16_t x, const int16_t y, const std::string& text) {
    if (text.empty()) {
        return;
    }
    if (fKLSTFont == nullptr) {
        return;
    }
    for (uint32_t i = 0; i < text.size(); i++) {
        draw_char(x + i * fKLSTFont->width, y, text[i]);
    }
}

void draw_text(const int16_t x, const int16_t y, const std::string& text, const uint8_t scale) {
    if (scale == 1) {
        draw_text(x, y, text);
        return;
    }
    if (text.empty()) {
        return;
    }
    if (fKLSTFont == nullptr) {
        return;
    }
    for (uint32_t i = 0; i < text.size(); i++) {
        draw_char(x + i * fKLSTFont->width * scale, y, text[i], scale);
    }
}


uint32_t draw_blend_colors(const uint32_t color_a, const uint32_t color_b, const uint8_t alpha) {
    const uint8_t  inv   = 0xFF - alpha;
    const uint8_t  r     = (GET_RED(color_a) * inv + GET_RED(color_b) * alpha) >> 8;
    const uint8_t  g     = (GET_GREEN(color_a) * inv + GET_GREEN(color_b) * alpha) >> 8;
    const uint8_t  b     = (GET_BLUE(color_a) * inv + GET_BLUE(color_b) * alpha) >> 8;
    const uint32_t blend = RGBA(r, g, b, 0xFF);
    return blend;
}

#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
