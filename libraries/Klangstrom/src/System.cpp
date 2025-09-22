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

#include <cstdint>
#include <stdbool.h>

#include "System.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO this must be made BSP
#define SYSTEM_INITIAL_NUM_AUDIO_DEVICES 3
#define SYSTEM_INITIAL_NUM_SERIAL_DEVICES 4
#define SYSTEM_INITIAL_NUM_PERIODIC_TIMERS 3

static ArrayList_AudioDevicePtr   _audio_device_listeners;
static ArrayList_SerialDevicePtr  _serial_device_listeners;
static ArrayList_GPIOListenerPtr  _gpio_listeners;
static ArrayList_PeriodicTimerPtr _periodic_timer_listeners;
static uint16_t                   _device_id;
static bool                       _system_initialized;
static uint32_t                   _system_start_time;

void system_init() {
    _device_id          = 0;
    _system_initialized = false;
    arraylist_AudioDevicePtr_init(&_audio_device_listeners, SYSTEM_INITIAL_NUM_AUDIO_DEVICES);
    arraylist_SerialDevicePtr_init(&_serial_device_listeners, SYSTEM_INITIAL_NUM_SERIAL_DEVICES);
    arraylist_PeriodicTimerPtr_init(&_periodic_timer_listeners, SYSTEM_INITIAL_NUM_PERIODIC_TIMERS);
    system_init_BSP();
    console_clear();
    console_system_info();

    console_status("Sub-System (ASP/BSP) initialized");
    console_status("System initialized%s", KLST_CONSOLE_LINE_ENDING);

    _system_initialized = true;
    _system_start_time  = system_get_ticks_BSP();
}

bool system_is_initialized() {
    return _system_initialized;
}

uint16_t system_get_unique_device_ID() {
    return _device_id++;
}

void system_register_audiodevice(AudioDevice* audiodevice) {
    arraylist_AudioDevicePtr_add(&_audio_device_listeners, audiodevice);
}

ArrayList_AudioDevicePtr* system_get_registered_audiodevices() {
    return &_audio_device_listeners;
}

void system_register_serialdevice(SerialDevice* serialdevice) {
    arraylist_SerialDevicePtr_add(&_serial_device_listeners, serialdevice);
}

ArrayList_SerialDevicePtr* system_get_registered_serialdevices() {
    return &_serial_device_listeners;
}

void system_register_gpio_listener(GPIOListener* gpio_listener) {
    arraylist_GPIOListenerPtr_add(&_gpio_listeners, gpio_listener);
}

ArrayList_GPIOListenerPtr* system_get_registered_gpio_listener() {
    return &_gpio_listeners;
}

void system_register_periodic_timer(PeriodicTimer* timer) {
    arraylist_PeriodicTimerPtr_add(&_periodic_timer_listeners, timer);
}

ArrayList_PeriodicTimerPtr* system_get_registered_periodic_timers() {
    return &_periodic_timer_listeners;
}

uint32_t system_get_ticks() {
    return system_get_ticks_BSP() - _system_start_time;
}

float system_cycles_to_micros(const uint32_t cycles) {
    return static_cast<float>(cycles) / (static_cast<float>(system_clock_frequency()) / 1000000.0f);
}

/*
 * Memory Allocator :: Simple monotonic allocator for external memory. Aligns allocations, supports reset, no per-block free.
 */

#ifndef SYSTEM_EXTERNAL_MEMORY_DEFAULT_ALIGNMENT
#define SYSTEM_EXTERNAL_MEMORY_DEFAULT_ALIGNMENT 8u
#endif

static uintptr_t system_external_memory_base_address    = 0;
static uintptr_t system_external_memory_current_address = 0;
static uintptr_t system_external_memory_end_address     = 0;

static uintptr_t align_up_uintptr(const uintptr_t value, const uintptr_t alignment) {
    return (value + (alignment - 1u)) & ~(alignment - 1u);
}

void system_external_memory_init(const uintptr_t base_address, const size_t total_size) {
    system_external_memory_base_address    = base_address;
    system_external_memory_current_address = base_address;
    system_external_memory_end_address     = base_address + total_size;
}

void system_external_memory_reset() {
    system_external_memory_current_address = system_external_memory_base_address;
}

size_t system_external_memory_available() {
    if (system_external_memory_end_address < system_external_memory_current_address) {
        return 0u;
    }
    return system_external_memory_end_address - system_external_memory_current_address;
}

size_t system_external_memory_used() {
    if (system_external_memory_current_address < system_external_memory_base_address) {
        return 0u;
    }
    return system_external_memory_current_address - system_external_memory_base_address;
}

bool system_external_memory_is_in_range(const void* pointer) {
    const auto address = reinterpret_cast<uintptr_t>(pointer);
    return (address >= system_external_memory_base_address) && (address < system_external_memory_end_address);
}

void* system_external_memory_allocate(const size_t size) {
    const uintptr_t aligned = align_up_uintptr(system_external_memory_current_address, SYSTEM_EXTERNAL_MEMORY_DEFAULT_ALIGNMENT);
    if (aligned > system_external_memory_end_address) {
        return nullptr;
    }
    if (size > system_external_memory_end_address - aligned) {
        return nullptr;
    }
    system_external_memory_current_address = aligned + size;
    return reinterpret_cast<void*>(aligned);
}

void* system_external_memory_allocate_aligned(const size_t size, const size_t alignment) {
    if ((alignment == 0u) || (alignment & (alignment - 1u)) != 0u) {
        return nullptr; // must be power of two
    }
    const uintptr_t aligned = align_up_uintptr(system_external_memory_current_address, alignment);
    if (aligned > system_external_memory_end_address) {
        return nullptr;
    }
    if (size > system_external_memory_end_address - aligned) {
        return nullptr;
    }
    system_external_memory_current_address = aligned + size;
    return reinterpret_cast<void*>(aligned);
}

#ifdef __cplusplus
}
#endif
