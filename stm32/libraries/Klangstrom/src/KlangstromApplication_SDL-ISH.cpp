/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
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

#if (KLST_ARCH==KLST_ARCH_DESKTOP)

#include "KLST_SDL-adapter.h"

#include <chrono>

#include "Arduino.h"
#include "KlangstromApplicationArduino.h"
#include "KlangstromApplicationInterface_SDL-ISH.h"
#include "KlangstromDefinesArduino.h"
#include "klangstrom_arduino_sdl.h"

using namespace std;

extern KLST_Simulator mSimulator;

WEAK void serialEvent() {} // @TODO(check location of this function)

/* ----------------------------------------------------------------------------------------------------- */

void klangstrom::begin_serial_debug(bool pWaitForSerial, uint32_t pBaudRate) { Serial.begin(115200); }

void klangstrom::option(uint8_t pOption, float pValue) {}

void klangstrom::beats_per_minute(float pBPM) { klangstrom_arduino_beats_per_minute(pBPM); }

void klangstrom::beats_per_minute_ms(uint32_t pMicroSeconds) { klangstrom_arduino_beats_per_minute_ms(pMicroSeconds); }

void klangstrom::LED(uint16_t pLED, uint8_t pState) {
    switch (pState) {
        case LED_ON:
            mSimulator.led(pLED, pState);
            break;
        case LED_OFF:
            mSimulator.led(pLED, pState);
            break;
        case LED_TOGGLE:
            mSimulator.led_toggle(pLED);
            break;
    }
}

void klangstrom::led(uint16_t pLED, bool pState) {
    mSimulator.led(pLED, pState);
}

void klangstrom::led_toggle(uint16_t pLED) {
    mSimulator.led_toggle(pLED);
}

bool klangstrom::button_state(uint8_t pButton) { return false; }

bool klangstrom::pin_state(uint8_t pButton) { return false; }

void klangstrom::event_transmit(const uint8_t pEvent, float* pPayload) { klangstrom_arduino_event_transmit(pEvent, pPayload); }

void klangstrom::data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength) { klangstrom_arduino_data_transmit(pSender, pData, pDataLength); }

int16_t klangstrom::ID() { return KLST_NO_ID; }

uint16_t klangstrom::adc(uint8_t pADC) { return 0; }  // @todo(implement ADC)

void klangstrom::dac(uint8_t pDAC, uint16_t pValue) {}  // @todo(implement DAC)


#endif // (KLST_ARCH==KLST_ARCH_DESKTOP)
