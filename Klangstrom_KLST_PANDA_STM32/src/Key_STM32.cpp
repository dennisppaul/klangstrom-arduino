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
#ifdef KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS
#ifdef KLST_ARCH_IS_STM32

#include "main.h"
#include "Key.h"
#include "Key_STM32.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

void key_callback_BSP(const uint16_t GPIO_Pin) {
    ArrayList_KeyPtr* fKeyListeners = key_get_listeners();
    for (size_t i = 0; i < fKeyListeners->size; i++) {
        Key* key = arraylist_KeyPtr_get(fKeyListeners, i);
        if (key != nullptr && key->peripherals != nullptr) {
            if (key->peripherals->gpio_pin == GPIO_Pin) {
                key->pressed = !HAL_GPIO_ReadPin(key->peripherals->gpio_port, key->peripherals->gpio_pin);
                key_event(key);
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
    return false;
}

void key_deinit_peripherals_BSP(Key* key) {
    if (key->peripherals != nullptr) {
        delete key->peripherals;
        key->peripherals = nullptr;
    }
}

bool key_init_BSP(Key* key) {
    return true;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS
