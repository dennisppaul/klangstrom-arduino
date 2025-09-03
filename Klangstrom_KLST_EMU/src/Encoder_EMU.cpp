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

#define KLST_PERIPHERAL_ENABLE_ENCODER
#warning "KLST_PERIPHERAL_ENABLE_ENCODER is currently always defined for KLST_EMU … this needs to be fixed"

#ifdef KLST_PERIPHERAL_ENABLE_ENCODER
#ifdef KLST_ARCH_IS_EMU

#include <string>

#include "Encoder.h"
#include "Encoder_EMU.h"
#include "Console.h"

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


#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
#endif // KLST_PERIPHERAL_ENABLE_ENCODER