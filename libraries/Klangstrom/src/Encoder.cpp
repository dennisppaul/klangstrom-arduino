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
#include "Encoder.h"

#ifdef __cplusplus
extern "C" {
#endif

WEAK void encoder_event(Encoder* encoder, uint8_t event) {
    (void) encoder;
    (void) event;
}

static ArrayList_EncoderPtr fEncoderListeners;

Encoder* encoder_create(const uint8_t device_type) {
    auto* encoder                  = new Encoder();
    encoder->device_type           = device_type;
    encoder->device_id             = system_get_unique_device_ID();
    encoder->callback              = encoder_event;
    const bool init_peripherals_ok = encoder_init_peripherals_BSP(encoder);
    const bool init_ok             = encoder_init_BSP(encoder);
    if (init_peripherals_ok && init_ok) {
        encoder_register_listener(encoder);
        encoder_start(encoder);
    } else {
        encoder->device_type = ENCODER_INIT_INCOMPLETE;
    }

    return encoder;
}

void encoder_delete(Encoder* encoder) {
    encoder_deinit_peripherals_BSP(encoder);
    encoder_deinit_BSP(encoder);
    delete encoder;
}

void encoder_register_listener(Encoder* encoder) {
    arraylist_EncoderPtr_add(&fEncoderListeners, encoder);
}

ArrayList_EncoderPtr* encoder_get_listeners() {
    return &fEncoderListeners;
}

#ifdef __cplusplus
}
#endif
