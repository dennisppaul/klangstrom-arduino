/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2025 Dennis P Paul.
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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#ifdef KLST_PANDA_EMU
#define KLST_EMU_DISPLAY_WIDTH 480
#define KLST_EMU_DISPLAY_HEIGHT 272
#else
#ifndef KLST_EMU_DISPLAY_WIDTH
#warning "KLST_EMU_DISPLAY_WIDTH not defined, using default value"
#define KLST_EMU_DISPLAY_WIDTH 480
#endif // KLST_EMU_DISPLAY_WIDTH
#ifndef KLST_EMU_DISPLAY_HEIGHT
#warning "KLST_EMU_DISPLAY_HEIGHT not defined, using default value"
#define KLST_EMU_DISPLAY_HEIGHT 272
#endif // KLST_EMU_DISPLAY_HEIGHT
#endif // KLST_PANDA_EMU

#include "Console.h"
#include "PGraphics.h"
#include "PImage.h"
#include "PVector.h"
#include "KlangstromEmulator.h"
#include "Display.h"
#include "DisplayDrawInterface.h"
#include "UmfeldFunctions.h"

using namespace umfeld;

class DrawableDisplay final : public Drawable {
public:
    const float width;
    const float height;
    float       brightness = 1.0f;

    DrawableDisplay(const uint16_t display_width,
                    const uint16_t display_height) : width(display_width),
                                                     height(display_height), fFrameBuffers{} {
        fFrameBuffers[0] = new PImage(display_width, display_height);
        fFrameBuffers[1] = new PImage(display_width, display_height);

        // subscribe to `mouseMoved()`
    }

    void draw(PGraphics* g_ptr) override {
        if (g_ptr == nullptr) {
            return;
        }
        PGraphics& graphics = *g_ptr;
        PImage&    buffer   = *fFrameBuffers[fActiveBuffer];

        graphics.pushMatrix();
        graphics.translate(fPosition.x, fPosition.y);

        graphics.fill(1.0f);
        graphics.noStroke();
        graphics.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f);
        graphics.text("DISPLAY", -1, -2);
        graphics.noFill();

        if (!fDisplayOn) {
            graphics.fill(0.0f);
            graphics.noStroke();
            graphics.rect(0, 0, width, height);
        } else {
            if (display_is_double_buffered()) {
                display_swap_buffer();
            }
            mouseMoved(); // TODO move this to subscription model
            display_update_event();
            buffer.updatePixels(g_ptr);
            graphics.fill(brightness);
            graphics.noStroke();
            graphics.image(&buffer, 0, 0);
        }

        graphics.stroke(1);
        graphics.noFill();
        graphics.rect(-1, -1, width + 2, height + 2);

        graphics.popMatrix();
    }

    void mouseMoved() const {
        // TODO clamp to display size
        const float x = KlangstromEmulator::instance()->mouseX() - fPosition.x;
        const float y = KlangstromEmulator::instance()->mouseY() - fPosition.y;
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        TouchEvent touchevent;
        touchevent.number_of_touches = 1;
        touchevent.gesture_id        = 0;
        touchevent.x[0]              = KlangstromEmulator::instance()->mouseX() - fPosition.x;
        touchevent.y[0]              = KlangstromEmulator::instance()->mouseY() - fPosition.y;
        display_touch_event(&touchevent);
    }

    void set_position(const float x, const float y) {
        fPosition.set(x, y);
    }

    uint32_t* get_buffer() const {
        return fFrameBuffers[fActiveBuffer]->pixels;
    }

    void clear(const uint32_t color) const {
        const PImage&  buffer       = *fFrameBuffers[fActiveBuffer];
        uint32_t*      pixel_buffer = buffer.pixels;
        const uint32_t length       = buffer.width * buffer.height;
        for (uint32_t i = 0; i < length; i++) {
            pixel_buffer[i] = color;
        }
    }

    void set_pixel(const uint16_t x, const uint16_t y, const uint32_t color) const {
        const PImage& buffer = *fFrameBuffers[fActiveBuffer];
        buffer.set(x, y, color);
    }

    uint32_t get_pixel(const uint16_t x, const uint16_t y) const {
        const PImage& buffer = *fFrameBuffers[fActiveBuffer];
        return buffer.get(x, y);
    }

    void swap_buffer() {
        fActiveBuffer = (fActiveBuffer + 1) % 2;
    }

private:
    DrawableDisplay() = delete;
    PVector fPosition;
    PImage* fFrameBuffers[2];
    uint8_t fActiveBuffer = 0;
    bool    fDisplayOn    = true;
};

#ifdef __cplusplus
extern "C" {
#endif

static DrawableDisplay* display_ptr = nullptr;
static bool             initialized = false;

bool display_init_BSP(TouchPanelMode touch_panel_mode) {
    if (!initialized) {
        initialized = true;
        display_ptr = new DrawableDisplay(KLST_EMU_DISPLAY_WIDTH, KLST_EMU_DISPLAY_HEIGHT);
        display_ptr->set_position(KlangstromEmulator::instance()->get_width() - (KLST_EMU_DISPLAY_WIDTH + 20), 50);
        KlangstromEmulator::instance()->register_drawable(display_ptr);
    }
    return true;
}

void display_deinit() {
    if (initialized && display_ptr != nullptr) {
        KlangstromEmulator::instance()->unregister_drawable(display_ptr);
        delete display_ptr;
        display_ptr = nullptr;
    }
}

int16_t display_get_width() {
    // TODO make this platform specific
    if (display_ptr == nullptr) {
        return 0;
    }
    return display_ptr->width;
}

int16_t display_get_height() {
    // TODO make this platform specific
    if (display_ptr == nullptr) {
        return 0;
    }
    return display_ptr->height;
}

void display_set_backlight(const float brightness) {
    if (display_ptr == nullptr) {
        return;
    }
    display_ptr->brightness = brightness;
}

void display_enable_automatic_update(const bool sync_to_v_blank) { // TODO implement
    (void) sync_to_v_blank;
}

void display_request_reload() {
    warning_in_function_once("display_request_reload() called … not implement in KLST_EMU (WIP)");
}

void display_swap_buffer() {
    if (display_ptr == nullptr) {
        return;
    }
    display_ptr->swap_buffer();
}

void display_switch_on() { // TODO implement
}

void display_switch_off() { // TODO implement
}

volatile uint32_t display_get_buffer_address() { return 0; }

volatile uint32_t* display_get_buffer() { return display_ptr->get_buffer(); }

void touch_init(TouchPanelMode touch_panel_mode) { // TODO implement
    (void) touch_panel_mode;
}

void touch_read(TouchEvent* touchevent) { // TODO implement
    (void) touchevent;
}

bool touch_has_event() { // TODO implement
    return false;
}

#ifdef __cplusplus
}
#endif

/* ----------------------------------- DRAW LIBRARY ----------------------------------- */

// note that display works internally with ARGB while OpenGL require ABGR ( = RGBA in little endian?!? )

void display_clear_BSP(const uint32_t color) { // ARGB
    display_ptr->clear(KLST_DISPLAY_ARGB_TO_ABGR(color));
}

void display_set_pixel_BSP(const uint16_t x, const uint16_t y, const uint32_t color) {
    if (display_ptr == nullptr) {
        return;
    }
    display_ptr->set_pixel(x, y, KLST_DISPLAY_ARGB_TO_ABGR(color));
}

void display_set_pixel_alpha_BSP(const uint16_t x, const uint16_t y, const uint32_t color) {
    if (display_ptr == nullptr) {
        return;
    }
    const uint32_t color_ARGB         = color;
    const uint32_t current_color_ARGB = display_get_pixel_BSP(x, y);
    const float    alpha              = static_cast<float>(KLST_DISPLAY_GET_ALPHA(color_ARGB)) / 255.0f;
    const float    inv_alpha          = 1.0f - alpha;
    const auto     r                  = static_cast<uint8_t>((KLST_DISPLAY_GET_RED(color_ARGB) * alpha + KLST_DISPLAY_GET_RED(current_color_ARGB) * inv_alpha));
    const auto     g                  = static_cast<uint8_t>((KLST_DISPLAY_GET_GREEN(color_ARGB) * alpha + KLST_DISPLAY_GET_GREEN(current_color_ARGB) * inv_alpha));
    const auto     b                  = static_cast<uint8_t>((KLST_DISPLAY_GET_BLUE(color_ARGB) * alpha + KLST_DISPLAY_GET_BLUE(current_color_ARGB) * inv_alpha));
    display_ptr->set_pixel(x, y, umfeld::color(r / 255.0f, g / 255.0f, b / 255.0f));
}

uint32_t display_get_pixel_BSP(const uint16_t x, const uint16_t y) { return KLST_DISPLAY_ABGR_TO_ARGB(display_ptr->get_pixel(x, y)); }

void display_rect_fill_BSP(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint32_t color) {
    if (display_ptr == nullptr) {
        return;
    }
    const uint32_t color_rgba = KLST_DISPLAY_ARGB_TO_ABGR(color);
    for (uint16_t i = 0; i < width; i++) {
        for (uint16_t j = 0; j < height; j++) {
            display_ptr->set_pixel(x + i, y + j, color_rgba);
        }
    }
}

void display_line_horizontal_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color) {}

void display_line_vertical_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color) {}

void display_image_BSP(uint32_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {}

void display_char_BSP(BitmapFont* font, uint16_t x, uint16_t y, uint8_t ascii_char, uint32_t color, uint32_t background_color) {}

#endif // KLST_ARCH_IS_EMU