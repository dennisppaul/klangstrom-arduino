---
layout: libdoc
title: System
permalink: /system/
index: 101
---

`system_init()` should be called, before any other peripherals are initialized or used.

```c
void                       system_init()
AudioDevice*               system_init_audiocodec();

void                       system_register_audiodevice(AudioDevice* audiodevice)
ArrayList_AudioDevicePtr*  system_get_registered_audiodevices()
void                       system_register_serialdevice(SerialDevice* serialdevice)
ArrayList_SerialDevicePtr* system_get_registered_serialdevices()
void                       system_register_gpio_listener(GPIOListener* gpio_listener)
ArrayList_GPIOListenerPtr* system_get_registered_gpio_listener()
void                       system_register_timer(Timer* timer)
ArrayList_TimerPtr*        system_get_registered_timer()
bool                       system_is_initialized()
uint32_t                   system_get_ticks()
void                       system_enable_cycle_counter(bool enable)
void                       system_reset_cycles()
uint32_t                   system_get_cycles()
uint32_t                   system_clock_frequency()
float                      system_cycles_to_micros(uint32_t cycles)

void                       system_external_memory_init(uintptr_t base_address, size_t total_size)
void                       system_external_memory_reset()
size_t                     system_external_memory_available()
size_t                     system_external_memory_used()
bool                       system_external_memory_is_in_range(const void* pointer)
void*                      system_external_memory_allocate(size_t size)
inline float*              system_external_memory_allocate_float_array(const size_t size)
inline uint32_t*           system_external_memory_allocate_uint32_array(const size_t size)
void*                      system_external_memory_allocate_aligned(size_t size, size_t alignment)
```

⚠️⚠️⚠️ WIP ⚠️⚠️⚠️

- `system_jump_to_bootloader()` still working?
- DEMO mit `PeripheralConfiguration_CUSTOM.h`?!? ( "activate specfic peripherals:" )
    ```c
    #define KLST_PERIPHERAL_ENABLE_GPIO
    #define KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG
    #define KLST_PERIPHERAL_ENABLE_AUDIODEVICE
    #define KLST_PERIPHERAL_ENABLE_LEDS
    #define KLST_PERIPHERAL_ENABLE_SD_CARD
    #define KLST_PERIPHERAL_ENABLE_IDC_SERIAL
    #define KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC
    #define KLST_PERIPHERAL_ENABLE_TIMERS
    #define KLST_PERIPHERAL_ENABLE_ENCODER
    #define KLST_PERIPHERAL_ENABLE_MECHANICAL_KEYS
    #define KLST_PERIPHERAL_ENABLE_MIDI
    #define KLST_PERIPHERAL_ENABLE_ADC_DAC
    #define KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY
    #define KLST_PERIPHERAL_ENABLE_DISPLAY
    ```

## Example

```c
#include "Arduino.h"
#include "Klangstrom.h"
#include "System.h"

void setup() {
    system_init();
}

void loop() {}
```