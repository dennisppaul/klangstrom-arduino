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

#ifndef KlangstromApplicationInterface_h
#define KlangstromApplicationInterface_h

#ifdef __cplusplus
#include "KlangstromDefines.hpp"
#include "KlangstromEvents.h"
#endif  // __cplusplus
#include "KlangstromDefinesArduino.h"

/* --------------------------------------------------------------------------------------------- */
/* --- application-level functions ------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

#ifdef __cplusplus

namespace klangstrom {
    void     beats_per_minute(float pBPM);
    void     beats_per_minute_ms(uint32_t pMicroSeconds);
    void     begin_serial_debug(bool pWaitForSerial = false, uint32_t pBaudRate = 115200);
    bool     button_state(uint8_t pButton);
    void     data_transmit(const uint8_t pTransmitter, uint8_t* pData, uint8_t pDataLength);
    void     event_transmit(EVENT_TYPE pEvent, float* pPayload);
    int16_t  ID();
    char*    U_ID();
    void     LED(uint16_t pLED, uint8_t pState);
    void     led(uint16_t pLED, bool pState);
    void     led_toggle(uint16_t pLED);
    void     option(uint8_t pOption, float pValue);
    float    get_option(uint8_t pOption);
    bool     pin_state(uint8_t pButton);
    uint16_t adc(uint8_t pADC);
    void     dac(uint8_t pDAC, uint16_t pValue);
};  // namespace klangstrom

#endif  // __cplusplus

#endif /* KlangstromApplicationInterface_h */
