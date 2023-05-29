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

#include "KlangstromApplicationArduino.h"

#include "KlangstromApplicationInterface.h"

WEAK void configure() {}  // called before 'setup()'. hardware is not initialized yet. can e.g be used to configure Serial baud rates, etc.
WEAK void setup() {}
WEAK void loop() {}
WEAK void beat(uint32_t pBeat) {}
WEAK void event_receive(const EVENT_TYPE event, const void* data) {}
WEAK void data_receive(const uint8_t receiver, uint8_t* data, uint8_t length) {}  // called when 'raw' data is received e.g from serial port

WEAK void audioblock(float** input_signal, float** output_signal) {
    for (uint8_t j = 0; j < klangstrom::get_output_channels(); j++) {
        for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
            output_signal[j][i] = 0.0;
        }
    }
}

uint8_t klangstrom::get_input_channels() {
    // TODO might want to differntiate between boards
    return 2;
}

uint8_t klangstrom::get_output_channels() {
    // TODO might want to differntiate between boards
    return 2;
}