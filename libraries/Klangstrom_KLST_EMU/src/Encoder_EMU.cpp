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

#include "Klangstrom.h"

#define KLST_PERIPHERAL_ENABLE_ENCODER
#warning "KLST_PERIPHERAL_ENABLE_ENCODER is currently always defined for KLST_EMU … this needs to be fixed"

#ifdef KLST_PERIPHERAL_ENABLE_ENCODER
#ifdef KLST_ARCH_IS_EMU

#include <string>

#include "stm32_hal.h"

#include "Drawable.h"
#include "KlangstromEmulator.h"
#include "Encoder.h"
#include "Encoder_EMU.h"
#include "Console.h"

class DrawableEncoder final : public Drawable {
public:
    explicit DrawableEncoder(Encoder* encoder) : _encoder(encoder) { (void)_encoder; }

    void draw(umfeld::PGraphics* g) override {
#ifdef KLST_ENCODER_WIP
        g->pushMatrix();
        g->translate(_position.x, _position.y);
        g->textSize(12);
        g->fill(1.0f);
        g->noStroke();
        g->text(_name.c_str(), 0, -30);
        g->text(("rot: " + std::to_string(_encoder->rotation)).c_str(), 0, -15);
        g->fill(1);
        g->noStroke();
        g->ellipse(0, 0, _radius, _radius);
        g->popMatrix();
#endif
    }

    void set_name(const std::string& name) {
        _name = name;
    }

    void set_position(const float x, const float y) {
        _position.x = x;
        _position.y = y;
    }

private:
    Encoder*    _encoder;
#ifdef KLST_ENCODER_WIP
    float       _radius = 40;
#endif
    std::string _name;
    struct {
        float x{0};
        float y{0};
    } _position;
};

#ifdef __cplusplus
extern "C" {
#endif

bool encoder_init_peripherals_BSP(Encoder* encoder) {
    encoder->peripherals            = new EncoderPeripherals();
    EncoderPeripherals& peripherals = *encoder->peripherals;
    if (encoder->device_type == ENCODER_LEFT) {
        return true;
    }
    if (encoder->device_type == ENCODER_RIGHT) {
        return true;
    }
    if (encoder->device_type == ENCODER_TYPE_CUSTOM) {
        return true;
    }
    delete encoder->peripherals;
    return false;
}

bool encoder_init_BSP(Encoder* encoder) {
    if (encoder->peripherals == nullptr) {
        console_error("ERROR: peripherals not initialized");
        return false;
    }
    if (encoder->device_type == ENCODER_LEFT || encoder->device_type == ENCODER_RIGHT) {
        if (encoder->device_type == ENCODER_RIGHT) {
            static bool initialized = false;
            if (!initialized) {
                initialized         = true;
                const auto drawable = new DrawableEncoder(encoder);
                drawable->set_name("Encoder RIGHT");
                drawable->set_position(200, 200);
                umfeld::KlangstromEmulator::instance()->register_drawable(drawable);
            }
        }
        if (encoder->device_type == ENCODER_LEFT) {
            static bool initialized = false;
            if (!initialized) {
                initialized         = true;
                const auto drawable = new DrawableEncoder(encoder);
                drawable->set_name("Encoder LEFT");
                drawable->set_position(150, 200);
                umfeld::KlangstromEmulator::instance()->register_drawable(drawable);
            }
        }
        return true;
    }
    if (encoder->device_type == ENCODER_TYPE_CUSTOM) {
        console_status("device type: custom(%i) ( device must be intialized manually )", encoder->device_type);
        return false;
    }
    return false;
}

void encoder_deinit_BSP(Encoder* encoder) {
}

void encoder_deinit_peripherals_BSP(Encoder* encoder) {
    delete encoder->peripherals;
}

void encoder_start(Encoder* encoder) {
}

void encoder_stop(Encoder* encoder) {
}

/**
 * event handler for encoder ( button and rotation )
 * TODO maybe move this to System if other peripherals need timer interrupts too
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    const ArrayList_EncoderPtr& listeners = *encoder_get_listeners();
    console_println("HAL_TIM_IC_CaptureCallback: timer interrupt received");
    // for (size_t i = 0; i < listeners.size; i++) {
    //     Encoder*                  encoder    = listeners.data[i];
    //     const EncoderPeripherals& peripheral = *encoder->peripherals;
    //     if (peripheral.timer_handle == htim) {
    //         const auto event_type   = get_event_type(*htim, peripheral);
    //         const auto button_state = !HAL_GPIO_ReadPin(peripheral.gpio_port, peripheral.gpio_pin);
    //         encoder->rotation       = get_encoder_value(htim);
    //         encoder->button_pressed = button_state;
    //         encoder->callback(encoder, event_type);
    //     }
    // }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
#endif // KLST_PERIPHERAL_ENABLE_ENCODER