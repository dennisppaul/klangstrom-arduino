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
#include "PVector.h"
#include "KlangstromEmulator.h"
#include "Display.h"
#include "DisplayDrawInterface.h"

class DrawableDisplay final : public Drawable {
public:
    const float width;
    const float height;

    DrawableDisplay(const uint16_t display_width,
                    const uint16_t display_height) : width(display_width),
                                                     height(display_height), fFrameBuffers{} {
        fFrameBuffers[0] = new umfeld::PImage(display_width, display_height);
        fFrameBuffers[1] = new umfeld::PImage(display_width, display_height);

        // subscribe to `mouseMoved()`
    }

    void draw(umfeld::PGraphics* g_ptr) override {
        umfeld::PGraphics& g      = *g_ptr;
        umfeld::PImage&    buffer = *fFrameBuffers[fActiveBuffer];

        g.pushMatrix();
        g.translate(fPosition.x, fPosition.y);

        g.fill(1.0f);
        g.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f);
        g.text("DISPLAY", -1, -2);
        g.noFill();

        if (!fDisplayOn) {
            g.fill(0.0f);
            g.noStroke();
            g.rect(0, 0, width, height);
        } else {
            display_swap_buffer();
            mouseMoved(); // TODO move this to subscription model
            display_update_event();
            buffer.updatePixels(g_ptr);
            g.image(&buffer, 0, 0);
        }

        g.stroke(1, 0.5f);
        g.rect(-1, -1, width + 2, height + 2);

        g.popMatrix();
    }

    void mouseMoved() const {
        TouchEvent touchevent;
        touchevent.number_of_touches = 1;
        touchevent.gesture_id        = 0;
        touchevent.x[0]              = KlangstromEmulator::instance()->mouseX - fPosition.x;
        touchevent.y[0]              = KlangstromEmulator::instance()->mouseY - fPosition.y;
        display_touch_event(&touchevent);
    }

    void set_position(const float x, const float y) {
        fPosition.set(x, y);
    }

    uint32_t* get_buffer() const {
        return fFrameBuffers[fActiveBuffer]->pixels;
    }

    void clear(const uint32_t color) const {
        const umfeld::PImage&  buffer       = *fFrameBuffers[fActiveBuffer];
        uint32_t*      pixel_buffer = buffer.pixels;
        const uint32_t length       = buffer.width * buffer.height;
        for (uint32_t i = 0; i < length; i++) {
            pixel_buffer[i] = color;
        }
    }

    void set_pixel(const uint16_t x, const uint16_t y, const uint32_t color) const {
        const umfeld::PImage& buffer = *fFrameBuffers[fActiveBuffer];
        buffer.set(x, y, color);
    }

    uint32_t get_pixel(const uint16_t x, const uint16_t y) const {
        const umfeld::PImage& buffer = *fFrameBuffers[fActiveBuffer];
        return buffer.get(x, y);
    }

    void swap_buffer() {
        fActiveBuffer = (fActiveBuffer + 1) % 2;
    }

private:
    DrawableDisplay() = delete;
    umfeld::PVector fPosition;
    umfeld::PImage* fFrameBuffers[2];
    uint8_t fActiveBuffer = 0;
    bool    fDisplayOn    = true;
};

#ifdef __cplusplus
extern "C" {
#endif

static DrawableDisplay* display     = nullptr;
static bool             initialized = false;

bool display_init_BSP(TouchPanelMode touch_panel_mode) {
    if (!initialized) {
        initialized = true;
        display     = new DrawableDisplay(KLST_EMU_DISPLAY_WIDTH, KLST_EMU_DISPLAY_HEIGHT);
        display->set_position(KlangstromEmulator::instance()->width - (KLST_EMU_DISPLAY_WIDTH + 20), 50);
        KlangstromEmulator::instance()->register_drawable(display);
    }
    return true;
}

void display_deinit() {
    if (initialized && display != nullptr) {
        KlangstromEmulator::instance()->unregister_drawable(display);
        delete display;
        display = nullptr;
    }
}

int16_t display_get_width() {
    // TODO make this platform specific
    if (display == nullptr) {
        return 0;
    }
    return display->width;
}

int16_t display_get_height() {
    // TODO make this platform specific
    if (display == nullptr) {
        return 0;
    }
    return display->height;
}

void display_set_backlight(const float brightness) { // TODO implement
    (void) brightness;
}

void display_enable_automatic_update(const bool sync_to_v_blank) { // TODO implement
    (void) sync_to_v_blank;
}

void display_swap_buffer() {
    if (display == nullptr) {
        return;
    }
    display->swap_buffer();
}

void display_switch_on() { // TODO implement
}

void display_switch_off() { // TODO implement
}

volatile uint32_t display_get_buffer_address() { return 0; }

volatile uint32_t* display_get_buffer() { return display->get_buffer(); }

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
    display->clear(ARGB_TO_ABGR(color));
}

void display_set_pixel_BSP(const uint16_t x, const uint16_t y, const uint32_t color) {
    if (display == nullptr) {
        return;
    }
    display->set_pixel(x, y, ARGB_TO_ABGR(color));
}

void display_set_pixel_alpha_BSP(const uint16_t x, const uint16_t y, const uint32_t color) {
    if (display == nullptr) {
        return;
    }
    const uint32_t color_ARGB         = color;
    const uint32_t current_color_ARGB = display_get_pixel_BSP(x, y);
    const float    alpha              = static_cast<float>(GET_ALPHA(color_ARGB)) / 255.0f;
    const float    inv_alpha          = 1.0f - alpha;
    const uint8_t  r                  = static_cast<uint8_t>((GET_RED(color_ARGB) * alpha + GET_RED(current_color_ARGB) * inv_alpha));
    const uint8_t  g                  = static_cast<uint8_t>((GET_GREEN(color_ARGB) * alpha + GET_GREEN(current_color_ARGB) * inv_alpha));
    const uint8_t  b                  = static_cast<uint8_t>((GET_BLUE(color_ARGB) * alpha + GET_BLUE(current_color_ARGB) * inv_alpha));
    display->set_pixel(x, y, umfeld::color(r / 255.0f, g / 255.0f, b / 255.0f));
}

uint32_t display_get_pixel_BSP(const uint16_t x, const uint16_t y) { return ABGR_TO_ARGB(display->get_pixel(x, y)); }

void display_rect_fill_BSP(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const uint32_t color) {
    if (display == nullptr) {
        return;
    }
    const uint32_t color_rgba = ARGB_TO_ABGR(color);
    for (uint16_t i = 0; i < width; i++) {
        for (uint16_t j = 0; j < height; j++) {
            display->set_pixel(x + i, y + j, color_rgba);
        }
    }
}

void display_line_horizontal_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color) {}

void display_line_vertical_BSP(uint16_t x, uint16_t y, uint16_t length, uint32_t color) {}

void display_image_BSP(uint32_t* data, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {}

void display_char_BSP(BitmapFont* font, uint16_t x, uint16_t y, uint8_t ascii_char, uint32_t color, uint32_t background_color) {}

#endif // KLST_ARCH_IS_EMU