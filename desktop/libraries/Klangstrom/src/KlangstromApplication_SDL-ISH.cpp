/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#include "KlangstromDefines.hpp"

#if (KLST_ARCH == KLST_ARCH_DESKTOP)

#include <chrono>

#include "Arduino.h"
#include "KLST_SDL-adapter.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterface_SDL-ISH.h"
#include "KlangstromDefinesArduino.h"
#include "klangstrom_arduino_sdl.h"

using namespace std;

extern KLST_Emulator mEmulator;

WEAK void serialEvent() {}  // @TODO(check location of this function)

/* ----------------------------------------------------------------------------------------------------- */

void klangstrom::begin_serial_debug(bool wait_for_serial, uint32_t baud_rate) { Serial.begin(115200); }

void klangstrom::option(uint8_t option, float value) {}

float klangstrom::get_option(uint8_t option) { return -1.0; }

void klangstrom::beats_per_minute(float BPM) { klangstrom_arduino_beats_per_minute(BPM); }

void klangstrom::beats_per_minute_ms(uint32_t micro_seconds) { klangstrom_arduino_beats_per_minute_ms(micro_seconds); }

void klangstrom::LED(uint16_t LED, uint8_t state) {
    switch (state) {
        case LED_ON:
            mEmulator.led(LED, state);
            break;
        case LED_OFF:
            mEmulator.led(LED, state);
            break;
        case LED_TOGGLE:
            mEmulator.led_toggle(LED);
            break;
    }
}

bool klangstrom::button_state(uint8_t button) {
    switch (button) {
        case ENCODER_00:
            return get_encoder_button_state(ENCODER_00);
        case ENCODER_01:
            return get_encoder_button_state(ENCODER_01);
        case ENCODER_02:
            return get_encoder_button_state(ENCODER_02);
        case KLST_BUTTON_PROGRAMMER:
            return !digitalRead(BUTTON_PROGRAMMER);
    }
    return false;
}

bool klangstrom::pin_state(uint8_t button) { return false; }

void klangstrom::event_transmit(const uint8_t event, float* payload) { klangstrom_arduino_event_transmit(event, payload); }

void klangstrom::data_transmit(const uint8_t sender, uint8_t* data, uint16_t length) { klangstrom_arduino_data_transmit(sender, data, length); }

int16_t klangstrom::ID() { return KLST_NO_ID; }

uint16_t klangstrom::ADC(uint8_t ADC) { return 0; }  // @todo(implement ADC)

void klangstrom::DAC(uint8_t DAC, uint16_t value) {}  // @todo(implement DAC)

int klangstrom::get_LED_pin(uint16_t index) {
    if (index >= 0 && index < KLST_NUM_LEDS) {
        return LED_00 + index;
    }
    return -1;
}

uint32_t klangstrom::query_available_memory_block() {
// maybe we should set a fixed value here that is somewhat in line with the MCUs e.g
#if (KLANG_BOARD_EMULATOR == KLANG_BOARD_EMULATOR_KLST_SHEEP)
    return 131072;
#elif (KLANG_BOARD_EMULATOR == KLANG_BOARD_EMULATOR_KLST_CORE)
#warning @query_available_memory_block KLANG_BOARD_EMULATOR_KLST_CORE not evaluated yet
    return 131072;
#elif (KLANG_BOARD_EMULATOR == KLANG_BOARD_EMULATOR_KLST_TINY)
#warning @query_available_memory_block KLANG_BOARD_EMULATOR_KLST_TINY not evaluated yet
    return 131072;
#else
#warning @query_available_memory_block KLANG_BOARD_EMULATOR not supported
    return 67108864;
    // uint32_t              size              = 1;
    // uint32_t              acc_size          = 0;
    // uint8_t*              ptr               = nullptr;
    // uint8_t*              largest_block_ptr = nullptr;
    // std::vector<uint8_t*> pointers;
    // while (true) {
    //     ptr = new (std::nothrow) uint8_t[size];
    //     if (ptr == nullptr) {
    //         break;
    //     }
    //     acc_size += size;
    //     largest_block_ptr = ptr;
    //     pointers.push_back(ptr);
    //     size *= 2;
    // }
    // for (auto ptr : pointers) {
    //     delete[] ptr;
    // }
    // return size / 2;
#endif
}

#endif  // (KLST_ARCH==KLST_ARCH_DESKTOP)
