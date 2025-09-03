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

#include "System.h"
#include "GPIOListener.h"
#include "Key.h"

#ifdef __cplusplus
extern "C" {
#endif

static GPIOListener fGPIOListener;

WEAK void key_event(const Key* key) {
    (void) key;
}

static ArrayList_KeyPtr fKeyListeners;
static bool             is_initialized = false;

void key_init(const uint8_t number_of_keys) {
    arraylist_GPIOListenerPtr_init(system_get_registered_gpio_listener(), number_of_keys);
    fGPIOListener.callback = key_callback_BSP;
    system_register_gpio_listener(&fGPIOListener);
    is_initialized = true;
}

Key* key_create(const uint8_t device_type) {
    if (!is_initialized) {
        key_init();
    }
    auto* key          = new Key();
    key->device_type   = device_type;
    key->device_id     = system_get_unique_device_ID();
    key->callback      = key_event;
    const bool init_ok = key_init_BSP(key);
    if (init_ok && key_init_peripherals_BSP(key)) {
        key_register_listener(key);
    } else {
        key->device_type = KEY_INIT_INCOMPLETE;
    }
    return key;
}

void key_delete(Key* key) {
    key_deinit_peripherals_BSP(key);
    // key_deinit_BSP(key);
    delete key;
}

void key_register_listener(Key* key) {
    arraylist_KeyPtr_add(&fKeyListeners, key);
}

ArrayList_KeyPtr* key_get_listeners() {
    return &fKeyListeners;
}

#ifdef __cplusplus
}
#endif
