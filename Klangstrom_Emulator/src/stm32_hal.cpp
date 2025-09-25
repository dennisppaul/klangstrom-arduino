
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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#include <iostream>
#include <chrono>

#include "stm32_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

using namespace std::chrono;

const static uint64_t mStartTimeOffset = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();

#define GPIO_PORT_COUNT 8  // Ports A-H
#define GPIO_PIN_COUNT  16 // Pins 0-15

typedef struct {
    uint16_t port_states[GPIO_PORT_COUNT]; // Each port has 16 pins represented by a 16-bit value
} GPIO_Emulator;
static GPIO_Emulator gpio = {0};

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, const uint16_t GPIO_Pin, const GPIO_PinState PinState) {
    if (GPIOx == nullptr) {
        printf("Invalid GPIOx\n");
    }
    const int port_index = GPIOx->ID;
    if (port_index < 0 || GPIO_Pin >= GPIO_PIN_COUNT) {
        printf("Invalid port or pin\n");
        return;
    }

    if (PinState == GPIO_PIN_SET) {
        gpio.port_states[port_index] |= (1 << GPIO_Pin); // Set the bit
    } else {
        gpio.port_states[port_index] &= ~(1 << GPIO_Pin); // Clear the bit
    }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, const uint16_t GPIO_Pin) {
    if (GPIOx == nullptr) {
        printf("Invalid GPIOx\n");
        return GPIO_PIN_RESET;
    }
    const int port_index = GPIOx->ID;
    if (port_index < 0 || GPIO_Pin >= GPIO_PIN_COUNT) {
        printf("Invalid port or pin\n");
        return GPIO_PIN_RESET;
    }

    return (gpio.port_states[port_index] & (1 << GPIO_Pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}

void GPIO_PrintPorts() {
    for (int i = 0; i < GPIO_PORT_COUNT; i++) {
        printf("Port %c: 0x%04X\n", ('A' + i), gpio.port_states[i]);
    }
}

uint32_t HAL_GetTick() {
    const uint64_t mCurrentTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    const uint32_t mDelta       = mCurrentTime - mStartTimeOffset;
    return mDelta;
}

uint32_t HAL_RCC_GetSysClockFreq() {
#if defined(GENERIC_EMU)
    return 0;
#elif defined(KLST_CORE_EMU)
    return 0;
#elif defined(KLST_TINY_EMU)
    return 0;
#elif defined(KLST_SHEEP_EMU)
    return 0;
#elif defined(KLST_PANDA_EMU)
    return 550000000;
#elif defined(KLST_CATERPILLAR_EMU)
    return 550000000;
#else
    return 0;
#endif
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU
