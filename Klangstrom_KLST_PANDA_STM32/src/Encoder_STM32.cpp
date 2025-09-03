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
#ifdef KLST_PERIPHERAL_ENABLE_ENCODER
#ifdef KLST_ARCH_IS_STM32

#include <string>

#include "main.h"
#include "tim.h"
#include "Encoder.h"
#include "Encoder_STM32.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

static void enable_rotation_interrupts(const EncoderPeripherals* r) {
    __HAL_TIM_ENABLE_IT(r->timer_handle, TIM_IT_CC1);
    __HAL_TIM_ENABLE_IT(r->timer_handle, TIM_IT_CC2);
}

bool encoder_init_peripherals_BSP(Encoder* encoder) {
    encoder->peripherals            = new EncoderPeripherals();
    EncoderPeripherals& peripherals = *encoder->peripherals;
    if (encoder->device_type == ENCODER_LEFT) {
        peripherals.timer_handle        = &htim1;
        peripherals.button_channel      = HAL_TIM_ACTIVE_CHANNEL_3;
        peripherals.button_channel_i    = TIM_CHANNEL_3;
        peripherals.gpio_port           = _ENCODER_00_BUTTON_GPIO_Port;
        peripherals.gpio_pin            = _ENCODER_00_BUTTON_Pin;
        peripherals.encoder_channel_a   = HAL_TIM_ACTIVE_CHANNEL_1;
        peripherals.encoder_channel_a_i = TIM_CHANNEL_1;
        peripherals.encoder_channel_b   = HAL_TIM_ACTIVE_CHANNEL_2;
        peripherals.encoder_channel_a_i = TIM_CHANNEL_2;
        peripherals.bsp_init            = MX_TIM1_Init;
        return true;
    }
    if (encoder->device_type == ENCODER_RIGHT) {
        peripherals.timer_handle        = &htim2;
        peripherals.button_channel      = HAL_TIM_ACTIVE_CHANNEL_4;
        peripherals.button_channel_i    = TIM_CHANNEL_4;
        peripherals.gpio_port           = _ENCODER_01_BUTTON_GPIO_Port;
        peripherals.gpio_pin            = _ENCODER_01_BUTTON_Pin;
        peripherals.encoder_channel_a   = HAL_TIM_ACTIVE_CHANNEL_1;
        peripherals.encoder_channel_a_i = TIM_CHANNEL_1;
        peripherals.encoder_channel_b   = HAL_TIM_ACTIVE_CHANNEL_2;
        peripherals.encoder_channel_b_i = TIM_CHANNEL_2;
        peripherals.bsp_init            = MX_TIM2_Init;
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
        // console_status("device type: %s", encoder->device_type == ENCODER_LEFT ? "ENCODER_LEFT" : "ENCODER_RIGHT");
        encoder->peripherals->bsp_init();
        enable_rotation_interrupts(encoder->peripherals);
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
    const auto& peripherals = *encoder->peripherals;
    HAL_TIM_Encoder_Start(peripherals.timer_handle, peripherals.encoder_channel_a_i);
    HAL_TIM_Encoder_Start(peripherals.timer_handle, peripherals.encoder_channel_b_i);
    HAL_TIM_IC_Start_IT(peripherals.timer_handle, peripherals.button_channel_i);
    // HAL_TIM_IC_Start_IT(peripherals.timer_handle, peripherals.encoder_channel_a_i); // TODO not needed?
    // HAL_TIM_IC_Start_IT(peripherals.timer_handle, peripherals.encoder_channel_b_i); // TODO not needed?
    encoder->started = true;
}

void encoder_stop(Encoder* encoder) {
    const auto& peripherals = *encoder->peripherals;
    HAL_TIM_Encoder_Stop(peripherals.timer_handle, peripherals.encoder_channel_a_i);
    HAL_TIM_Encoder_Stop(peripherals.timer_handle, peripherals.encoder_channel_b_i);
    HAL_TIM_IC_Stop_IT(peripherals.timer_handle, peripherals.button_channel_i);
    encoder->started = false;
}

static int32_t get_encoder_value(const TIM_HandleTypeDef* htim) {
    return static_cast<int16_t>(__HAL_TIM_GET_COUNTER(htim)) * -1;
}

// int32_t encoder_get_rotation(const Encoder* encoder) {
//     if (encoder->peripherals->timer_handle == nullptr) {
//         return 0;
//     }
//     return get_encoder_value(encoder->peripherals->timer_handle);
// }

static uint8_t get_event_type(const TIM_HandleTypeDef& htim, const EncoderPeripherals& r) {
    if (htim.Channel == r.button_channel) {
        return ENCODER_EVENT_BUTTON;
    }
    if (htim.Channel == r.encoder_channel_a || htim.Channel == r.encoder_channel_b) {
        return ENCODER_EVENT_ROTATION;
    }
    return ENCODER_EVENT_NONE;
}

/**
 * event handler for encoder ( button and rotation )
 * TODO maybe move this to System if other peripherals need timer interrupts too
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim) {
    const ArrayList_EncoderPtr& listeners = *encoder_get_listeners();
    for (size_t i = 0; i < listeners.size; i++) {
        Encoder*                  encoder    = listeners.data[i];
        const EncoderPeripherals& peripheral = *encoder->peripherals;
        if (peripheral.timer_handle == htim) {
            const auto event_type   = get_event_type(*htim, peripheral);
            const auto button_state = !HAL_GPIO_ReadPin(peripheral.gpio_port, peripheral.gpio_pin);
            encoder->rotation       = get_encoder_value(htim);
            encoder->button_pressed = button_state;
            encoder->callback(encoder, event_type);
        }
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_ENCODER