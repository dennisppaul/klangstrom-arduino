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

#define KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS
#warning "KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS is currently always defined for KLST_EMU … this needs to be fixed"

#ifdef KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS
#ifdef KLST_ARCH_IS_EMU

#include <string.h>
#include "KlangstromEmulator.h"
#include "Key.h"
#include "Key_EMU.h"
#include "Console.h"
#include "PGraphics.h"
#include "stm32_hal.h"

using namespace umfeld;

class DrawableKey final : public Drawable {
public:
    float key_widget_position_x = 20;
    float key_widget_position_y = 38 * 16;

    explicit DrawableKey(Key* key) : key_ptr(key) {
        position_x = o_position_x;
        o_position_x += umfeld::KlangstromEmulator::DEFAULT_FONT_SIZE;
    }

    void draw(PGraphics* g_ptr) override {
        PGraphics& graphics = *g_ptr;

        graphics.pushMatrix();
        graphics.translate(key_widget_position_x + position_x, key_widget_position_y);

        graphics.fill(1.0f);
        graphics.noStroke();
        graphics.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f);
        graphics.text("KEYS", -1, -2);
        graphics.noFill();

        graphics.stroke(1.0f);
        if (key_ptr->pressed) {
            graphics.fill(1.0f);
        } else {
            graphics.noFill();
        }
        constexpr float mWidth = KlangstromEmulator::DEFAULT_FONT_SIZE - 4;
        graphics.rect(0, 0, mWidth, mWidth);

        graphics.fill(1.0f);
        graphics.noStroke();
        graphics.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5);
        const std::string mID = std::to_string(key_ptr->device_id);
        graphics.text(mID.c_str(), mWidth / 2 - 4, KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f + 3);

        graphics.popMatrix();
    }

private:
    Key*         key_ptr;
    float        position_x;
    static float o_position_x;
};

float DrawableKey::o_position_x = 0;

#ifdef __cplusplus
extern "C" {
#endif

void key_callback_BSP(const uint16_t GPIO_Pin) {
    ArrayList_KeyPtr* fKeyListeners = key_get_listeners();
    for (size_t i = 0; i < fKeyListeners->size; i++) {
        Key* _key = arraylist_KeyPtr_get(fKeyListeners, i);
        if (_key != nullptr && _key->peripherals != nullptr) {
            if (_key->peripherals->gpio_pin == GPIO_Pin) {
                _key->pressed = HAL_GPIO_ReadPin(_key->peripherals->gpio_port, _key->peripherals->gpio_pin);
                key_event(_key);
            }
        }
    }
}

bool key_init_peripherals_BSP(Key* key) {
    key->peripherals = new KeyPeripherals();
    if (key->device_type == KEY_00) {
        KeyPeripherals& peripherals = *key->peripherals;
        peripherals.gpio_port       = _MECH_BUTTON_00_GPIO_Port;
        peripherals.gpio_pin        = _MECH_BUTTON_00_Pin;
        return true;
    }
    if (key->device_type == KEY_01) {
        KeyPeripherals& peripherals = *key->peripherals;
        peripherals.gpio_port       = _MECH_BUTTON_01_GPIO_Port;
        peripherals.gpio_pin        = _MECH_BUTTON_01_Pin;
        return true;
    }
    if (key->device_type == KEY_TYPE_CUSTOM) {
        console_status("device type: custom(%i) ( device must be intialized manually )", key->device_type);
        return true;
    }
    console_error("device type: unknown(%i)", key->device_type);
    delete key->peripherals;
    return false;
}

void key_deinit_peripherals_BSP(Key* key) {
    if (key->peripherals != nullptr) {
        delete key->peripherals;
        key->peripherals = nullptr;
    }
}

#ifdef __cplusplus
}
#endif

bool key_init_BSP(Key* key) {
    if (key == nullptr) {
        console_error("key is nullptr");
        return false;
    }
    static bool initialized = false;
    if (!initialized) {
        KlangstromEmulator::instance()->register_drawable(new DrawableKey(key));
        return true;
    }
    return false;
}

#endif // KLST_ARCH_IS_EMU
#endif // KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS
