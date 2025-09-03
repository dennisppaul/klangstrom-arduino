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

#pragma once

#include <cstdint>

#include "Klangstrom.h"
#include "AudioDevice.h"
#include "SerialDevice.h"
#include "ArrayList.h"
#include "Tools.h"
#include "GPIOListener.h"
#include "Timer.h"
#include "SystemUID.h"

#ifdef __cplusplus
extern "C" {
#endif

DEFINE_ARRAYLIST(GPIOListener*, GPIOListenerPtr)
DEFINE_ARRAYLIST(AudioDevice*, AudioDevicePtr)
DEFINE_ARRAYLIST(SerialDevice*, SerialDevicePtr)
DEFINE_ARRAYLIST(Timer*, TimerPtr)

void                       system_init();
void                       system_register_audiodevice(AudioDevice* audiodevice);
ArrayList_AudioDevicePtr*  system_get_registered_audiodevices();
void                       system_register_serialdevice(SerialDevice* serialdevice);
ArrayList_SerialDevicePtr* system_get_registered_serialdevices();
void                       system_register_gpio_listener(GPIOListener* gpio_listener);
ArrayList_GPIOListenerPtr* system_get_registered_gpio_listener();
void                       system_register_timer(Timer* timer);
ArrayList_TimerPtr*        system_get_registered_timer();
uint16_t                   system_get_unique_device_ID();
bool                       system_is_initialized();
uint32_t                   system_get_ticks();
void                       system_enable_cycle_counter(bool enable); // implemented as BSP
void                       system_reset_cycles();                    // implemented as BSP
uint32_t                   system_get_cycles();                      // implemented as BSP
uint32_t                   system_clock_frequency();                 // implemented as BSP
float                      system_cycles_to_micros(uint32_t cycles);

/**
 * @brief initialize the default audio codec and starts it
 * @return instance to audio device (audio codec). needs to be freed by caller
 */
AudioDevice* system_init_audiocodec(); // NOTE implemented as BSP for now

void     system_init_BSP();
uint32_t system_get_ticks_BSP();

#ifdef __cplusplus
}
#endif
